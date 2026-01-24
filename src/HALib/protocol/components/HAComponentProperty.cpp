#include "HAComponentProperty.h"
#include "../../tools/HAUtils.h"

// We include this to ensure compatibility if other types are needed,
// though strictly speaking Constante.hpp handles most keys.
#include "HAComponentType.h"

namespace HALIB_NAMESPACE
{
    HAComponentProperty::HAComponentProperty(HAComponentPropertyKey name, const char *value)
    {
        mName = name;
        if (NULL != value)
        {
            // --- ESP8266 MEMORY DETECTION LOGIC ---
            // On ESP8266, Flash memory is mapped starting at 0x40200000.
            // If the pointer address is higher than this, it points to Flash (PROGMEM).
            if ((uint32_t)value >= 0x40200000)
            {
                // Case: Pointer to Flash (PROGMEM)
                // Use helper to allocate RAM and copy from Flash safely
                mValue = HAUtils::strdup_P((PGM_P)value);
            }
            else
            {
                // Case: Pointer to RAM
                // Use standard duplication
                mValue = strdup(value);
            }
        }
        else
        {
            // Case: NULL value provided
            // Load the default value associated with this key from Flash
            PGM_P defaultValuePtr = (PGM_P)pgm_read_ptr(&PropertyDefaultValue[mName]);

            // Allocate and copy the default value (using PROGMEM functions)
            // Note: We avoid double allocation by doing it manually here or using HAUtils::strdup_P
            size_t len = strlen_P(defaultValuePtr);
            mValue = (char *)malloc(len + 1);
            if (mValue != NULL)
            {
                strcpy_P(mValue, defaultValuePtr);
            }
        }
    }

    HAComponentProperty::~HAComponentProperty()
    {
        if (NULL != mValue)
        {
            free(mValue);
        }
    }

    PGM_P HAComponentProperty::getName()
    {
        // Read the pointer from the pointer array in Flash
        return (PGM_P)pgm_read_ptr(&DiscoveryMessageKeyLabel[mName]);
    }

    HAComponentPropertyKey HAComponentProperty::getKey()
    {
        return mName;
    }

    const char *HAComponentProperty::getValue()
    {
        return mValue;
    }

    bool HAComponentProperty::operator!=(const HAComponentProperty &other)
    {
        return (other.mName != mName);
    }

    bool HAComponentProperty::operator==(const HAComponentProperty &other)
    {
        return (other.mName == mName);
    }

    size_t HAComponentProperty::getJson(char **ppJsonBuffer)
    {
        // 1. Retrieve the property label (key) from Flash
        PGM_P keyLabel = (PGM_P)pgm_read_ptr(&DiscoveryMessageKeyLabel[mName]);

        // 2. Calculate the required length
        // Format is either "key":"value" or "key":value (if object)
        size_t length = ((*mValue == '{') ? 3 : 5) + strlen_P(keyLabel) + strlen(mValue);

        if (NULL != ppJsonBuffer)
        {
            if (*mValue == '{')
            {
                // It is a JSON object (starts with {) - No quotes around value
                // Note: On ESP8266, sprintf_P can handle PGM_P directly for %s
                sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_OBJECT, keyLabel, mValue);
            }
            else
            {
                // It is a String - Quotes around value
                sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_STRING, keyLabel, mValue);
            }
        }

        return length;
    }

} // namespace HALIB_NAMESPACE