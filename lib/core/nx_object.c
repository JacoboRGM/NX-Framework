/**
 * @file nx_object.c
 * @brief Implementation of runtime object interrogation and metadata abstraction utilities.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include "nx_object.h"

/*============================================================================*
 * Public API Function Implementations
 *============================================================================*/

bool NX_ObjectHasAttribute(const NX_Object_t *object, NX_Attribute_t attribute)
{
    // High-safety defensive guard against uninitialized or incomplete objects
    if (object == NULL || object->component == NULL || attribute >= NX_ATTR_MAX ) {
        return false;
    }

    // Delegate authorization verification to the abstract metaclass component layer
    return NX_ComponentHasAttribute(object->component, attribute);
}

/*----------------------------------------------------------------------------*/

const NX_AttributeInfo_t *NX_ObjectGetAttribute(const NX_Object_t *object, NX_Attribute_t attribute)
{
    // Strict null pointer protection to prevent runtime CPU faults
    if (object == NULL || object->component == NULL || attribute >= NX_ATTR_MAX) {
        return NULL;
    }

    // Retrieve property definitions from the underlying component blueprint
    return NX_ComponentGetAttribute(object->component, attribute);
}

/*----------------------------------------------------------------------------*/

NX_Type_t NX_ObjectType(const NX_Object_t *object)
{
    // Strict defense against incomplete or uninitialized object pointers
    if (object == NULL || object->component == NULL) {
        return NX_TYPE_ERROR;
    }

    return object->component->type;
}

/*----------------------------------------------------------------------------*/

uint8_t NX_ObjectId(const NX_Object_t *object)
{
    if (object == NULL) {
        return 0;
    }

    return object->componentId;
}

/*----------------------------------------------------------------------------*/

const NX_String_t *NX_ObjectName(const NX_Object_t *object)
{
    if (object == NULL) {
        return NULL;
    }

    return &object->objectName;
}

/*----------------------------------------------------------------------------*/

const NX_ComponentClass_t *NX_ObjectComponent(const NX_Object_t *object)
{
    if (object == NULL) {
        return NULL;
    }

    return object->component;
}

/*----------------------------------------------------------------------------*/

const char *NX_ObjectAttributeName(const NX_Object_t *object, NX_Attribute_t attribute)
{
    // Attempt to safely resolve the requested attribute metadata from the object hierarchy
    const NX_AttributeInfo_t *info = NX_ObjectGetAttribute(object, attribute);

    if (info == NULL) {
        return NULL;
    }

    // Return the literal null-terminated string used for serial transmission formatting
    return info->name.data;
}
