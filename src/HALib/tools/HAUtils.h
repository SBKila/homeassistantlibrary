#pragma once

#include <Arduino.h>
// Include HAComponentProperty because it is used in LinkedList templates
#include "../protocol/components/HAComponentProperty.h"
#include "../protocol/components/HAComponentType.h"
#include "../tools/LinkedList.hpp"
#include "debug.h"

namespace HALIB_NAMESPACE
{
    class HAUtils
    {
    public:
        // Converts a list of properties into a JSON string object
        static char *propertyListToJson(LinkedList<HAComponentProperty *> &pProperties);

        // Frees memory for all properties in the list and clears the list
        static void deleteProperties(LinkedList<HAComponentProperty *> &pProperties);

        // Retrieves the value of a property by its key
        static const char *getProperty(LinkedList<HAComponentProperty *> &pProperties, HAComponentPropertyKey name);

        // Generates a unique ID based on the name and component type
        static uint32_t generateId(const char *pName, ComponentType pType);

        // Duplicates a string from Flash (PROGMEM) to RAM (malloc + strcpy_P)
        static char *strdup_P(PGM_P src);
    };
} // namespace HALIB_NAMESPACE