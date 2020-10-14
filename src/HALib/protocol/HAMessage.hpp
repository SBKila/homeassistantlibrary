#pragma once

#include <Arduino.h>
#include "HAAction.hpp"

namespace HALIB_NAMESPACE
{
    class HAMessage : public HAAction
    {

    public:
        HAMessage(const char *p_pTopic, const char *p_pMessage, boolean p_Retain) : HAAction(POSTMESSAGE,p_pTopic)
        {
            setMessage(p_pMessage);
            setRetain(p_Retain);
        }
        HAMessage() : HAAction(POSTMESSAGE)
        {
            message = NULL;
            retain = false;
        }
        virtual ~HAMessage()
        {
            if (NULL != message)
                free(message);
        }

        const char *getMessage()
        {
            return message;
        }
        void setMessage(const char *message)
        {
            this->message = strdup(message);
        }

        void setRetain(boolean p_Retain)
        {
            retain = p_Retain;
        }
        boolean isRetain()
        {
            return retain;
        }

        bool operator!=(const HAMessage &other) { return HAAction::operator!=(other) || (0 != strcmp(other.message, message)); }
        bool operator==(const HAMessage &other) { return HAAction::operator==(other) && (0 == strcmp(other.message, message)); }

    private:
        char *message;
        boolean retain;
    };
} // namespace HALIB_NAMESPACE