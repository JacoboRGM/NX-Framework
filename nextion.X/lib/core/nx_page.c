/**
 * @file nx_page.c
 * @brief Implementation of page container querying and object lookup utilities.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include <string.h>
#include "nx_page.h"

const NX_Object_t *NX_PageFindObjectById(const NX_Page_t *page, uint8_t componentId)
{
    // Defensive check to avoid null pointer dereferencing or uninitialized layouts
    if (page == NULL || page->objects == NULL) {
        return NULL;
    }

    // Traverse the array of pointers to look for the matching hardware ID index
    for (uint8_t i = 0; i < page->objectCount; i++) {
        if (page->objects[i] != NULL && page->objects[i]->componentId == componentId) {
            return page->objects[i]; // Concrete object instance found
        }
    }

    return NULL; // Object is not registered within this specific layout context
}

/*----------------------------------------------------------------------------*/

const NX_Object_t *NX_PageFindObjectByName(const NX_Page_t *page, const char *name)
{
    if (page == NULL || page->objects == NULL || name == NULL) {
        return NULL;
    }

    // Query size extraction to ensure deterministic bounded string operations
    uint8_t query_len = (uint8_t)strlen(name);

    // Scan layout hierarchy looking for explicit length and alphanumeric matches
    for (uint8_t i = 0; i < page->objectCount; i++) {
        if (page->objects[i] != NULL && page->objects[i]->objectName.data != NULL) {
            // Compare safe metadata lengths first to achieve high execution speeds
            if (page->objects[i]->objectName.length == query_len) {
                if (strncmp(page->objects[i]->objectName.data, name, query_len) == 0) {
                    return page->objects[i];
                }
            }
        }
    }

    return NULL;
}

bool NX_PageContainsObject(const NX_Page_t *page, const NX_Object_t *object) {

    if ((page == NULL) || (object == NULL) || (page->objects == NULL))
        return false;

    for (uint8_t i = 0; i < page->objectCount; i++) {
        if (page->objects[i] == object) {
            return true;
        }
    }
    return false;
}