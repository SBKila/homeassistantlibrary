#include "HASubscribCmd.h"

namespace HALIB_NAMESPACE
{
    HASubscribCmd::HASubscribCmd(const char *pTopic, boolean pSubscrib)
        : HAAction(SUBSCRIPTION, pTopic)
    {
        this->subscrib = pSubscrib;
    }

    boolean HASubscribCmd::isSubscribtion()
    {
        return this->subscrib;
    }

    bool HASubscribCmd::operator!=(const HASubscribCmd &other)
    {
        // Compare base class fields (type/topic) OR local subscription status
        return HAAction::operator!=(other) || (other.subscrib != this->subscrib);
    }

    bool HASubscribCmd::operator==(const HASubscribCmd &other)
    {
        // Compare base class fields (type/topic) AND local subscription status
        return HAAction::operator==(other) && (other.subscrib == this->subscrib);
    }

} // namespace HALIB_NAMESPACE