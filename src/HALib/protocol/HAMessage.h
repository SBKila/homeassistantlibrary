#pragma once

#include <Arduino.h>
#include "HAAction.h"

namespace HALIB_NAMESPACE
{
    class HAMessage : public HAAction
    {
    public:
        // Constructor: Creates a message with a topic, payload, and retain flag
        HAMessage(const char *p_pTopic, const char *p_pMessage, boolean p_Retain);

        // Default constructor
        HAMessage();

        // Destructor: Frees memory allocated for the message payload
        virtual ~HAMessage();

        // Returns the message payload
        const char *getMessage();

        // Sets the message payload (allocates memory)
        void setMessage(const char *message);

        // Sets the retain flag
        void setRetain(boolean p_Retain);

        // Returns the retain flag status
        boolean isRetain();

        // Inequality operator: Compares base action and message content
        bool operator!=(const HAMessage &other);

        // Equality operator: Compares base action and message content
        bool operator==(const HAMessage &other);

    private:
        char *message;
        boolean retain;
    };
} // namespace HALIB_NAMESPACE