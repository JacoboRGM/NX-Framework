/**
 * @file nx_page.h
 * @brief Nextion page container abstraction layer.
 * @note Groups concrete object instances belonging to the same screen layout context.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef NX_PAGE_H
#define NX_PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nx_types.h"
#include "nx_object.h"

/**
 * @brief Page Instance structure.
 * Models a full screen layout on the Nextion display.
 */
struct NX_Page
{
    uint8_t pageId;                         // The hardware page index defined in Nextion Editor
    NX_String_t pageName;                     // Literal alphanumeric string name of the page
    NX_Object_t **objects;                  // Array of pointers to objects inside this page
    uint8_t objectCount;                    // Total number of objects registered in this page
    // --- Dynamic Page Lifecycle Hooks ---
    NX_PageCallback_t onInit;               // Optional callback triggered when the page loads
};

/*============================================================================*
 * Page Instance API Layer
 *============================================================================*/

/**
 * @brief Searches and retrieves a registered object inside a page by its component ID.
 * @param[in] page Pointer to the target page instance.
 * @param[in] componentId The hardware ID index of the component to find.
 * @return Pointer to the live NX_Object_t instance, or NULL if not found/invalid.
 */
const NX_Object_t *NX_PageFindObjectById(const NX_Page_t *page, uint8_t componentId);

/**
 * @brief Searches and retrieves a registered object inside a page by its string name.
 * @param[in] page Pointer to the target page instance.
 * @param[in] name The string name of the object to search for.
 * @return Pointer to the live NX_Object_t instance, or NULL if not found/invalid.
 */
const NX_Object_t *NX_PageFindObjectByName(const NX_Page_t *page, const char *name);

/**
 * @brief Searches and retrieves a registered object inside a page by NX_Object_t type.
 * @param[in] page Pointer to the target page instance.
 * @param[in] object Typedef to search for.
 * @return [out] true/false if the object are inside of page
 */
bool NX_PageContainsObject(const NX_Page_t *page, const NX_Object_t *object);

#ifdef __cplusplus
}
#endif

#endif /* NX_PAGE_H */
