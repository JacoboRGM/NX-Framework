/**
 * @file nx_component.h
 * @brief Nextion graphic component class abstraction and validation registry.
 * @note Models component capabilities using 64-bit bitmasks for high-performance validation.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef NX_COMPONENT_H
#define NX_COMPONENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nx_types.h"
#include "nx_attributes.h"

/**
 * @brief Bitmask representation where each bit index corresponds to an NX_Attribute_t value.
 */
typedef uint64_t NX_AttributeMask_t;

/*============================================================================*
 * Component Class Object Model
 *============================================================================*/

/**
 * @brief Metaclass definition containing the blueprint and capabilities of a component.
 */
typedef struct
{
    NX_Type_t type;                         // Unique component type identifier code
    NX_String_t componentName;               // Literal system text representation
    NX_AttributeMask_t supportedAttributes;// Bitwise flags indicating valid attributes
} NX_ComponentClass_t;

/* Global Immutable Metaclass Blueprints visible for compile-time structural linking */
extern const NX_ComponentClass_t nx_text;
extern const NX_ComponentClass_t nx_scroll_text;
extern const NX_ComponentClass_t nx_number;
extern const NX_ComponentClass_t nx_xfloat;
extern const NX_ComponentClass_t nx_button;
extern const NX_ComponentClass_t nx_progress_bar;
extern const NX_ComponentClass_t nx_picture;
extern const NX_ComponentClass_t nx_crop;
extern const NX_ComponentClass_t nx_hotspot;
extern const NX_ComponentClass_t nx_touch_cap;
extern const NX_ComponentClass_t nx_gauge;
extern const NX_ComponentClass_t nx_waveform;
extern const NX_ComponentClass_t nx_slider;
extern const NX_ComponentClass_t nx_timer;
extern const NX_ComponentClass_t nx_variable;
extern const NX_ComponentClass_t nx_dual_button;
extern const NX_ComponentClass_t nx_check_box;
extern const NX_ComponentClass_t nx_qr_code;
extern const NX_ComponentClass_t nx_switch;
extern const NX_ComponentClass_t nx_gmov;
extern const NX_ComponentClass_t nx_combo_box;
extern const NX_ComponentClass_t nx_select_text;
extern const NX_ComponentClass_t nx_sltext;
extern const NX_ComponentClass_t nx_radio_button;
extern const NX_ComponentClass_t nx_data_record;
extern const NX_ComponentClass_t nx_file_browser;
extern const NX_ComponentClass_t nx_video;
extern const NX_ComponentClass_t nx_ex_picture;
extern const NX_ComponentClass_t nx_file_stream;
extern const NX_ComponentClass_t nx_audio;

/*============================================================================*
 * Component Class API Layer
 *============================================================================*/

/**
 * @brief Retrieves the immutable class blueprint for a given component type from the database.
 * @param[in] type The target component type to query.
 * @return Pointer to the immutable class definition, or NULL if type is unsupported/invalid.
 */
const NX_ComponentClass_t *NX_GetComponentClass(NX_Type_t type);

/**
 * @brief Checks if a component class blueprint supports a specific attribute.
 * @param[in] component Pointer to the component class definition to evaluate.
 * @param[in] attribute The attribute token to validate against the bitmask.
 * @return true if the attribute is supported, false otherwise.
 */
bool NX_ComponentHasAttribute(const NX_ComponentClass_t *component, NX_Attribute_t attribute);

/**
 * @brief Resolves and fetches full metadata for an attribute if the component supports it.
 * @param[in] component Pointer to the component class definition.
 * @param[in] attribute The target attribute token to resolve.
 * @return Pointer to the static NX_AttributeInfo_t structure, or NULL if unauthorized.
 */
const NX_AttributeInfo_t *NX_ComponentGetAttribute(const NX_ComponentClass_t *component, NX_Attribute_t attribute);

#ifdef __cplusplus
}
#endif

#endif /* NX_COMPONENT_H */
