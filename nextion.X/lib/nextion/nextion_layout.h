// ============================================================================
// GENERATED AUTOMATICALLY BY NEXTION PARSER ENGINE - HEADER INTERFACE
// DO NOT MODIFY MANUALLY
// ============================================================================

#ifndef NEXTION_LAYOUT_H_
#define NEXTION_LAYOUT_H_

#include <stddef.h>
#include "nextion_layout.h"
#include "../core/nx_types.h"
#include "../core/nx_component.h"


// ============================================================================
// Page Index Enumeration
// ============================================================================
typedef enum {
    PAGE_DASHBOARD_PAGE1,
    PAGE_DASHBOARD_PAGE2
} NX_PageId_e;

// ============================================================================
// Component Widgets Event Callbacks Prototypes
// ============================================================================
void On_btn_start_Press(NX_Object_t *obj);
void On_btn_stop_Release(NX_Object_t *obj);
void On_g0_Release(NX_Object_t *obj);
void On_p0_Release(NX_Object_t *obj);
void On_bt0_Press(NX_Object_t *obj);
void On_bt0_Release(NX_Object_t *obj);

// ============================================================================
// Public Component Objects Instances
// ============================================================================
extern NX_Object_t btn_start;
extern NX_Object_t t0;
extern NX_Object_t btn_stop;
extern NX_Object_t t1;
extern NX_Object_t number0;
extern NX_Object_t x0;
extern NX_Object_t g0;
extern NX_Object_t nx_j0;
extern NX_Object_t p0;
extern NX_Object_t q0;
extern NX_Object_t m0;
extern NX_Object_t tc0;
extern NX_Object_t z0;
extern NX_Object_t s0;
extern NX_Object_t h0;
extern NX_Object_t tm0;
extern NX_Object_t va0;
extern NX_Object_t bt0;
extern NX_Object_t c0;
extern NX_Object_t qr0;
extern NX_Object_t sw0;
extern NX_Object_t gm0;
extern NX_Object_t cb0;
extern NX_Object_t select0;
extern NX_Object_t slt0;
extern NX_Object_t r0;
extern NX_Object_t data0;
extern NX_Object_t fbrowser0;
extern NX_Object_t fs0;
extern NX_Object_t v0;
extern NX_Object_t wav0;
extern NX_Object_t exp0;

extern NX_Page_t page_dashboard;
extern NX_Page_t page_page0;

// ============================================================================
// Global Event System
// ============================================================================
NX_Status_t ScreenGlobalEvent(NX_Vector_t *unhandled_vector);

// ============================================================================
// Public HMI Device Instance
// ============================================================================
extern NX_Screen_t miDispositivoHMI;

#endif // NEXTION_LAYOUT_H_