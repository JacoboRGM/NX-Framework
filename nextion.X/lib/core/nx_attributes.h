/**
 * @file nx_attributes.h
 * @brief Nextion attribute identifiers and bit-mask translation macros.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 */

#ifndef NX_ATTRIBUTES_H
#define NX_ATTRIBUTES_H

#include <stddef.h>
#include "nx_types.h"

#define NX_ATTR_BIT(x) (UINT64_C(1) << (x))

typedef enum {
    NX_ATTR_DRAG = 0, NX_ATTR_APH, NX_ATTR_EFFECT, NX_ATTR_FONT,
    NX_ATTR_BCO, NX_ATTR_PCO, NX_ATTR_PCO1, NX_ATTR_BCO2, NX_ATTR_PCO2,
    NX_ATTR_PCO3, NX_ATTR_PCO0, NX_ATTR_BCO1,
    NX_ATTR_XCEN, NX_ATTR_YCEN, NX_ATTR_PW, NX_ATTR_TXT,
    NX_ATTR_ISBR, NX_ATTR_SPAX, NX_ATTR_SPAY, NX_ATTR_X, NX_ATTR_Y,
    
    // Scrolling
    NX_ATTR_DIR, NX_ATTR_DIS, NX_ATTR_TIM, NX_ATTR_EN,
            
    // Numbers & Gauges
    NX_ATTR_VAL, NX_ATTR_LENGTH, NX_ATTR_FORMAT,
    NX_ATTR_WS0, NX_ATTR_WS1, NX_ATTR_WS2,
    
    // Bounds
    NX_ATTR_WID, NX_ATTR_HIG, NX_ATTR_MAXVAL, NX_ATTR_MINVAL, NX_ATTR_CH,

    // Resources
    NX_ATTR_PIC, NX_ATTR_PICC, NX_ATTR_UP, NX_ATTR_DOWN, NX_ATTR_LEFT,

    // Advanced Graphic blocks
    NX_ATTR_GDC, NX_ATTR_GDW, NX_ATTR_GDH, NX_ATTR_LOOP, NX_ATTR_VID,
    //Visibility
    NX_ATTR_VIS, NX_ATTR_TSW,
            
   //combo box, SLText, select text
    NX_ATTR_PATH, NX_ATTR_QTY, NX_ATTR_MODE, NX_ATTR_VAL_Y, NX_ATTR_ORDER,
    NX_ATTR_VAL_X,NX_ATTR_FILTER, NX_ATTR_PIC1, NX_ATTR_PIC2,

    NX_ATTR_MAX 
} NX_Attribute_t;

typedef struct {
    NX_Attribute_t attribute;
    NX_String_t name;           
} NX_AttributeInfo_t;

const NX_AttributeInfo_t *NX_GetAttributeInfo(NX_Attribute_t attribute);

#endif /* NX_ATTRIBUTES_H */
