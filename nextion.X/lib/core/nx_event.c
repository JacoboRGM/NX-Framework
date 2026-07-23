/**
 * @file nx_event.c
 * @brief Implementation of runtime object interrogation and metadata abstraction utilities.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include "nx_event.h"
#include "nx_types.h"
#include "nx_transport.h"
#include "nx_fifo.h"
#include "nx_screen.h"

NX_Status_t NX_eventDispatch(NX_Screen_t *screen, NX_Vector_t * rxEvent){
    
    if(screen == NULL || rxEvent == NULL)
        return NX_ERROR_NULL_POINTER;
    
    //only process the NX_EVENT_TOUCH for each component
    if ((rxEvent->length == 4) && (rxEvent->data[0] == NX_EVENT_TOUCH)) {

        //valid the object and execute the functions
        //Vector Layout: [0x65] [Page ID] [Component ID] [State: 0=Release, 1=Press]
        uint8_t page_id = rxEvent->data[1];
        uint8_t component_id = rxEvent->data[2];
        uint8_t touch_state = rxEvent->data[3];
        bool is_pressed = (touch_state == 0x01);

        // Locate the target page within the structural layout hierarchy
        NX_Page_t *target_page = NULL;
        for (uint8_t i = 0; i < screen->pageCount; i++) {
            if (screen->pages[i] != NULL && screen->pages[i]->pageId == page_id) {
                target_page = screen->pages[i];
                break;
            }
        }
        if (target_page == NULL) return NX_ERROR_NO_PAGE_FOUND;

        // Interrogate the localized page to find the specific concrete object instance
        NX_Object_t *target_object = (NX_Object_t *) NX_PageFindObjectById(target_page, component_id);
        if (target_object == NULL) return NX_ERROR_OBJECT_NOT_FOUND;

        // AUTOMATIC REDIRECTION: Fire dynamic user-space execution hooks based on state
        if (is_pressed) {
            if (target_object->onPress != NULL) {
                target_object->onPress(target_object);
            }
        } else {
            if (target_object->onRelease != NULL) {
                target_object->onRelease(target_object);
            }
        }
        return NX_OK;
    }
   
    //valid user pointer to user implement
    if(screen->globalEvent == NULL)
        return NX_ERROR_NULL_POINTER;
    
    return screen->globalEvent(rxEvent);
    
}