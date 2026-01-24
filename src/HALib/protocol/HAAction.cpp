#include "HAAction.h"

namespace HALIB_NAMESPACE
{
    HAAction::HAAction(ActionType type)
    {
        this->type = type;
        this->topic = NULL;
        this->retryCounter = 0;
    }

    HAAction::HAAction(ActionType type, const char *topic)
    {
        this->type = type;
        this->topic = NULL; // Initialize to NULL to ensure setTopic works safely
        setTopic(topic);
        this->retryCounter = 0;
    }

    HAAction::~HAAction()
    {
        if (NULL != this->topic)
        {
            free(this->topic);
        }
    }

    ActionType HAAction::getType()
    {
        return this->type;
    }

    const char *HAAction::getTopic()
    {
        return this->topic;
    }

    void HAAction::setTopic(const char *topic)
    {
        // Safety: Free previous topic if it exists to prevent memory leaks
        if (this->topic != NULL)
        {
            free(this->topic);
            this->topic = NULL;
        }

        if (topic != NULL)
        {
            this->topic = strdup(topic);
        }
    }

    short HAAction::retry()
    {
        return this->retryCounter++;
    }

    bool HAAction::operator!=(const HAAction &other)
    {
        // Check if types are different
        if (other.type != this->type)
        {
            return true;
        }

        // Handle NULL pointers safely for strcmp
        if (this->topic == NULL || other.topic == NULL)
        {
            return this->topic != other.topic;
        }

        return (0 != strcmp(other.topic, this->topic));
    }

    bool HAAction::operator==(const HAAction &other)
    {
        // Check if types are the same
        if (other.type != this->type)
        {
            return false;
        }

        // Handle NULL pointers safely for strcmp
        if (this->topic == NULL || other.topic == NULL)
        {
            return this->topic == other.topic;
        }

        return (0 == strcmp(other.topic, this->topic));
    }

} // namespace HALIB_NAMESPACE