/**
 * @file nx_attributes.c
 * @brief Immutable metadata lookup table for Nextion serial command attributes.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include "nx_attributes.h"

/**
 * @brief Internal lookup table mapping attributes to raw protocol strings.
 * @note Declared static const to enforce strict file-scope encapsulation and read-only flash storage.
 */
static const NX_AttributeInfo_t nx_attribute_table[NX_ATTR_MAX] = {
    [NX_ATTR_DRAG]   = { .attribute = NX_ATTR_DRAG,   .name = { .data = "drag",   .length = 4 } },
    [NX_ATTR_APH]    = { .attribute = NX_ATTR_APH,    .name = { .data = "aph",    .length = 3 } },
    [NX_ATTR_EFFECT] = { .attribute = NX_ATTR_EFFECT, .name = { .data = "effect", .length = 6 } },
    [NX_ATTR_FONT]   = { .attribute = NX_ATTR_FONT,   .name = { .data = "font",   .length = 4 } },
    [NX_ATTR_BCO]    = { .attribute = NX_ATTR_BCO,    .name = { .data = "bco",    .length = 3 } },
    [NX_ATTR_PCO]    = { .attribute = NX_ATTR_PCO,    .name = { .data = "pco",    .length = 3 } },
    [NX_ATTR_BCO1]   = { .attribute = NX_ATTR_BCO1,   .name = { .data = "bco1",   .length = 4 } }, 
    [NX_ATTR_BCO2]   = { .attribute = NX_ATTR_BCO2,   .name = { .data = "bco2",   .length = 4 } },
    [NX_ATTR_PCO0]   = { .attribute = NX_ATTR_PCO0,   .name = { .data = "pco0",   .length = 4 } }, 
    [NX_ATTR_PCO1]   = { .attribute = NX_ATTR_PCO1,   .name = { .data = "pco1",   .length = 4 } }, 
    [NX_ATTR_PCO2]   = { .attribute = NX_ATTR_PCO2,   .name = { .data = "pco2",   .length = 4 } },
    [NX_ATTR_PCO3]   = { .attribute = NX_ATTR_PCO3,   .name = { .data = "pco3",   .length = 4 } }, 
    [NX_ATTR_XCEN]   = { .attribute = NX_ATTR_XCEN,   .name = { .data = "xcen",   .length = 4 } },
    [NX_ATTR_YCEN]   = { .attribute = NX_ATTR_YCEN,   .name = { .data = "ycen",   .length = 4 } },
    [NX_ATTR_PW]     = { .attribute = NX_ATTR_PW,     .name = { .data = "pw",     .length = 2 } },
    [NX_ATTR_TXT]    = { .attribute = NX_ATTR_TXT,    .name = { .data = "txt",    .length = 3 } },
    [NX_ATTR_ISBR]   = { .attribute = NX_ATTR_ISBR,   .name = { .data = "isbr",   .length = 4 } },
    [NX_ATTR_SPAX]   = { .attribute = NX_ATTR_SPAX,   .name = { .data = "spax",   .length = 4 } },
    [NX_ATTR_SPAY]   = { .attribute = NX_ATTR_SPAY,   .name = { .data = "spay",   .length = 4 } },
    [NX_ATTR_X]      = { .attribute = NX_ATTR_X,      .name = { .data = "x",      .length = 1 } },
    [NX_ATTR_Y]      = { .attribute = NX_ATTR_Y,      .name = { .data = "y",      .length = 1 } },

    // --- Scrolling Text Only ---
    [NX_ATTR_DIR]    = { .attribute = NX_ATTR_DIR,    .name = { .data = "dir",    .length = 3 } },
    [NX_ATTR_DIS]    = { .attribute = NX_ATTR_DIS,    .name = { .data = "dis",    .length = 3 } },
    [NX_ATTR_TIM]    = { .attribute = NX_ATTR_TIM,    .name = { .data = "tim",    .length = 3 } },
    [NX_ATTR_EN]     = { .attribute = NX_ATTR_EN,     .name = { .data = "en",     .length = 2 } },

    // --- Number Types ---
    [NX_ATTR_VAL]    = { .attribute = NX_ATTR_VAL,    .name = { .data = "val",    .length = 3 } },
    [NX_ATTR_LENGTH] = { .attribute = NX_ATTR_LENGTH, .name = { .data = "length", .length = 6 } },
    [NX_ATTR_FORMAT] = { .attribute = NX_ATTR_FORMAT, .name = { .data = "format", .length = 6 } },
    [NX_ATTR_WS0]    = { .attribute = NX_ATTR_WS0,    .name = { .data = "ws0",    .length = 3 } },
    [NX_ATTR_WS1]    = { .attribute = NX_ATTR_WS1,    .name = { .data = "ws1",    .length = 3 } },
    [NX_ATTR_WS2]    = { .attribute = NX_ATTR_WS2,    .name = { .data = "ws2",    .length = 3 } },

    // --- Layout & Constraints ---
    [NX_ATTR_WID]    = { .attribute = NX_ATTR_WID,    .name = { .data = "wid",    .length = 3 } },
    [NX_ATTR_HIG]    = { .attribute = NX_ATTR_HIG,    .name = { .data = "hig",    .length = 3 } },
    [NX_ATTR_MAXVAL] = { .attribute = NX_ATTR_MAXVAL, .name = { .data = "maxval", .length = 6 } },
    [NX_ATTR_MINVAL] = { .attribute = NX_ATTR_MINVAL, .name = { .data = "minval", .length = 6 } },
    [NX_ATTR_CH]     = { .attribute = NX_ATTR_CH,     .name = { .data = "ch",     .length = 2 } },

    // --- Images ---
    [NX_ATTR_PIC]    = { .attribute = NX_ATTR_PIC,    .name = { .data = "pic",    .length = 3 } },
    [NX_ATTR_PICC]   = { .attribute = NX_ATTR_PICC,   .name = { .data = "picc",   .length = 4 } }, // FIXME: verificar (crop, imagen de crop)
    [NX_ATTR_UP]     = { .attribute = NX_ATTR_UP,     .name = { .data = "up",     .length = 2 } },
    [NX_ATTR_DOWN]   = { .attribute = NX_ATTR_DOWN,   .name = { .data = "down",   .length = 4 } },
    [NX_ATTR_LEFT]   = { .attribute = NX_ATTR_LEFT,   .name = { .data = "left",   .length = 4 } },

    // --- Waveform ---
    [NX_ATTR_GDC]    = { .attribute = NX_ATTR_GDC,    .name = { .data = "gdc",    .length = 3 } },
    [NX_ATTR_GDW]    = { .attribute = NX_ATTR_GDW,    .name = { .data = "gdw",    .length = 3 } },
    [NX_ATTR_GDH]    = { .attribute = NX_ATTR_GDH,    .name = { .data = "gdh",    .length = 3 } },

    // --- GMOV ---
    [NX_ATTR_LOOP]   = { .attribute = NX_ATTR_LOOP,   .name = { .data = "loop",   .length = 4 } },
    [NX_ATTR_VID]    = { .attribute = NX_ATTR_VID,    .name = { .data = "vid",    .length = 3 } },
    //---Visibility--
    [NX_ATTR_VIS]    = { .attribute = NX_ATTR_VIS,    .name = { .data = "vis",    .length = 3 } },
    [NX_ATTR_TSW]    = { .attribute = NX_ATTR_TSW,    .name = { .data = "tsw",    .length = 3 } },

    // --- Combo box, SLText, select text, data record ---
    [NX_ATTR_PATH]   = { .attribute = NX_ATTR_PATH,   .name = { .data = "path",   .length = 4 } },  
    [NX_ATTR_QTY]    = { .attribute = NX_ATTR_QTY,    .name = { .data = "qty",    .length = 3 } },  
    [NX_ATTR_MODE]   = { .attribute = NX_ATTR_MODE,   .name = { .data = "mode",   .length = 4 } }, 
    [NX_ATTR_VAL_Y]  = { .attribute = NX_ATTR_VAL_Y,  .name = { .data = "val_y",  .length = 5 } },  
    [NX_ATTR_ORDER]  = { .attribute = NX_ATTR_ORDER,  .name = { .data = "order",  .length = 5 } },  
    [NX_ATTR_VAL_X]  = { .attribute = NX_ATTR_VAL_X,  .name = { .data = "val_x",  .length = 5 } },  
    [NX_ATTR_FILTER] = { .attribute = NX_ATTR_FILTER, .name = { .data = "filter", .length = 6 } },  
    [NX_ATTR_PIC1]   = { .attribute = NX_ATTR_PIC1,   .name = { .data = "pic1",   .length = 4 } },  
    [NX_ATTR_PIC2]   = { .attribute = NX_ATTR_PIC2,   .name = { .data = "pic2",   .length = 4 } }   
};

const NX_AttributeInfo_t *NX_GetAttributeInfo(NX_Attribute_t attribute)
{
    // High-safety defensive boundary guard
    if (attribute >= NX_ATTR_MAX) {
        return NULL;
    }

    return &nx_attribute_table[attribute];
}
