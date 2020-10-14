#pragma once

#include <Arduino.h>
#include "./HAAction.hpp"

namespace HALIB_NAMESPACE
{
    class HASubscribCmd : public HAAction
    {
    public:
        HASubscribCmd(const char *pTopic, boolean pSubscrib) : HAAction(SUBSCRIPTION,pTopic)
        {
            subscrib = pSubscrib;
        }
       

        boolean isSubscribtion()
        {
            return subscrib;
        }

        bool operator!=(const HASubscribCmd &other) { return HAAction::operator!=(other) || (other.subscrib != subscrib); }
        bool operator==(const HASubscribCmd &other) { return HAAction::operator==(other) && (other.subscrib == subscrib); }

    private:
        boolean subscrib;
    };
} // namespace HALIB_NAMESPACE