#pragma once

#include <Arduino.h>
#include <pgmspace.h>
#include "HAComponentPropertyConstante.hpp"
#include "HAComponentType.hpp"


namespace HALIB_NAMESPACE
{
    class HAComponentProperty
    {
    public:
        HAComponentProperty(HAComponentPropertyKey name, const char *value = NULL)
        {
            mName = name;
            if (NULL != value)
            {
                mValue = strdup(value);
            }
            else
            {
                mValue = (char *)malloc(strlen_P(PropertyDefaultValue[mName]) + 1);
                strcpy_P(mValue, PropertyDefaultValue[mName]);
            }
        }
        virtual ~HAComponentProperty()
        {
            if (NULL != mValue)
            {
                free(mValue);
            }
        }
        PGM_P getName()
        {
            return DiscoveryMessageKeyLabel[mName];
        }
        HAComponentPropertyKey getKey()
        {
            return mName;
        }
        const char *getValue()
        {
            return mValue;
        }
        bool operator!=(const HAComponentProperty &other)
        {
            return (other.mName != mName);
        }
        bool operator==(const HAComponentProperty &other)
        {
            return (other.mName == mName);
        }

        size_t getJson(char **ppJsonBuffer = NULL)
        {
            size_t length = ((*mValue == '{') ? 3 : 5) + strlen_P(DiscoveryMessageKeyLabel[mName]) + strlen(mValue);

            if (NULL != ppJsonBuffer)
            {
                if (*mValue == '{')
                {
                    sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_OBJECT, DiscoveryMessageKeyLabel[mName], mValue);
                }
                else
                {
                    sprintf_P(*ppJsonBuffer, PROPERTY_TEMPLATE_STRING, DiscoveryMessageKeyLabel[mName], mValue);
                }
            }

            return length;
        }


    private:
        HAComponentPropertyKey mName;
        char *mValue;
    };
}