"""
Renders a parsed Nextion project (`{page_internal_name: [ComponentInfo, ...]}`)
into the two files the driver core expects: nextion_layout.c and
nextion_layout.h.

Naming rule (this is what ties generated identifiers back to the actual
Nextion project instead of to whatever filenames were used to read it):

  - `page_name` here is always the page's *internal* name as read from the
    binary itself by nextion_parser.parse_page_data — never a filename.
  - Page struct   -> `page_<page_name>`            (e.g. page_dashboard)
  - Page enum     -> `PAGE_<PAGE_NAME>`             (e.g. PAGE_DASHBOARD)
  - Object struct -> `nx_<component.name>`          (component.name already
                      comes from the widget's own objname/objectname field,
                      so no equivalent bug is possible there)

Output is split in five clearly delimited sections, generated in this order:
  1. Per-page object instances + the page struct that owns them
  2. The master page tree (array of all page pointers)
  3. Transport + FIFO instances
  4. The screen instance (top-level driver handle)
  5. Callback prototypes for every widget with an onPress/onRelease handler
"""

from typing import Dict, List

from nextion_types import ComponentInfo, PAGE_GENERIC_TYPE, TYPE_MAP, get_c_library_component

GENERATED_FILE_BANNER = (
    "// ============================================================================\n"
    "// GENERATED AUTOMATICALLY BY NEXTION PARSER ENGINE — {kind}\n"
    "// DO NOT MODIFY MANUALLY\n"
    "// ============================================================================\n"
)

SECTION_RULE = "// " + "-" * 74


def generate_c_layout(
    project_pages: Dict[str, List[ComponentInfo]],
    output_c: str = "nextion_layout.c",
    output_h: str = "nextion_layout.h",
) -> None:
    """Generate both nextion_layout.c and nextion_layout.h for `project_pages`.

    `project_pages` keys must already be each page's internal name (see the
    naming rule in this module's docstring) — this function does not know
    or care where that name came from, it just uses it verbatim.
    """
    generate_c_file(project_pages, output_c, output_h)
    generate_h_file(project_pages, output_h)


def _is_child_widget(component: ComponentInfo) -> bool:
    """True for anything that should get a generated NX_Object_t — i.e.
    everything except the page's own self-descriptive entry."""
    return not component.is_page_entry


# ---------------------------------------------------------------------------
# .c file
# ---------------------------------------------------------------------------
def generate_c_file(
    project_pages: Dict[str, List[ComponentInfo]], output_c: str, output_h: str
) -> None:
    c_code: List[str] = [GENERATED_FILE_BANNER.format(kind="IMPLEMENTATION")]
    c_code.append(f'#include "{output_h}"\n')

    declared_pages = []
    c_code.append("// ============================================================================")
    c_code.append("// 1. Screen Layouts & Object Instances (one block per page)")
    c_code.append("// ============================================================================")
    for page_index, (page_name, components) in enumerate(project_pages.items()):
        c_code.append(_render_page_block(page_index, page_name, components, declared_pages))

    c_code.append("// ============================================================================")
    c_code.append("// 2. Master Project Page Tree")
    c_code.append("// ============================================================================")
    c_code.append("static NX_Page_t *nextion_page_tree[] = {")
    c_code.append(",\n".join(declared_pages))
    c_code.append("};")
    c_code.append(SECTION_RULE + "\n")

    c_code.append("// ============================================================================")
    c_code.append("// 3. Transport and FIFO Instances")
    c_code.append("// ============================================================================")
    c_code.append(_render_transport_block())
    c_code.append(SECTION_RULE + "\n")

    c_code.append("// ============================================================================")
    c_code.append("// 4. Screen (top-level driver handle)")
    c_code.append("// ============================================================================")
    c_code.append(_render_screen_block())
    c_code.append(SECTION_RULE + "\n")

    c_code.append("// ============================================================================")
    c_code.append("// 5. Component Widget Event Callback Stubs")
    c_code.append("// ============================================================================")
    c_code.append(_render_callback_stubs(project_pages))

    c_code.append("/******************* END OF FILE *********************************************/")

    with open(output_c, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(c_code))
    print(f"[OK] Archivo C creado: '{output_c}'")


def _render_page_block(
    page_index: int,
    page_name: str,
    components: List[ComponentInfo],
    declared_pages: List[str],
) -> str:
    """Render one page's object instances + its NX_Page_t, and register its
    struct name into `declared_pages` for the master page tree."""
    lines = [f"// --- Page: {page_name}  (internal name read from source, ID: {page_index}) ---"]

    child_widgets = [c for c in components if _is_child_widget(c)]
    object_pointer_names = []

    for widget in child_widgets:
        c_component_var = get_c_library_component(widget.generic_type)
        press_callback = f"On_{widget.name}_Press" if widget.has_press else "NULL"
        release_callback = f"On_{widget.name}_Release" if widget.has_release else "NULL"

        lines.append(
            f"""NX_Object_t nx_{widget.name} = {{
    .component = &{c_component_var},
    .pageId = {page_index},
    .componentId = {widget.component_id},
    .objectName = {{ .data = "{widget.name}", .length = {len(widget.name)} }},
    .onPress = {press_callback},
    .onRelease = {release_callback},
    .userData = NULL
}};"""
        )
        object_pointer_names.append(f"    &nx_{widget.name}")

    page_struct_name = f"page_{page_name}"
    declared_pages.append(f"    &{page_struct_name}")

    if object_pointer_names:
        objects_array_name = f"page{page_index}_objects"
        lines.append(f"\nstatic NX_Object_t *{objects_array_name}[] = {{")
        lines.append(",\n".join(object_pointer_names))
        lines.append("};")
        lines.append(
            f"""NX_Page_t {page_struct_name} = {{
    .pageId = {page_index},
    .pageName = {{ .data = "{page_name}", .length = {len(page_name)} }},
    .objects = {objects_array_name},
    .objectCount = (uint8_t)(sizeof({objects_array_name}) / sizeof({objects_array_name}[0])),
    .onInit = NULL
}};"""
        )
    else:
        lines.append(
            f"""NX_Page_t {page_struct_name} = {{
    .pageId = {page_index},
    .pageName = {{ .data = "{page_name}", .length = {len(page_name)} }},
    .objects = NULL,
    .objectCount = 0,
    .onInit = NULL
}};"""
        )

    lines.append("\n" + SECTION_RULE + "\n")
    return "\n".join(lines)


def _render_transport_block() -> str:
    return (
        "static NX_VectorFifo_t rx, tx;\n\n"
        "NX_Transport_t transportDriver = {\n"
        "    .rxFifo = &rx,\n"
        "    .txFifo = &tx,\n"
        "    .portId = 0,\n"
        "    .rx_hardware_func = NULL,\n"
        "    .task_transport = NULL,\n"
        "    .tx_hardware_func = NULL\n"
        "};"
    )


def _render_screen_block() -> str:
    return (
        "NX_Screen_t miDispositivoHMI = {\n"
        "    .transport = &transportDriver,\n"
        "    .pages = nextion_page_tree,\n"
        "    .pageCount = sizeof(nextion_page_tree) / sizeof(nextion_page_tree[0]),\n"
        "    .currentPage = NULL,\n"
        "    .globalEvent = ScreenGlobalEvent\n"
        "};"
    )


def _render_callback_stubs(project_pages: Dict[str, List[ComponentInfo]]) -> str:
    lines = []
    for components in project_pages.values():
        for widget in components:
            if not _is_child_widget(widget):
                continue
            if widget.has_press:
                lines.append(f"void On_{widget.name}_Press(NX_Object_t *obj)")
                lines.append("{\n// implement your callback here\n}")
            if widget.has_release:
                lines.append(f"void On_{widget.name}_Release(NX_Object_t *obj)")
                lines.append("{\n// implement your callback here\n}")

    if not lines:
        return "// No active component local callbacks detected."
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# .h file
# ---------------------------------------------------------------------------
def generate_h_file(project_pages: Dict[str, List[ComponentInfo]], output_h: str) -> None:
    h_code: List[str] = [GENERATED_FILE_BANNER.format(kind="HEADER INTERFACE")]
    h_code.append("#ifndef NEXTION_LAYOUT_H_")
    h_code.append("#define NEXTION_LAYOUT_H_\n")
    h_code.append('#include "nextion.h"\n')

    h_code.append("// ============================================================================")
    h_code.append("// Page Index Enumeration (order matches page IDs assigned during generation)")
    h_code.append("// ============================================================================")
    h_code.append("typedef enum {")
    # PAGE_<NAME> uses each page's internal name — same source of truth as
    # the page_<name> struct generated in the .c file, so the two always agree.
    page_enum_entries = [f"    PAGE_{page_name.upper()}" for page_name in project_pages.keys()]
    h_code.append(",\n".join(page_enum_entries))
    h_code.append("} NX_PageId_e;\n")

    h_code.append("// ============================================================================")
    h_code.append("// Component Widget Event Callback Prototypes")
    h_code.append("// ----------------------------------------------------------------------------")
    h_code.append("// Signature MUST match NX_ObjectCallback_t (nx_types.h): void (*)(NX_Object_t*).")
    h_code.append("// Any extra parameter here makes it incompatible with NX_Object_t.onPress/")
    h_code.append("// onRelease and will fail to compile where the object literal assigns it.")
    h_code.append("// ============================================================================")
    press_prototypes = [
        f"void On_{widget.name}_Press(NX_Object_t *obj);"
        for components in project_pages.values()
        for widget in components
        if _is_child_widget(widget) and widget.has_press
    ]
    release_prototypes = [
        f"void On_{widget.name}_Release(NX_Object_t *obj);"
        for components in project_pages.values()
        for widget in components
        if _is_child_widget(widget) and widget.has_release
    ]
    all_prototypes = press_prototypes + release_prototypes

    h_code.append("\n".join(all_prototypes) if all_prototypes else "// No active component local callbacks detected.")
    h_code.append("")

    h_code.append("// ============================================================================")
    h_code.append("// Public Component Object Instances")
    h_code.append("// ============================================================================")
    object_externs = [
        f"extern NX_Object_t nx_{widget.name};"
        for components in project_pages.values()
        for widget in components
        if _is_child_widget(widget)
    ]
    h_code.append("\n".join(object_externs))
    h_code.append("")

    h_code.append("// ============================================================================")
    h_code.append("// Public Page Instances")
    h_code.append("// ----------------------------------------------------------------------------")
    h_code.append("// Needed by any translation unit that navigates pages, e.g.:")
    h_code.append("//   NX_setPage(&miDispositivoHMI, &page_dashboard_main);")
    h_code.append("// ============================================================================")
    page_externs = [f"extern NX_Page_t page_{page_name};" for page_name in project_pages.keys()]
    h_code.append("\n".join(page_externs))
    h_code.append("")

    h_code.append("// ============================================================================")
    h_code.append("// Global Event System")
    h_code.append("// ============================================================================")
    h_code.append("NX_Status_t ScreenGlobalEvent(NX_Vector_t *unhandled_vector);\n")

    h_code.append("// ============================================================================")
    h_code.append("// Public HMI Device Instance")
    h_code.append("// ============================================================================")
    h_code.append("extern NX_Screen_t miDispositivoHMI;\n")
    h_code.append("#endif // NEXTION_LAYOUT_H_")
    h_code.append("/******************* END OF FILE *********************************************/")

    with open(output_h, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(h_code))
    print(f"[OK] Archivo H creado con instancias estáticas públicas: '{output_h}'")
