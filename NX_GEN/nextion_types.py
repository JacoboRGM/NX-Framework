"""
Static lookup tables shared by the parser and the code generator, plus the
data structures used to pass parsed component information between them.

- TYPE_MAP        : Nextion protocol type byte  -> generic widget type name.
- C_LIBRARY_MAP   : generic widget type name    -> C blueprint variable name
                     declared in the driver core (lib/core/nx_component.c).
- ComponentInfo   : one parsed widget (button, text, gauge... or the page
                     itself) as extracted from a raw .page dump.
"""

from typing import NamedTuple, Optional

# ---------------------------------------------------------------------------
# Nextion protocol type byte -> generic widget type name
# ---------------------------------------------------------------------------
# This is the official Nextion HMI type-byte table. The byte value is what
# the Nextion Editor stores per-widget in the exported .page binary; the
# string is the generic name used everywhere else in this toolchain.
TYPE_MAP = {
    122: "gauge",          # 'z'
    121: "page",           # 'y'
    116: "text",           # 't'
    113: "crop",           # 'q'
    112: "picture",        # 'p'
    109: "hotspot",        # 'm'
    106: "progress bar",   # 'j'
    98: "button",          # 'b'
    68: "select text",     # 'select'
    67: "switch",          # 'sw'
    66: "data record",     # 'data'
    65: "file browser",    # 'fbrowser'
    63: "file stream",     # 'fs'
    62: "SLText",          # 'SLText'
    61: "combo box",       # 'cb'
    60: "ex picture",      # 'exp'
    59: "xfloat",          # 'x'
    58: "QR code",         # 'qr'
    57: "radio",           # 'r'
    56: "check box",       # 'c'
    55: "scroll text",     # 'g'
    54: "number",          # 'n'
    53: "dual button",     # 'bt'
    52: "variable",        # 'va'
    51: "timer",           # 'tm'
    5: "touch cap",        # 'tc'
    4: "audio",            # 'wav'
    3: "video",            # 'v'
    2: "gmov",             # 'gm'
    1: "slider",           # 'h'
    0: "waveform",         # 's'
}

# Reserved generic type name used exclusively for the page's own entry.
# A .page file describes both the page itself (one entry, type byte 121)
# and every child widget placed on it (all other entries).
PAGE_GENERIC_TYPE = "page"

# ---------------------------------------------------------------------------
# generic widget type name -> C blueprint variable declared in lib/core
# ---------------------------------------------------------------------------
C_LIBRARY_MAP = {
    "page":          "nx_page",
    "button":        "nx_button",
    "text":          "nx_text",
    "number":        "nx_number",
    "xfloat":        "nx_xfloat",
    "scroll text":   "nx_scroll_text",
    "progress bar":  "nx_progress_bar",
    "picture":       "nx_picture",
    "crop":          "nx_crop",
    "hotspot":       "nx_hotspot",
    "touch cap":     "nx_touch_cap",
    "gauge":         "nx_gauge",
    "slider":        "nx_slider",
    "timer":         "nx_timer",
    "variable":      "nx_variable",
    "dual button":   "nx_dual_button",
    "check box":     "nx_check_box",
    "QR code":       "nx_qr_code",
    "switch":        "nx_switch",
    "waveform":      "nx_waveform",
    "gmov":          "nx_gmov",
    "combo box":     "nx_combo_box",
    "select text":   "nx_select_text",
    "SLText":        "nx_sltext",
    "sltext":        "nx_sltext",
    "radio":         "nx_radio_button",
    "data record":   "nx_data_record",
    "file browser":  "nx_file_browser",
    "video":         "nx_video",
    "ex picture":    "nx_ex_picture",
    "file stream":   "nx_file_stream",
    "audio":         "nx_audio",
}


def get_c_library_component(generic_type: str) -> str:
    """Return the C blueprint variable name for a generic widget type.

    Falls back to ``nx_unknown_<generic_type>`` for any type not present in
    C_LIBRARY_MAP, so a project with an unsupported widget still generates
    code that fails loudly at compile time instead of silently at runtime.
    """
    return C_LIBRARY_MAP.get(generic_type, f"nx_unknown_{generic_type}")


# ---------------------------------------------------------------------------
# Parsed data structures
# ---------------------------------------------------------------------------
class ComponentInfo(NamedTuple):
    """One widget entry parsed out of a raw .page binary dump.

    Behaves exactly like the plain 5-tuple this replaces
    ``(comp_id, obj_name, type_byte, has_press, has_release)`` — it still
    unpacks the same way in a ``for`` loop — but every field is now named,
    which is what makes the parser and code generator readable.
    """
    component_id: str
    name: str
    type_byte: int
    has_press: bool
    has_release: bool

    @property
    def generic_type(self) -> str:
        """Generic widget type name (e.g. 'button', 'page', 'unknown')."""
        return TYPE_MAP.get(self.type_byte, "unknown")

    @property
    def is_page_entry(self) -> bool:
        """True if this entry is the page's own definition, not a child widget."""
        return self.generic_type == PAGE_GENERIC_TYPE


class ParsedPage(NamedTuple):
    """Result of parsing one .page file.

    `internal_name` is the page's own name as declared *inside* the binary
    (read from its own objname/objectname field) — this is what must be used
    to name the generated C page struct and enum entry. It is `None` only if
    the page's own entry could not be located/decoded, in which case the
    caller is responsible for deciding on a fallback (and should warn the
    user loudly, since silently falling back to the source filename is
    exactly the bug this refactor fixes).
    """
    internal_name: Optional[str]
    components: list
