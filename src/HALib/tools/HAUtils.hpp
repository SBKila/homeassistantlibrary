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
                // size += (index == 0) ? 0 : 1;
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
            uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
            uint32_t indexRotation = 0;
            // generate initValue with component name
            for (size_t index = 0; index < strlen(pName); index++)
            {
                uint32_t input = (*(pName + index));
                input = input << indexRotation++;
                if (indexRotation > (uint32_t)sizeof(identifier))
                    indexRotation = 0;
                identifier ^= input;
            }
            // and component type
            for (size_t index = 0; index < strlen_P(componentTypeTag[pType]); index++)
            {
                uint32_t input = pgm_read_byte(componentTypeTag[pType] + index);
                input = input << indexRotation++;
                if (indexRotation > (uint32_t) sizeof(identifier))
                    indexRotation = 0;
                identifier ^= input;
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
