#include "HAUtils.h"
#include <pgmspace.h>

#include "../protocol/components/HAComponentType.h"

namespace HALIB_NAMESPACE
{

    char *HAUtils::propertyListToJson(LinkedList<HAComponentProperty *> &pProperties)
    {
        size_t size = 2; // For '{' and '}'

        for (int index = 0; index < pProperties.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = pProperties.get(index);
            size += pComponentProperty->getJson(); // add item size
            size++;                                // add ',' size
        }

        char *jsonString = (char *)calloc(size + 1, sizeof(char));
        char *currentPosition = jsonString;
        *currentPosition = '{';
        currentPosition++;

        for (int index = 0; index < pProperties.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = (HAComponentProperty *)pProperties.get(index);

            // No ',' before the first element
            if (currentPosition != jsonString + 1)
            {
                *currentPosition = ',';
                currentPosition++;
            }
            currentPosition += pComponentProperty->getJson(&currentPosition);
        }

        *currentPosition = '}';
        return jsonString;
    }

    void HAUtils::deleteProperties(LinkedList<HAComponentProperty *> &pProperties)
    {
        HAComponentProperty *pProperty = pProperties.shift();
        while (NULL != pProperty)
        {
            delete (pProperty);
            pProperty = (HAComponentProperty *)pProperties.shift();
        }
    }

    const char *HAUtils::getProperty(LinkedList<HAComponentProperty *> &pProperties, HAComponentPropertyKey name)
    {
        // Create a temporary object for the search
        HAComponentProperty propertyToFind(name, "");

        int propertyIndex = pProperties.find(&propertyToFind, true);

        if (propertyIndex != -1)
        {
            return ((HAComponentProperty *)pProperties.get(propertyIndex))->getValue();
        }
        else
        {
            return NULL;
        }
    }

    uint32_t HAUtils::generateId(const char *pName, ComponentType pType)
    {
        uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
        uint32_t indexRotation = 0;

        // Generate initValue with component name
        for (size_t index = 0; index < strlen(pName); index++)
        {
            uint32_t input = (*(pName + index));
            input = input << indexRotation++;
            if (indexRotation > (uint32_t)sizeof(identifier))
                indexRotation = 0;
            identifier ^= input;
        }

        // And component type
        // NOTE: Kept the PROGMEM bug fix (&) discussed previously
        PGM_P expectedPayload = (PGM_P)pgm_read_ptr(&componentTypeTag[pType]);

        for (size_t index = 0; index < strlen_P(expectedPayload); index++)
        {
            uint32_t input = pgm_read_byte(expectedPayload + index);
            input = input << indexRotation++;
            if (indexRotation > (uint32_t)sizeof(identifier))
                indexRotation = 0;
            identifier ^= input;
        }
        return identifier;
    }

    char *HAUtils::strdup_P(PGM_P src)
    {
        size_t length = strlen_P(src);
        char *dst = (char *)malloc(length + 1);
        if (dst != NULL) // Safety check is always recommended
        {
            dst[length] = 0;
            strcpy_P(dst, src); // strcpy_P is safer than strncpy_P if length is already calculated
        }
        return dst;
    }

} // namespace HALIB_NAMESPACE