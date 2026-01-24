#pragma once

#include <Arduino.h>

namespace HALIB_NAMESPACE
{
    // Enumeration defining the types of actions available
    enum ActionType
    {
        POSTMESSAGE,
        SUBSCRIPTION
    };

    class HAAction
    {
    public:
        // Constructor: Initializes an action with just a type
        HAAction(ActionType type);

        // Constructor: Initializes an action with a type and a topic string
        HAAction(ActionType type, const char *topic);

        // Destructor: Frees the memory allocated for the topic
        virtual ~HAAction();

        // Returns the type of the action
        ActionType getType();

        // Returns the topic associated with the action
        const char *getTopic();

        // Sets (or updates) the topic string.
        // Handles memory allocation (strdup) and deallocation of previous value.
        void setTopic(const char *topic);

        // Increments and returns the retry counter
        short retry();

        // Inequality operator: Compares type and topic string content
        bool operator!=(const HAAction &other);

        // Equality operator: Compares type and topic string content
        bool operator==(const HAAction &other);

    protected:
        ActionType type;
        char *topic;
        short retryCounter;
    };
} // namespace HALIB_NAMESPACE