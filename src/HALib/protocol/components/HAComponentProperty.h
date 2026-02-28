#pragma once

#include <Arduino.h>
#include <pgmspace.h>
#include "HAComponentPropertyConstante.hpp"

namespace HALIB_NAMESPACE
{
    class HAComponentProperty
    {
    public:
        // Constructor: Initializes the property with a key (name) and an optional value.
        // Supports value in RAM, Flash (PROGMEM), or NULL (default value).
        HAComponentProperty(HAComponentPropertyKey name, const char *value = NULL);

        // Destructor: Frees the allocated memory for the value.
        virtual ~HAComponentProperty();

        // Returns the name/label of the property from Flash (e.g., "stat_t").
        PGM_P getName();

        // Returns the enum key of the property.
        HAComponentPropertyKey getKey();

        // Returns the current value stored in RAM.
        const char *getValue();

        // Comparison operators based on the property name (key).
        bool operator!=(const HAComponentProperty &other);
        bool operator==(const HAComponentProperty &other);

        // Generates the JSON representation of this property (e.g., "name":"value").
        // If ppJsonBuffer is provided, it fills the buffer.
        // Returns the length of the generated JSON string.
        size_t getJson(char **ppJsonBuffer = NULL);

    private:
        HAComponentPropertyKey mName;
        const char *mValue;
    };
} // namespace HALIB_NAMESPACE