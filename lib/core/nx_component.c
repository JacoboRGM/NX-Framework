/**
 * @file nx_component.c
 * @brief Database registry implementation for Nextion graphic component classes.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include "nx_component.h"

/*============================================================================*
 * Static Component Definitions (Flash-Resident Blueprints)
 *============================================================================*/

const NX_ComponentClass_t nx_text = {
    .type = NX_TYPE_TEXT,
    .componentName = { .data = "text", .length = 4 },
    .supportedAttributes =
    NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_DRAG)  
};

const NX_ComponentClass_t nx_scroll_text = {
    .type = NX_TYPE_STEXT,
    .componentName = {.data = "s_text", .length = 6},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_DIR)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_EN)    
    | NX_ATTR_BIT(NX_ATTR_TIM)
    | NX_ATTR_BIT(NX_ATTR_TXT)  
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
    
};

const NX_ComponentClass_t nx_number = {
    .type = NX_TYPE_NUMBER,
    .componentName = { .data = "number", .length = 6},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_LENGTH)
    | NX_ATTR_BIT(NX_ATTR_FORMAT)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

const NX_ComponentClass_t nx_xfloat = {

    .type = NX_TYPE_XFLOAT,
    .componentName = {.data = "XFLOAT", .length = 6},
    .supportedAttributes =
     NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_WS0)
    | NX_ATTR_BIT(NX_ATTR_WS1)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)        
};

const NX_ComponentClass_t nx_button = {
    .type = NX_TYPE_BUTTON,
    .componentName = { .data = "button", .length = 6 },
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

const NX_ComponentClass_t nx_progress_bar ={
    .type = NX_TYPE_PROGRESSBAR,
    .componentName = {.data = "progress bar", .length=11},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_DIR)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)       
};

const NX_ComponentClass_t nx_picture ={
    .type = NX_TYPE_PICTURE,
    .componentName = {.data = "picture",.length = 7},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_PIC)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)  
};

const NX_ComponentClass_t nx_crop ={
    .type = NX_TYPE_CROP,
    .componentName = {.data = "crop",.length = 4},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_PICC)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)  
};

const NX_ComponentClass_t nx_hotspot ={
    .type = NX_TYPE_HOTSPOT,
    .componentName = {.data = "hotspot",.length = 7},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)  
};

const NX_ComponentClass_t nx_touch_cap ={
    .type = NX_TYPE_TOUCH_CAP,
    .componentName = {.data = "touch cap",.length = 7},
    .supportedAttributes = 0x00
};

const NX_ComponentClass_t nx_gauge = {
    .type = NX_TYPE_GAUGE,
    .componentName = {.data = "gauge", .length=5},
    .supportedAttributes =
     NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_FORMAT)
    | NX_ATTR_BIT(NX_ATTR_UP)
    | NX_ATTR_BIT(NX_ATTR_DOWN)
    | NX_ATTR_BIT(NX_ATTR_LEFT)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_HIG)
    | NX_ATTR_BIT(NX_ATTR_WS0)
    | NX_ATTR_BIT(NX_ATTR_WS1)
    | NX_ATTR_BIT(NX_ATTR_WS2)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};

const NX_ComponentClass_t nx_waveform = {
    .type = NX_TYPE_WAVEFORM,
    .componentName = {.data = "waveform", .length=8},
    .supportedAttributes =
     NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_GDC)
    | NX_ATTR_BIT(NX_ATTR_GDW)
    | NX_ATTR_BIT(NX_ATTR_GDH)
    | NX_ATTR_BIT(NX_ATTR_PCO0)
    | NX_ATTR_BIT(NX_ATTR_PCO1)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO3)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
    
};

const NX_ComponentClass_t nx_slider ={
    .type = NX_TYPE_SLIDER,
    .componentName = {.data = "slider", .length=6},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_WID)
    | NX_ATTR_BIT(NX_ATTR_HIG)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_BCO1)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_MAXVAL)
    | NX_ATTR_BIT(NX_ATTR_MINVAL)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)       
};

const NX_ComponentClass_t nx_timer ={
    .type = NX_TYPE_TIMER,
    .componentName = {.data = "timer",.length = 5},
    .supportedAttributes =
     NX_ATTR_BIT(NX_ATTR_TIM)
    | NX_ATTR_BIT(NX_ATTR_EN)
};

const NX_ComponentClass_t nx_variable ={
    .type = NX_TYPE_VARIABLE,
    .componentName = {.data = "variable",.length = 8},
    .supportedAttributes =
     NX_ATTR_BIT(NX_ATTR_VAL)
};

const NX_ComponentClass_t nx_dual_button ={
    .type = NX_TYPE_DUALSTATE,
    .componentName = {.data = "dual button", .length=11},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)   
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

const NX_ComponentClass_t nx_check_box = {
    .type = NX_TYPE_CHECKBOX,
    .componentName = {.data = "check box", .length = 9},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_BCO) 
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)       
};

const NX_ComponentClass_t nx_qr_code = {
    .type = NX_TYPE_QRCODE,
    .componentName = {.data = "qr code", .length = 7},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

const NX_ComponentClass_t nx_switch = {
    .type = NX_TYPE_SWITCH,
    .componentName = {.data = "switch", .length = 6},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO1)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

//gmov
const NX_ComponentClass_t nx_gmov = {
    .type = NX_TYPE_GMOV,
    .componentName = {.data= "gmov", .length = 4},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_VID)
    | NX_ATTR_BIT(NX_ATTR_EN)
    | NX_ATTR_BIT(NX_ATTR_LOOP)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_TIM)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

//combo box

const NX_ComponentClass_t nx_combo_box = {
    .type = NX_TYPE_COMBO_BOX,
    .componentName = {.data = "combo box", .length =9},
    .supportedAttributes =
    NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_YCEN)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_UP)
    | NX_ATTR_BIT(NX_ATTR_PCO3)
    | NX_ATTR_BIT(NX_ATTR_BCO1)
    | NX_ATTR_BIT(NX_ATTR_PCO1)
    | NX_ATTR_BIT(NX_ATTR_PATH)
    | NX_ATTR_BIT(NX_ATTR_DIR)
    | NX_ATTR_BIT(NX_ATTR_QTY)
    | NX_ATTR_BIT(NX_ATTR_WS0)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_HIG)
    | NX_ATTR_BIT(NX_ATTR_DOWN)
    | NX_ATTR_BIT(NX_ATTR_MODE)
    | NX_ATTR_BIT(NX_ATTR_WID)
    | NX_ATTR_BIT(NX_ATTR_WS1)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};
//text select
const NX_ComponentClass_t nx_select_text ={
    .type  = NX_TYPE_SELECT_TEXT,
    .componentName = {.data ="select text", .length=11},
    .supportedAttributes = 
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_SPAX)        
    | NX_ATTR_BIT(NX_ATTR_HIG)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_PCO1)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_PATH)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)            
            
};

//SLText

const NX_ComponentClass_t nx_sltext = {
    .type = NX_TYPE_SLTEXT,
    .componentName = {.data = "SLText", .length = 6},
    .supportedAttributes = 
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_LEFT)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_TXT)
    | NX_ATTR_BIT(NX_ATTR_ISBR)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_VAL_Y)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};

//radio button
const NX_ComponentClass_t nx_radio_button = {
    .type = NX_TYPE_RADIO,
    .componentName = {.data = "radio", .length = 5},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

//data record
const NX_ComponentClass_t nx_data_record = {
    .type = NX_TYPE_DATA_RECORD,
    .componentName = {.data = "data record", .length=12},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_XCEN)
    | NX_ATTR_BIT(NX_ATTR_PATH)
    | NX_ATTR_BIT(NX_ATTR_FORMAT)
    | NX_ATTR_BIT(NX_ATTR_DIR)
    | NX_ATTR_BIT(NX_ATTR_MODE)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_ORDER)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_HIG)
    | NX_ATTR_BIT(NX_ATTR_LEFT)
    | NX_ATTR_BIT(NX_ATTR_GDC)
    | NX_ATTR_BIT(NX_ATTR_GDW)
    | NX_ATTR_BIT(NX_ATTR_GDH)
    | NX_ATTR_BIT(NX_ATTR_BCO1)
    | NX_ATTR_BIT(NX_ATTR_PCO1)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_VAL_Y)
    | NX_ATTR_BIT(NX_ATTR_VAL_X)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};

//file browser

const NX_ComponentClass_t nx_file_browser ={
    .type = NX_TYPE_FILE_BROWSER,
    .componentName = {.data = "file browser", .length = 12},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_FONT)
    | NX_ATTR_BIT(NX_ATTR_BCO)
    | NX_ATTR_BIT(NX_ATTR_PCO)
    | NX_ATTR_BIT(NX_ATTR_PCO2)
    | NX_ATTR_BIT(NX_ATTR_BCO2)
    | NX_ATTR_BIT(NX_ATTR_LEFT)
    | NX_ATTR_BIT(NX_ATTR_CH)
    | NX_ATTR_BIT(NX_ATTR_DIR)
    | NX_ATTR_BIT(NX_ATTR_FILTER)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_SPAX)
    | NX_ATTR_BIT(NX_ATTR_SPAY)
    | NX_ATTR_BIT(NX_ATTR_PIC1)
    | NX_ATTR_BIT(NX_ATTR_PIC2)
    | NX_ATTR_BIT(NX_ATTR_WS2)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
};

//video

const NX_ComponentClass_t nx_video = {
    .type = NX_TYPE_VIDEO,
    .componentName = {.data = "video", .length = 5},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_VID)
    | NX_ATTR_BIT(NX_ATTR_EN)
    | NX_ATTR_BIT(NX_ATTR_LOOP)
    | NX_ATTR_BIT(NX_ATTR_DIS)
    | NX_ATTR_BIT(NX_ATTR_TIM)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};

//external picture
const NX_ComponentClass_t nx_ex_picture = {
    .type = NX_TYPE_EX_PICTURE,
    .componentName = {.data = "ex picture", .length = 10},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_DRAG)
    | NX_ATTR_BIT(NX_ATTR_APH)
    | NX_ATTR_BIT(NX_ATTR_EFFECT)
    | NX_ATTR_BIT(NX_ATTR_PATH)
    | NX_ATTR_BIT(NX_ATTR_X)
    | NX_ATTR_BIT(NX_ATTR_Y)
            
};

const NX_ComponentClass_t nx_file_stream = {
    .type = NX_TYPE_FILE_STREAM,
    .componentName = {.data = "file stream", .length = 11},
    .supportedAttributes =
            NX_ATTR_BIT(NX_ATTR_VAL)
};

const NX_ComponentClass_t nx_audio = {
    .type = NX_TYPE_AUDIO,
    .componentName = {.data = "audio", .length = 5},
    .supportedAttributes =
      NX_ATTR_BIT(NX_ATTR_VID)
    | NX_ATTR_BIT(NX_ATTR_VAL)
    | NX_ATTR_BIT(NX_ATTR_EN)
    | NX_ATTR_BIT(NX_ATTR_LOOP)
    | NX_ATTR_BIT(NX_ATTR_TIM)
            
};

/*============================================================================*
 * Component Internal Database (O(1) Map)
 *============================================================================*/

/**
 * @brief Static database linking component type codes to their class definitions.
 * @note Marked 'const * const' to prevent pointers or table structure overrides at runtime.
 */
static const NX_ComponentClass_t * const NX_DB[NX_TYPE_MAX] =
{

    [NX_TYPE_TEXT] = &nx_text,
    [NX_TYPE_STEXT] = &nx_scroll_text,
    [NX_TYPE_NUMBER] = &nx_number,
    [NX_TYPE_XFLOAT] = &nx_xfloat,
    [NX_TYPE_BUTTON] = &nx_button,
    [NX_TYPE_PROGRESSBAR] = &nx_progress_bar,
    [NX_TYPE_PICTURE] = &nx_picture,
    [NX_TYPE_CROP] = &nx_crop,
    [NX_TYPE_HOTSPOT] = &nx_hotspot,
    [NX_TYPE_TOUCH_CAP] = &nx_touch_cap,
    [NX_TYPE_GAUGE] = &nx_gauge,
    [NX_TYPE_WAVEFORM] = &nx_waveform,
    [NX_TYPE_SLIDER] = &nx_slider,
    [NX_TYPE_TIMER] = &nx_timer,
    [NX_TYPE_VARIABLE] = &nx_variable,
    [NX_TYPE_DUALSTATE] = &nx_dual_button,
    [NX_TYPE_CHECKBOX] = &nx_check_box,
    [NX_TYPE_QRCODE] = &nx_qr_code,
    [NX_TYPE_SWITCH] = &nx_switch,
    [NX_TYPE_GMOV] = &nx_gmov,
    [NX_TYPE_COMBO_BOX] = &nx_combo_box,
    [NX_TYPE_SELECT_TEXT] = &nx_select_text,
    [NX_TYPE_SLTEXT] = &nx_sltext,
    [NX_TYPE_RADIO] = &nx_radio_button,
    [NX_TYPE_DATA_RECORD] = &nx_data_record,
    [NX_TYPE_FILE_BROWSER] = &nx_file_browser,
    [NX_TYPE_VIDEO] = &nx_video,
    [NX_TYPE_EX_PICTURE] = &nx_ex_picture,
    [NX_TYPE_FILE_STREAM] = &nx_file_stream,
    [NX_TYPE_AUDIO] = &nx_audio
    
};



/*============================================================================*
 * API Function Implementations
 *============================================================================*/

const NX_ComponentClass_t *NX_GetComponentClass(NX_Type_t type)
{
    // High-safety defensive boundary guard
    if (type >= NX_TYPE_MAX) {
        return NULL;
    }

    // Defensive check: Returns pointer only if the database slot has been initialized
    return NX_DB[type];
}

bool NX_ComponentHasAttribute(const NX_ComponentClass_t *component, NX_Attribute_t attribute)
{
    // Strict null pointer protection
    if (component == NULL || attribute >= NX_ATTR_MAX) {
        return false;
    }

    // Perform O(1) bit-level capability verification
    return (component->supportedAttributes & NX_ATTR_BIT(attribute)) != UINT64_C(0);
}

const NX_AttributeInfo_t *NX_ComponentGetAttribute(const NX_ComponentClass_t *component, NX_Attribute_t attribute)
{
    // Rely on safe bitmask validation before returning attribute metadata
    if (!NX_ComponentHasAttribute(component, attribute)) {
        return NULL;
    }

    return NX_GetAttributeInfo(attribute);
}
