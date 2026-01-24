#include "HAMessage.h"

namespace HALIB_NAMESPACE
{
    HAMessage::HAMessage(const char *p_pTopic, const char *p_pMessage, boolean p_Retain)
        : HAAction(POSTMESSAGE, p_pTopic)
    {
        setMessage(p_pMessage);
        setRetain(p_Retain);
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