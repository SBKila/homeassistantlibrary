#include "HAMessage.h"

namespace HALIB_NAMESPACE
{
    HAMessage::HAMessage(const char *p_pTopic, const char *p_pMessage, boolean p_Retain)
        : HAAction(POSTMESSAGE, p_pTopic)
    {
        message = NULL;
        retain = false;
        HALIB_ACTION_DEBUG_MSG("%s %s\n", __func__, p_pMessage);
        setMessage(p_pMessage);
        setRetain(p_Retain);
        HALIB_ACTION_DEBUG_MSG("%sEND\n", __func__);
    }

    HAMessage::HAMessage() : HAAction(POSTMESSAGE)
    {
        message = NULL;
        retain = false;
    }

    HAMessage::~HAMessage()
    {
        if (NULL != message)
        {
            free(message);
        }
    }

    const char *HAMessage::getMessage()
    {
        return message;
    }

    void HAMessage::setMessage(const char *message)
    {
        HALIB_ACTION_DEBUG_MSG("%s %s\n", __func__, message);
        // Safety: Free previous message if it exists
        if (this->message != NULL)
        {
            free(this->message);
            this->message = NULL;
        }

        if (message != NULL)
        {
            this->message = strdup(message);
        }
        HALIB_ACTION_DEBUG_MSG("%sEND\n", __func__);
    }

    void HAMessage::setRetain(boolean p_Retain)
    {
        retain = p_Retain;
    }

    boolean HAMessage::isRetain()
    {
        return retain;
    }

    bool HAMessage::operator!=(const HAMessage &other)
    {
        return HAAction::operator!=(other) || (0 != strcmp(other.message, message));
    }

    bool HAMessage::operator==(const HAMessage &other)
    {
        return HAAction::operator==(other) && (0 == strcmp(other.message, message));
    }

} // namespace HALIB_NAMESPACE