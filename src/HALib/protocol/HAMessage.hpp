#pragma once

#include <Arduino.h>

namespace HALIB_NAMESPACE
{
    class HAMessage
    {
    public:
        HAMessage(const char *p_pTopic, const char *p_pMessage, boolean p_Retain)
        {
            setTopic(p_pTopic);
            setMessage(p_pMessage);
            setRetain(p_Retain);
        }
        HAMessage()
        {
            topic = NULL;
            message = NULL;
            retain = false;
        }
        ~HAMessage()
        {
            if (NULL != topic)
                free(topic);
            if (NULL != message)
                free(message);
        }

        const char *getTopic()
        {
            return topic;
        }
        void setTopic(const char *topic)
        {

            this->topic = strdup(topic);
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

        bool operator!=(const HAMessage &other) { return 0 != (strcmp(other.topic, topic) + strcmp(other.message, message)); }
        bool operator==(const HAMessage &other) { return 0 == (strcmp(other.topic, topic) + strcmp(other.message, message)); }

    private:
        char *topic;
        char *message;
        boolean retain;
    };
} // namespace HALIB_NAMESPACE