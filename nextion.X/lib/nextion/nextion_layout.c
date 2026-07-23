// ============================================================================
// GENERATED AUTOMATICALLY BY NEXTION PARSER ENGINE - IMPLEMENTATION
// DO NOT MODIFY MANUALLY
// ============================================================================

#include <stdio.h>
#include "nextion_layout.h"
#include "../core/nx_types.h"
#include "../core/nx_component.h"
#include "../core/nx_object.h"
#include "../core/nx_page.h"
#include "../core/nx_screen.h"
#include "../core/nx_fifo.h"
#include "../core/nx_transport.h"
#include "../core/nx_api.h"

#include "../../mcc_generated_files/pin_manager.h"

// ============================================================================
// 1.- Screen Layouts & Objects Instantiation
// ============================================================================
// --- Componentes para la Pagina: dashboard_page1 (ID: 0) ---
NX_Object_t btn_start = {
    .component = &nx_button,
    .pageId = 0,
    .componentId = 2,
    .objectName = { .data = "btn_start", .length = 9 },
    .onPress = On_btn_start_Press,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t t0 = {
    .component = &nx_text,
    .pageId = 0,
    .componentId = 3,
    .objectName = { .data = "t0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t btn_stop = {
    .component = &nx_button,
    .pageId = 0,
    .componentId = 1,
    .objectName = { .data = "btn_stop", .length = 8 },
    .onPress = NULL,
    .onRelease = On_btn_stop_Release,
    .userData = NULL
};
NX_Object_t t1 = {
    .component = &nx_text,
    .pageId = 0,
    .componentId = 4,
    .objectName = { .data = "t1", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t number0 = {
    .component = &nx_number,
    .pageId = 0,
    .componentId = 5,
    .objectName = { .data = "number0", .length = 7 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t x0 = {
    .component = &nx_xfloat,
    .pageId = 0,
    .componentId = 6,
    .objectName = { .data = "x0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t g0 = {
    .component = &nx_scroll_text,
    .pageId = 0,
    .componentId = 7,
    .objectName = { .data = "g0", .length = 2 },
    .onPress = NULL,
    .onRelease = On_g0_Release,
    .userData = NULL
};
NX_Object_t nx_j0 = {
    .component = &nx_progress_bar,
    .pageId = 0,
    .componentId = 8,
    .objectName = { .data = "j0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t p0 = {
    .component = &nx_picture,
    .pageId = 0,
    .componentId = 9,
    .objectName = { .data = "p0", .length = 2 },
    .onPress = NULL,
    .onRelease = On_p0_Release,
    .userData = NULL
};
NX_Object_t q0 = {
    .component = &nx_crop,
    .pageId = 0,
    .componentId = 10,
    .objectName = { .data = "q0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t m0 = {
    .component = &nx_hotspot,
    .pageId = 0,
    .componentId = 11,
    .objectName = { .data = "m0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t tc0 = {
    .component = &nx_touch_cap,
    .pageId = 0,
    .componentId = 12,
    .objectName = { .data = "tc0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t z0 = {
    .component = &nx_gauge,
    .pageId = 0,
    .componentId = 13,
    .objectName = { .data = "z0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t s0 = {
    .component = &nx_waveform,
    .pageId = 0,
    .componentId = 14,
    .objectName = { .data = "s0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t h0 = {
    .component = &nx_slider,
    .pageId = 0,
    .componentId = 15,
    .objectName = { .data = "h0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t tm0 = {
    .component = &nx_timer,
    .pageId = 0,
    .componentId = 16,
    .objectName = { .data = "tm0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t va0 = {
    .component = &nx_variable,
    .pageId = 0,
    .componentId = 17,
    .objectName = { .data = "va0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t bt0 = {
    .component = &nx_dual_button,
    .pageId = 0,
    .componentId = 18,
    .objectName = { .data = "bt0", .length = 3 },
    .onPress = On_bt0_Press,
    .onRelease = On_bt0_Release,
    .userData = NULL
};
NX_Object_t c0 = {
    .component = &nx_check_box,
    .pageId = 0,
    .componentId = 19,
    .objectName = { .data = "c0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t qr0 = {
    .component = &nx_qr_code,
    .pageId = 0,
    .componentId = 20,
    .objectName = { .data = "qr0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t sw0 = {
    .component = &nx_switch,
    .pageId = 0,
    .componentId = 21,
    .objectName = { .data = "sw0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t gm0 = {
    .component = &nx_gmov,
    .pageId = 0,
    .componentId = 22,
    .objectName = { .data = "gm0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t cb0 = {
    .component = &nx_combo_box,
    .pageId = 0,
    .componentId = 23,
    .objectName = { .data = "cb0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t select0 = {
    .component = &nx_select_text,
    .pageId = 0,
    .componentId = 24,
    .objectName = { .data = "select0", .length = 7 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t slt0 = {
    .component = &nx_sltext,
    .pageId = 0,
    .componentId = 25,
    .objectName = { .data = "slt0", .length = 4 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t r0 = {
    .component = &nx_radio_button,
    .pageId = 0,
    .componentId = 26,
    .objectName = { .data = "r0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};

static NX_Object_t *page0_objects[] = {
    &btn_start,
    &t0,
    &btn_stop,
    &t1,
    &number0,
    &x0,
    &g0,
    &nx_j0,
    &p0,
    &q0,
    &m0,
    &tc0,
    &z0,
    &s0,
    &h0,
    &tm0,
    &va0,
    &bt0,
    &c0,
    &qr0,
    &sw0,
    &gm0,
    &cb0,
    &select0,
    &slt0,
    &r0
};
NX_Page_t page_dashboard = {
    .pageId = 0,
    .pageName = { .data = "dashboard", .length = 9 },
    .objects = page0_objects,
    .objectCount = (uint8_t)(sizeof(page0_objects) / sizeof(page0_objects[0])),
    .onInit = NULL
};

// --------------------------------------------------------------------------

// --- Componentes para la Página: dashboard_page2 (ID: 1) ---
NX_Object_t data0 = {
    .component = &nx_data_record,
    .pageId = 1,
    .componentId = 1,
    .objectName = { .data = "data0", .length = 5 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t fbrowser0 = {
    .component = &nx_file_browser,
    .pageId = 1,
    .componentId = 2,
    .objectName = { .data = "fbrowser0", .length = 9 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t fs0 = {
    .component = &nx_file_stream,
    .pageId = 1,
    .componentId = 3,
    .objectName = { .data = "fs0", .length = 3 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t v0 = {
    .component = &nx_video,
    .pageId = 1,
    .componentId = 4,
    .objectName = { .data = "v0", .length = 2 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t wav0 = {
    .component = &nx_audio,
    .pageId = 1,
    .componentId = 5,
    .objectName = { .data = "wav0", .length = 4 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};
NX_Object_t exp0 = {
    .component = &nx_ex_picture,
    .pageId = 1,
    .componentId = 6,
    .objectName = { .data = "exp0", .length = 4 },
    .onPress = NULL,
    .onRelease = NULL,
    .userData = NULL
};

static NX_Object_t *page1_objects[] = {
    &data0,
    &fbrowser0,
    &fs0,
    &v0,
    &wav0,
    &exp0
};
NX_Page_t page_page0 = {
    .pageId = 1,
    .pageName = { .data = "page0", .length = 5 },
    .objects = page1_objects,
    .objectCount = (uint8_t)(sizeof(page1_objects) / sizeof(page1_objects[0])),
    .onInit = NULL
};

// --------------------------------------------------------------------------

// ============================================================================
// 2.- Master Project Page Tree
// ============================================================================
static NX_Page_t *nextion_page_tree[] = {
    &page_dashboard,
    &page_page0
};

// --------------------------------------------------------------------------

// ============================================================================
// 3.- Transport and components
// ============================================================================
static NX_VectorFifo_t rx, tx;

NX_Transport_t transportDriver = {
    .rxFifo = &rx,
    .txFifo = &tx,
    .portId = 0,
    .rx_hardware_func = NULL,
    .task_transport = NULL,
    .tx_hardware_func = NULL
};

// --------------------------------------------------------------------------

// ============================================================================
// 4.- Screen
// ============================================================================
NX_Screen_t miDispositivoHMI = {
    .transport = &transportDriver,
    .pages = nextion_page_tree,
    .pageCount = sizeof(nextion_page_tree) / sizeof(nextion_page_tree[0]),
    .currentPage = NULL,
    .globalEvent = ScreenGlobalEvent
};


void On_btn_start_Press(NX_Object_t *obj){
    
    const NX_String_t myText = {
      .data = (char *)"RUNNING",
      .length = 8
    };
    
    NX_SetText(&miDispositivoHMI, obj, &myText);
    NX_SetBco(&miDispositivoHMI, obj, 1500);
    printf("\r\nbutton name =%s on-press\r\n", obj->objectName.data);
    
}
void On_btn_stop_Release(NX_Object_t *obj){
    printf("\r\nbutton name =%s release\r\n", obj->objectName.data);
}
void On_g0_Release(NX_Object_t *obj){
    printf("\r\nbutton name =%s releaser\n", obj->objectName.data);
}
void On_p0_Release(NX_Object_t *obj){
    printf("\r\nbutton name =%s release\r\n", obj->objectName.data);
}
void On_bt0_Press(NX_Object_t *obj){
    printf("\r\nbutton name =%s on-press\r\n", obj->objectName.data);
}
void On_bt0_Release(NX_Object_t *obj){
    printf("\r\nbutton name =%s on-release\r\n", obj->objectName.data);
}
