#include "HANode.h"
#include "../../tools/HAUtils.h"
#include "../HASubscribCmd.h" // Assuming these haven't been split yet, otherwise use .h
#include "../HAMessage.h"     // Assuming these haven't been split yet, otherwise use .h
#include "../components/HAComponentType.h"
#include "../components/HAComponentProperty.h"
#include <pgmspace.h>
#include <stdio.h>

namespace HALIB_NAMESPACE
{
    const char AVAILABILITY_TOPIC_TEMPLATE[] PROGMEM = "homeassistant/%s-%08X/avty";

    HANode::HANode(const char *pName)
    {
        mIsHAConnected = false;
        mName = strdup(pName);
        // Generate unique ID for the device
        mId = HAUtils::generateId(pName, DEVICE);

        /* AVAILABILITY SETUP */
        // Calculate size: Name len + prefix/suffix len + ID hex len (8) + null terminator
        char *out = (char *)calloc(strlen(mName) + 20 + 8 + 1, sizeof(char));
        sprintf_P(out, AVAILABILITY_TOPIC_TEMPLATE, mName, mId);

        mProperties.append(new HAComponentProperty(PROP_AVAILABILITY_TOPIC, out));
        mProperties.append(new HAComponentProperty(PROP_PAYLOAD_AVAILABLE, "online"));
        mProperties.append(new HAComponentProperty(PROP_PAYLOAD_NOT_AVAILABLE, "offline"));

        free(out);
    }

    HANode::~HANode()
    {
        if (NULL != mName)
        {
            free(mName);
        }

        // Clean up pending actions
        HAAction *pAction = mOutboxAction.shift();
        while (NULL != pAction)
        {
            delete (pAction);
            pAction = mOutboxAction.shift();
        }

        // Clean up properties
        HAUtils::deleteProperties(mProperties);
    }

    void HANode::setDeviceInfo(const char *pManuf, const char *pModel, const char *pRelease)
    {
        LinkedList<HAComponentProperty *> lProperties;

        if (NULL != pManuf)
        {
            lProperties.append(new HAComponentProperty(PROP_DEVICE_MANUFACTURER, pManuf));
        }
        if (NULL != pModel)
        {
            lProperties.append(new HAComponentProperty(PROP_DEVICE_MODEL, pModel));
        }
        if (NULL != pRelease)
        {
            lProperties.append(new HAComponentProperty(PROP_DEVICE_RELEASE, pRelease));
        }

        lProperties.append(new HAComponentProperty(PROP_NAME, mName));

        char *out = (char *)malloc(9 * sizeof(char));
        memset(out, 0, 9 * sizeof(char));
        sprintf(out, "%08X", mId);
        lProperties.append(new HAComponentProperty(PROP_DEVICE_IDENTIFIER, out));
        free(out);

        // Convert the list of properties to a JSON string for the "device" field
        out = HAUtils::propertyListToJson(lProperties);
        HAUtils::deleteProperties(lProperties);

        mProperties.append(new HAComponentProperty(PROP_DEVICE, out));
        free(out);
    }

    const char *HANode::getName()
    {
        return mName;
    }

    uint32_t HANode::getId()
    {
        return mId;
    }

    const char *HANode::getProperty(HAComponentPropertyKey name)
    {
        return HAUtils::getProperty(mProperties, name);
    }

    void HANode::removeComponent(IHAComponent *p_pComponent)
    {
        // HALIB_NODE_DEBUG_MSG("removeComponent\n");
        if (NULL != p_pComponent)
        {
            p_pComponent->setNode(NULL);
            mComponents.remove(p_pComponent);
        }
        // HALIB_NODE_DEBUG_MSG("removeComponentEND\n");
    }

    void HANode::addComponent(IHAComponent *p_pComponent)
    {
        // HALIB_NODE_DEBUG_MSG("addComponent\n");
        if (NULL != p_pComponent)
        {
            p_pComponent->setNode(this);
            mComponents.append(p_pComponent);
        }
        // HALIB_NODE_DEBUG_MSG("addComponentEND\n");
    }

    void HANode::postAutoDiscovery()
    {
        // HALIB_NODE_DEBUG_MSG("===>postAutoDiscovery\n");

        char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
        size_t deviceInfoLength = strlen(tempDeviceInfo) - 2; // -2 to remove opening and closing braces {}

        // Treat all components
        for (int index = 0; index < mComponents.getSize(); index++)
        {
            IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
            char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
            char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();
            char *discoveryMessage = componentDiscoveryMessage;

            // If device info is present, extend the component discovery message
            if (deviceInfoLength != 0)
            {
                size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
                discoveryMessage = (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

                strcpy(discoveryMessage, componentDiscoveryMessage);
                // Inject the device info JSON properties before the closing brace
                strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
                strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1, tempDeviceInfo + 1, deviceInfoLength); // +1 to skip first {
                strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 + deviceInfoLength, "}");

                free(componentDiscoveryMessage);
            }

            postMessage(new HAMessage(topic, discoveryMessage, true));
            free(topic);
            free(discoveryMessage);
        }
        free(tempDeviceInfo);
    }

    void HANode::postDiscoveryMessage(IHAComponent *pComponent)
    {
        // HALIB_NODE_DEBUG_MSG("===>postDiscoveryMessage\n");
        char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
        size_t deviceInfoLength = strlen(tempDeviceInfo) - 2;
        char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
        char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();

        char *discoveryMessage = componentDiscoveryMessage;

        // If device info is present, patch the component discovery message
        if (deviceInfoLength != 0)
        {
            size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
            discoveryMessage = (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

            strcpy(discoveryMessage, componentDiscoveryMessage);
            strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
            strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1, tempDeviceInfo + 1, deviceInfoLength); // +1 to skip first {
            strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 + deviceInfoLength, "}");

            free(componentDiscoveryMessage);
        }

        postMessage(new HAMessage(topic, discoveryMessage, true));
        free(topic);
        free(discoveryMessage);
        free(tempDeviceInfo);
    }

    bool HANode::onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length)
    {
        for (int index = 0; index < mComponents.getSize(); index++)
        {
            // Get component at index
            IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);

            if (pComponent->onHAMessage(topic, p_pPayload, length))
            {
                return true;
            }
        }
        return false;
    }

    void HANode::postMessage(HAMessage *pMessage)
    {
        // Only queue messages if connected, unless they are retained messages (which must be sent on connect)
        if (mIsHAConnected || pMessage->isRetain())
        {
            mOutboxAction.append(pMessage);
        }
        else
        {
            delete pMessage;
        }
    }

    void HANode::onHAConnect(boolean isConnected)
    {
        mIsHAConnected = isConnected;
        if (isConnected)
        {
            for (int index = 0; index < mComponents.getSize(); index++)
            {
                // Notify all components that HA is connected
                IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
                pComponent->_onHAConnect();
            }
        }
    }

    int HANode::getDiscoveryMessages(HAMessage *pMessages)
    {
        if (NULL != pMessages)
        {
            for (int index = 0; index < mComponents.getSize(); index++)
            {
                IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
                char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
                char *discoveryMessage = pComponent->buildDiscoveryMessage();

                pMessages[index].setTopic(topic);
                pMessages[index].setMessage(discoveryMessage);

                free(topic);
                free(discoveryMessage);
            }
        }
        return mComponents.getSize();
    }

    HAAction *HANode::pickupAction()
    {
        return mOutboxAction.shift();
    }

    int HANode::actionsSize()
    {
        return mOutboxAction.getSize();
    }

    void HANode::retryAction(HAAction *p_pAction)
    {
        mOutboxAction.append(p_pAction);
    }

    void HANode::postMessage(const char *pTopic, const char *pMessage, boolean retain)
    {
        postMessage(new HAMessage(pTopic, pMessage, retain));
    }

    void HANode::registerToHA(const char *topic, boolean subscription)
    {
        // HALIB_NODE_DEBUG_MSG("===>postSubscribe\n");
        mOutboxAction.append(new HASubscribCmd(topic, subscription));
    }

} // namespace HALIB_NAMESPACE