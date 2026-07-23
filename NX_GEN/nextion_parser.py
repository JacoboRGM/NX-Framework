"""
Parses a raw Nextion .page export into a `ParsedPage` (the page's own
internal name + every child widget on it), ready to hand to
`nextion_codegen.generate_c_layout`.

--------------------------------------------------------------------------
File format this parser understands
--------------------------------------------------------------------------
A .page export is a sequence of "component blocks". Each block starts at
an occurrence of the literal bytes b"type" and runs up to the next
occurrence (or end of file). Every block is expected to contain, in no
fixed order:

  - b"type" marker  -> followed eventually by a 0x11 delimiter; the byte
                       immediately before that delimiter is the widget's
                       Nextion protocol type byte (see nextion_types.TYPE_MAP).
  - b"id"           -> followed by zero or more 0x00 padding bytes, then a
                       single byte holding the widget's numeric component ID.
  - b"objname" / b"objectname"
                    -> followed by zero or more 0x00 padding bytes, then the
                       ASCII widget name, terminated by a 0x11 delimiter.
  - b"sendkey"      -> (optional) followed eventually by a 0x11 delimiter;
                       the byte before it flags which touch events this
                       widget reports: 0x01 = release only, 0x02 = press
                       only, 0x03 = both.

The page itself is just another block (type byte 121, generic type
"page") — its "id" and "objname" fields describe the page, not a widget on
it, which is exactly how we recover the page's real internal name.

Some exported files are not raw binary but a plain-text hex dump (one
address/offset column followed by space-separated byte pairs). `_coerce_to_binary`
detects and normalizes that case before parsing.
"""

import os
import re
from typing import List, Optional, Tuple

from nextion_types import ComponentInfo, ParsedPage, TYPE_MAP
from nextion_codegen import generate_c_layout

# ---------------------------------------------------------------------------
# Protocol constants (named instead of magic numbers/bytes)
# ---------------------------------------------------------------------------
FIELD_TYPE_MARKER = b"type"
FIELD_ID_MARKER = b"id"
FIELD_OBJNAME_MARKERS = (b"objname", b"objectname")
FIELD_SENDKEY_MARKER = b"sendkey"

DELIMITER = b"\x11"          # terminates a "type" or "objname" field
PADDING_BYTE = 0x00           # zero-padding skipped before id/objname values

SENDKEY_RELEASE_ONLY = 0x01
SENDKEY_PRESS_ONLY = 0x02
SENDKEY_PRESS_AND_RELEASE = 0x03

# How far past the "type" marker to start looking for the 0x11 delimiter.
# The type byte itself always sits a few bytes after the marker; 4 is the
# minimum offset observed across exported projects.
TYPE_FIELD_SEARCH_OFFSET = 4


# ---------------------------------------------------------------------------
# Step 1 — normalize input to raw binary
# ---------------------------------------------------------------------------
def _coerce_to_binary(raw_content: bytes) -> bytes:
    """Return `raw_content` as raw binary, decoding a hex-dump export if needed.

    Some tools export .page data as human-readable text: each line is an
    optional address/offset column followed by space-separated two-character
    hex byte pairs. If the first 500 bytes look like that (clean ASCII,
    contains whitespace, and contains hex-looking byte pairs), we reconstruct
    the actual binary from it. Otherwise we assume the file is already raw
    binary and return it unchanged.
    """
    try:
        sample = raw_content[:500].decode("ascii", errors="strict")
    except (ValueError, UnicodeDecodeError):
        return raw_content

    looks_like_hex_dump = (
        any(char in sample for char in ("\n", " "))
        and re.search(r"[0-9a-fA-F]{2}", sample) is not None
    )
    if not looks_like_hex_dump:
        return raw_content

    hex_digits = ""
    for line in raw_content.decode("ascii", errors="ignore").strip().split("\n"):
        tokens = line.split()
        if not tokens:
            continue
        # Lines with more than 4 tokens are assumed to start with an
        # address/offset column, which we skip.
        first_data_token = 1 if len(tokens) > 4 else 0
        for token in tokens[first_data_token:]:
            if len(token) == 2 and all(c in "0123456789abcdefABCDEF" for c in token):
                hex_digits += token

    try:
        return bytes.fromhex(hex_digits)
    except ValueError:
        # Malformed hex stream — fall back to treating the file as raw binary.
        return raw_content


# ---------------------------------------------------------------------------
# Step 2 — split into one block per widget/page entry
# ---------------------------------------------------------------------------
def _split_into_component_blocks(binary_data: bytes) -> List[bytes]:
    """Slice `binary_data` at every occurrence of the "type" field marker."""
    marker_positions = [m.start() for m in re.finditer(FIELD_TYPE_MARKER, binary_data)]

    blocks = []
    for i, start in enumerate(marker_positions):
        end = marker_positions[i + 1] if i + 1 < len(marker_positions) else len(binary_data)
        blocks.append(binary_data[start:end])
    return blocks


# ---------------------------------------------------------------------------
# Step 3 — field extractors, one responsibility each
# ---------------------------------------------------------------------------
def _parse_type_byte(block: bytes) -> Optional[int]:
    """Return the widget's Nextion protocol type byte, or None if not found."""
    delimiter_idx = block.find(DELIMITER, TYPE_FIELD_SEARCH_OFFSET)
    if delimiter_idx == -1:
        return None
    return block[delimiter_idx - 1]


def _parse_component_id(block: bytes) -> str:
    """Return the widget's numeric component ID as a string, or "N/A"."""
    id_marker_idx = block.find(FIELD_ID_MARKER)
    if id_marker_idx == -1:
        return "N/A"

    cursor = id_marker_idx + len(FIELD_ID_MARKER)
    while cursor < len(block) and block[cursor] == PADDING_BYTE:
        cursor += 1

    if cursor >= len(block):
        return "N/A"
    return str(block[cursor])


def _parse_component_name(block: bytes) -> str:
    """Return the widget's (or page's) ASCII name, or "N/A" if not found."""
    for marker in FIELD_OBJNAME_MARKERS:
        name_marker_idx = block.find(marker)
        if name_marker_idx != -1:
            cursor = name_marker_idx + len(marker)
            break
    else:
        return "N/A"

    while cursor < len(block) and block[cursor] == PADDING_BYTE:
        cursor += 1

    name_bytes = bytearray()
    while cursor < len(block) and block[cursor] != DELIMITER[0]:
        name_bytes.append(block[cursor])
        cursor += 1

    return name_bytes.decode("ascii", errors="ignore")


def _parse_touch_flags(block: bytes) -> Tuple[bool, bool]:
    """Return (has_press, has_release) from the optional "sendkey" field."""
    sendkey_marker_idx = block.find(FIELD_SENDKEY_MARKER)
    if sendkey_marker_idx == -1:
        return False, False

    search_from = sendkey_marker_idx + len(FIELD_SENDKEY_MARKER)
    delimiter_idx = block.find(DELIMITER, search_from)
    if delimiter_idx == -1:
        return False, False

    sendkey_value = block[delimiter_idx - 1]
    if sendkey_value == SENDKEY_PRESS_AND_RELEASE:
        return True, True
    if sendkey_value == SENDKEY_PRESS_ONLY:
        return True, False
    if sendkey_value == SENDKEY_RELEASE_ONLY:
        return False, True
    return False, False


def _parse_component_block(block: bytes) -> Optional[ComponentInfo]:
    """Parse a single component block into a ComponentInfo, or None if
    the block doesn't carry a complete/valid widget definition."""
    type_byte = _parse_type_byte(block)
    component_id = _parse_component_id(block)
    name = _parse_component_name(block)

    if type_byte is None or component_id == "N/A" or name in ("N/A", ""):
        return None

    has_press, has_release = _parse_touch_flags(block)
    return ComponentInfo(
        component_id=component_id,
        name=name,
        type_byte=type_byte,
        has_press=has_press,
        has_release=has_release,
    )


# ---------------------------------------------------------------------------
# Public entry point
# ---------------------------------------------------------------------------
def parse_page_data(raw_content: bytes) -> ParsedPage:
    """Parse one .page file's raw bytes into a ParsedPage.

    `ParsedPage.internal_name` is read from the page's *own* entry inside the
    file (its objname/objectname field) — never from the filename the data
    was read from. If the page's own entry can't be found or decoded,
    `internal_name` is None and the caller must decide on (and warn about)
    a fallback.
    """
    binary_data = _coerce_to_binary(raw_content)
    blocks = _split_into_component_blocks(binary_data)

    components: List[ComponentInfo] = []
    for block in blocks:
        parsed = _parse_component_block(block)
        if parsed is not None:
            components.append(parsed)

    page_entries = [c for c in components if c.is_page_entry]
    internal_name = page_entries[0].name if page_entries else None
    if len(page_entries) > 1:
        print(
            f"[Warning] Se encontraron {len(page_entries)} entradas de tipo "
            f"'page' en un mismo archivo; se usará la primera ('{internal_name}')."
        )

    return ParsedPage(internal_name=internal_name, components=components)


# ---------------------------------------------------------------------------
# CLI entry point
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    print("=== Nextion Decoupled Multi-Page Code Generator ===")
    print("Ingresa las rutas de tus archivos .page secuencialmente. Para terminar escribe 'FIN'.\n")

    project_pages = {}
    page_counter = 0

    while True:
        ruta = input(f"Ruta para la Página {page_counter} (o 'FIN'): ").strip()
        if not ruta or ruta.upper() == "FIN":
            break
        ruta = ruta.strip('"').strip("'")
        if not os.path.exists(ruta):
            print("[Error] El archivo no existe. Reintenta.")
            continue

        try:
            with open(ruta, "rb") as f:
                content = f.read()
            parsed_page = parse_page_data(content)
        except Exception as e:
            print(f"[Error] Error de lectura: {e}\n")
            continue

        page_name = parsed_page.internal_name
        if page_name is None:
            # Fallback only when the internal page name genuinely can't be
            # recovered — always surfaced to the user, never silent, since a
            # silent filename fallback is exactly the bug this fixes.
            page_name = os.path.splitext(os.path.basename(ruta))[0]
            print(
                f"[Warning] No se pudo leer el nombre interno de la página en "
                f"'{ruta}'. Se usará el nombre de archivo como respaldo: '{page_name}'."
            )

        if page_name in project_pages:
            print(
                f"[Warning] Ya existe una página registrada con el nombre "
                f"'{page_name}'; se sobrescribirá con el contenido de '{ruta}'."
            )

        project_pages[page_name] = parsed_page.components
        print(f"[OK] Página '{page_name}' (leída de '{ruta}') procesada y guardada en búfer.\n")
        page_counter += 1

    if project_pages:
        generate_c_layout(project_pages)
    else:
        print("\nNo se procesaron datos, abortando generación de código C.")
