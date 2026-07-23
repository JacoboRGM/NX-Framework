/**
 * @file nx_screen.c
 * @brief Implementation of master screen device lifecycle and asymmetric event routing.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include "nx_screen.h"

bool NX_ScreenInit(NX_Screen_t *screen, NX_Page_t **pages, uint8_t pageCount, NX_PortComm_ID_t port)
{
    // Defensive validation of structural initialization constraints
    if (screen == NULL || pages == NULL || pageCount == 0) {
        return false;
    }

    // Bind immutable topology layouts
    screen->pages = pages;
    screen->pageCount = pageCount;
    screen->currentPage = NULL; // Unassigned till screen broadcasts startup synchronization over UART

    //check global event
    if(screen->globalEvent == NULL)
        return false;
    return true;
}

/*----------------------------------------------------------------------------*/

void NX_ScreenSetActivePage(NX_Screen_t *screen, uint8_t pageId)
{
    if (screen == NULL || screen->pages == NULL) {
        return;
    }

    // Interrogate the page list array to update the state machine active tracking reference
    for (uint8_t i = 0; i < screen->pageCount; i++) {
        if (screen->pages[i] != NULL && screen->pages[i]->pageId == pageId) {
            screen->currentPage = screen->pages[i]; 
            return;
        }
    }
}

/*----------------------------------------------------------------------------*/

