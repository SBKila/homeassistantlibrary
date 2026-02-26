#include "HAComponentProperty.h"
#include "../../tools/HAUtils.h"
#include "HAComponentType.h"
namespace HALIB_NAMESPACE
{
    HAComponentProperty::HAComponentProperty(HAComponentPropertyKey name, const char *value)
    {
        mName = name;
        HALIB_COMPONENT_DEBUG_MSG("Constructor: name=%d, value=%s\n", name, (value != NULL) ? value : "NULL");
        if (NULL != value)
        {
            // --- ESP Memory Optimization Logic ---
            // Check memory address to determine if it is Flash (PROGMEM) or RAM.
            // ESP8266: irom0_text_start is around 0x40200000.
            if ((uint32_t)value >= 0x40200000)
            {
                // Source is in Flash.
                HALIB_COMPONENT_DEBUG_MSG("Value is in Flash (PROGMEM), using zero-copy\n");
                mValue = HAUtils::strdup_P((PGM_P)value);
            }
            else
            {
                // Case 2: Pointer to RAM.
                // The source is likely a temporary buffer (stack/heap).
                // We MUST copy it to our own heap storage to ensure persistence.
                HALIB_COMPONENT_DEBUG_MSG("Value is in RAM, copying to managed heap\n");
                mValue = strdup(value);
            }
        }
        else
        {
            // Case 3: No value provided (NULL).
            // Use the default value defined in PROGMEM (PropertyDefaultValue table).
            HALIB_COMPONENT_DEBUG_MSG("No value provided, using default from PROGMEM\n");
            PGM_P defaultValuePtr = (PGM_P)pgm_read_ptr(&PropertyDefaultValue[mName]);
            mValue = HAUtils::strdup_P((PGM_P)defaultValuePtr);
        }
    }

    HAComponentProperty::~HAComponentProperty()
    {
        // Only free memory if we explicitly allocated it (RAM case).
        // Never try to free a Flash pointer.
        HALIB_COMPONENT_DEBUG_MSG("Destructor: Freeing allocated RAM value\n");
        if (NULL != mValue)
        {
            free((void *)mValue);
        }
        mValue = NULL;
    }

    PGM_P HAComponentProperty::getName()
    {
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
        PGM_P keyLabel = (PGM_P)pgm_read_ptr(&DiscoveryMessageKeyLabel[mName]);

        // Calculate length.
        // Note: strlen works on Flash pointers on ESP8266/ESP32 transparently.
        size_t valueLen = strlen(mValue);
        size_t length = ((*mValue == '{') ? 3 : 5) + strlen_P(keyLabel) + valueLen;

        if (NULL != ppJsonBuffer)
        {
            // Use sprintf_P with %s. It handles both RAM and Flash pointers correctly
            // on this architecture, provided the pointer is valid.
            if (*mValue == '{')
            {
                // JSON Object format: "key":value
                sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_OBJECT, keyLabel, mValue);
            }
            else
            {
                // JSON String format: "key":"value"
                sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_STRING, keyLabel, mValue);
            }
        }

        return length;
    }

} // namespace HALIB_NAMESPACE