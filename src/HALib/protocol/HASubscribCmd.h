#pragma once

#include <Arduino.h>
#include "./HAAction.h"

namespace HALIB_NAMESPACE
{
    // Class representing a subscription (or unsubscription) command to an MQTT topic.
    // Inherits from HAAction.
    class HASubscribCmd : public HAAction
    {
    public:
        // Constructor: Initializes the command with a topic and a boolean flag.
        // pSubscrib: true to subscribe, false to unsubscribe.
        HASubscribCmd(const char *pTopic, boolean pSubscrib);

        // Returns true if this is a subscribe command, false if unsubscribe.
        boolean isSubscribtion();

        // Inequality operator: Checks if base action or subscription status differs.
        bool operator!=(const HASubscribCmd &other);

        // Equality operator: Checks if base action and subscription status are identical.
        bool operator==(const HASubscribCmd &other);

    private:
        boolean subscrib;
    };
} // namespace HALIB_NAMESPACE