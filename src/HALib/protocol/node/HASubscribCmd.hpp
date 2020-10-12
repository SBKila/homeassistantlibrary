#pragma once

#include <Arduino.h>

namespace HALIB_NAMESPACE
{
    class HASubscribCmd
    {
    public:
        HASubscribCmd(const char *pTopic, boolean pSubscrib)
        {
            topic = strdup(pTopic);
            subscrib = pSubscrib;
        }
        ~HASubscribCmd()
        {
            if (NULL != topic)
                free(topic);
        }

        const char *getTopic()
        {
            return topic;
        }
        boolean isSubscribtion()
        {
            return subscrib;
        }

        bool operator!=(const HASubscribCmd &other) { return (other.subscrib != subscrib) || (0 != strcmp(other.topic, topic)); }
        bool operator==(const HASubscribCmd &other) { return (other.subscrib == subscrib) && (0 == strcmp(other.topic, topic)); }

    private:
        char *topic;
        boolean subscrib;
    };
} // namespace HALIB_NAMESPACE