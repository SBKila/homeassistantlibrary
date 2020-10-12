#pragma once

#include "../protocol/components/HAComponentProperty.hpp"

// #include "../debug.h"
namespace HALIB_NAMESPACE
{
    class HAUtils
    {
    public:
        static char *propertyListToJson(LinkedList<HAComponentProperty *> &pProperties)
        {
            size_t size = 2;

            for (int index = 0; index < pProperties.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = pProperties.get(index);
                size += pComponentProperty->getJson(); // add item size
                size++;                                // add ',' size
                //size += (index == 0) ? 0 : 1;
            }

            char *jsonString = (char *)calloc(size + 1, sizeof(char));
            char *currentPosition = jsonString;
            *currentPosition = '{';
            currentPosition++;

            for (int index = 0; index < pProperties.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)pProperties.get(index);
                // no ',' for first element
                if (currentPosition != jsonString + 1)
                {
                    *currentPosition = ',';
                    currentPosition++;
                }
                currentPosition += pComponentProperty->getJson(&currentPosition);
            }

            *currentPosition = '}';
            return jsonString;
        };

        static void deleteProperties(LinkedList<HAComponentProperty *> &pProperties)
        {
            HAComponentProperty *pProperty = pProperties.shift();
            while (NULL != pProperty)
            {
                delete (pProperty);
                pProperty = (HAComponentProperty *)pProperties.shift();
            }
        }
        static const char *getProperty(LinkedList<HAComponentProperty *> &pProperties, HAComponentPropertyKey name)
        {
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
        static uint32_t generateId(const char *pName, ComponentType pType)
        {
            byte offset = 3;
            byte prevVal = 0;

            uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();

            // generate initValue with component name
            for (size_t index = 0; index < strlen(pName); index++)
            {
                prevVal ^= (*(pName + index));
            }
            // and component type
            for (size_t index = 0; index < strlen_P(componentTypeTag[pType]); index++)
            {
                prevVal ^= pgm_read_byte(componentTypeTag[pType] + index);
            }

            // generate a Xor
            for (size_t index = 0; index < strlen(pName); index++)
            {
                uint32_t val = ((identifier >> (4 * offset)) & 0xFF) ^ prevVal;
                identifier ^= (val << (4 * offset--));
                if (offset < 0)
                    offset = 3;
                prevVal = val;
            }

            return identifier;
        };

        static char *strdup_P(PGM_P src)
        {
            size_t length = strlen_P(src);
            char *dst = (char *)malloc(length + 1);
            dst[length] = 0;
            strncpy_P(dst, src, length);
            return dst;
        }
    };
} // namespace HALIB_NAMESPACE
