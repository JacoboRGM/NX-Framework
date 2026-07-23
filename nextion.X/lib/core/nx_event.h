/**
 * @file nx_event.h
 * @brief Centralized application programming interface (API) for Nextion C Driver.
 * @note Implements high-level abstraction setters for all component types, geometry,
 *       style, and device routing tasks using a unified NX_Screen_t container.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef NX_EVENT_H
#define	NX_EVENT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "nx_types.h"
    

    
NX_Status_t NX_eventDispatch(NX_Screen_t *screen, NX_Vector_t * rxEvent);




#ifdef	__cplusplus
}
#endif

#endif	/* NX_EVENT_H */

