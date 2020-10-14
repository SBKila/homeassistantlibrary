#pragma once

#include <Arduino.h>

namespace HALIB_NAMESPACE
{
    enum ActionType
    {
        POSTMESSAGE,
        SUBSCRIPTION
    };
    class HAAction
    {
    public:
        HAAction(ActionType type)
        {
            this->type = type;
            topic = NULL;
            retryCounter=0;
        }
        HAAction(ActionType type, const char *topic)
        {
            this->type = type;
            setTopic(topic);
            retryCounter=0;
        }
        virtual ~HAAction()
        {
            if (NULL != topic)
                free(topic);
        }
        ActionType getType()
        {
            return type;
        }
        const char *getTopic()
        {
            return topic;
        }
        void setTopic(const char *topic)
        {
            this->topic = strdup(topic);
        }

        short retry(){
            return retryCounter++;
        }

        bool operator!=(const HAAction &other) { return (other.type != type) || (0 != strcmp(other.topic, topic)); }
        bool operator==(const HAAction &other) { return (other.type == type) && (0 == strcmp(other.topic, topic)); }

    protected:
        ActionType type;
        char *topic;
        short retryCounter;
    }; 
} // namespace HALIB_NAMESPACE