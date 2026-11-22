#include "HANode.h"
#include "../../tools/HAUtils.h"
#include "../HAMessage.h"
#include "../HASubscribCmd.h"
#include "../components/HAComponentProperty.h"
#include "../components/HAComponentType.h"
#include <LittleFS.h>
#include <pgmspace.h>
#include <stdio.h>
#define SWAP_FILE_PATH "/outbox.bin"
namespace HALIB_NAMESPACE {
const char AVAILABILITY_TOPIC_TEMPLATE[] PROGMEM = "homeassistant/%s-%08X/avty";

HANode::HANode(const char *pName) {
  mIsHAConnected = false;
  mName = strdup(pName);
  // Generate unique ID for the device
  mId = HAUtils::generateId(pName, DEVICE);

  /* AVAILABILITY SETUP */
  // Calculate size: Name len + prefix/suffix len + ID hex len (8) + null
  // terminator
  char *out = (char *)calloc(strlen(mName) + 20 + 8 + 1, sizeof(char));
  sprintf_P(out, AVAILABILITY_TOPIC_TEMPLATE, mName, mId);

  mProperties.append(new HAComponentProperty(PROP_AVAILABILITY_TOPIC, out));
  mProperties.append(new HAComponentProperty(PROP_PAYLOAD_AVAILABLE, "online"));
  mProperties.append(
      new HAComponentProperty(PROP_PAYLOAD_NOT_AVAILABLE, "offline"));

  free(out);

  // Purge swap file on boot to guarantee 100% volatile behavior
  if (LittleFS.exists(SWAP_FILE_PATH)) {
    LittleFS.remove(SWAP_FILE_PATH);
  }
  mSwapFileExists = false;
}

HANode::~HANode() {
  if (NULL != mName) {
    free(mName);
  }

  // Clean up pending actions
  HAAction *pAction = mOutboxAction.shift();
  while (NULL != pAction) {
    delete (pAction);
    pAction = mOutboxAction.shift();
  }

  // Clean up properties
  HAUtils::deleteProperties(mProperties);
}

void HANode::setDeviceInfo(const char *pManuf, const char *pModel,
                           const char *pRelease) {
  LinkedList<HAComponentProperty *> lProperties;

  if (NULL != pManuf) {
    lProperties.append(
        new HAComponentProperty(PROP_DEVICE_MANUFACTURER, pManuf));
  }
  if (NULL != pModel) {
    lProperties.append(new HAComponentProperty(PROP_DEVICE_MODEL, pModel));
  }
  if (NULL != pRelease) {
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

const char *HANode::getName() { return mName; }

uint32_t HANode::getId() { return mId; }

const char *HANode::getProperty(HAComponentPropertyKey name) {
  return HAUtils::getProperty(mProperties, name);
}

void HANode::removeComponent(IHAComponent *p_pComponent) {
  // HALIB_NODE_DEBUG_MSG("removeComponent\n");
  if (NULL != p_pComponent) {
    p_pComponent->setNode(NULL);
    mComponents.remove(p_pComponent);
  }
  // HALIB_NODE_DEBUG_MSG("removeComponentEND\n");
}

void HANode::addComponent(IHAComponent *p_pComponent) {
  // HALIB_NODE_DEBUG_MSG("addComponent\n");
  if (NULL != p_pComponent) {
    p_pComponent->setNode(this);
    mComponents.append(p_pComponent);
  }
  // HALIB_NODE_DEBUG_MSG("addComponentEND\n");
}

void HANode::postAutoDiscovery() {
  HALIB_NODE_DEBUG_MSG("%s\n", __func__);

  char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
  size_t deviceInfoLength =
      strlen(tempDeviceInfo) - 2; // -2 to remove opening and closing braces {}

  // Treat all components
  for (int index = 0; index < mComponents.getSize(); index++) {
    IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
    char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
    char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();
    char *discoveryMessage = componentDiscoveryMessage;

    // If device info is present, extend the component discovery message
    if (deviceInfoLength != 0) {
      size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
      discoveryMessage =
          (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

      strcpy(discoveryMessage, componentDiscoveryMessage);
      // Inject the device info JSON properties before the closing brace
      strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
      strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1,
              tempDeviceInfo + 1, deviceInfoLength); // +1 to skip first {
      strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 +
                 deviceInfoLength,
             "}");

      free(componentDiscoveryMessage);
    }

    postMessage(new HAMessage(topic, discoveryMessage, true));
    free(topic);
    free(discoveryMessage);
  }
  free(tempDeviceInfo);
  HALIB_NODE_DEBUG_MSG("%sEND\n", __func__);
}

void HANode::postDiscoveryMessage(IHAComponent *pComponent) {
  HALIB_NODE_DEBUG_MSG("%s\n", __func__);
  // HALIB_NODE_DEBUG_MSG("===>postDiscoveryMessage\n");
  char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
  size_t deviceInfoLength = strlen(tempDeviceInfo) - 2;
  char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
  char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();

  char *discoveryMessage = componentDiscoveryMessage;

  // If device info is present, patch the component discovery message
  if (deviceInfoLength != 0) {
    size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
    discoveryMessage =
        (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

    strcpy(discoveryMessage, componentDiscoveryMessage);
    strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
    strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1,
            tempDeviceInfo + 1, deviceInfoLength); // +1 to skip first {
    strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 +
               deviceInfoLength,
           "}");

    free(componentDiscoveryMessage);
  }
  postMessage(new HAMessage(topic, discoveryMessage, true));
  free(topic);
  free(discoveryMessage);
  free(tempDeviceInfo);
  HALIB_NODE_DEBUG_MSG("%sEND\n", __func__);
}

bool HANode::onHAMessage(const char *topic, const byte *p_pPayload,
                         const unsigned int length) {
  for (int index = 0; index < mComponents.getSize(); index++) {
    // Get component at index
    IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);

    if (pComponent->onHAMessage(topic, p_pPayload, length)) {
      return true;
    }
  }
  return false;
}
void HANode::postMessage(const char *pTopic, const char *pMessage,
                         boolean retain) {
  // Directly call the main postMessage function with a new HAMessage object
  postMessage(new HAMessage(pTopic, pMessage, retain));
}

void HANode::postMessage(HAMessage *pMessage) {
  HALIB_NODE_DEBUG_MSG("%s\n", __func__);
  // Only queue messages if connected, unless they are retained messages (which
  // must be sent on connect)
  if (mIsHAConnected || pMessage->isRetain()) {
    // If RAM is full (>= 5) OR if there are already items in Flash (to respect
    // FIFO order)
    if (mOutboxAction.getSize() >= 5 || mSwapFileExists) {
      serializeAction(pMessage);
      delete pMessage; // Free RAM immediately, object is safe in Flash
    } else {
      mOutboxAction.append(pMessage);
    }
  } else {
    delete pMessage;
  }
  HALIB_NODE_DEBUG_MSG("%sEND\n", __func__);
}

void HANode::onHAConnect(boolean isConnected) {
  mIsHAConnected = isConnected;
  if (isConnected) {
    for (int index = 0; index < mComponents.getSize(); index++) {
      // Notify all components that HA is connected
      IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
      pComponent->_onHAConnect();
    }
  }
}

int HANode::getDiscoveryMessages(HAMessage *pMessages) {
  HALIB_NODE_DEBUG_MSG("%s\n", __func__);
  if (NULL != pMessages) {
    for (int index = 0; index < mComponents.getSize(); index++) {
      IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
      char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
      char *discoveryMessage = pComponent->buildDiscoveryMessage();

      pMessages[index].setTopic(topic);
      pMessages[index].setMessage(discoveryMessage);

      free(topic);
      free(discoveryMessage);
    }
  }
  HALIB_NODE_DEBUG_MSG("%sEND\n", __func__);
  return mComponents.getSize();
}

void HANode::registerToHA(const char *topic, boolean subscription) {
  // HALIB_NODE_DEBUG_MSG("===>postSubscribe\n");
  HASubscribCmd *cmd = new HASubscribCmd(topic, subscription);

  // Same logic: send to Flash if RAM is full or if Swap is active
  if (mOutboxAction.getSize() >= 5 || mSwapFileExists) {
    serializeAction(cmd);
    delete cmd;
  } else {
    mOutboxAction.append(cmd);
  }
}

void HANode::serializeAction(HAAction *pAction) {
  File f = LittleFS.open(SWAP_FILE_PATH, "a");
  if (!f)
    return;

  mSwapFileExists = true;

  // Write Status Flag: 0xFF = Unread
  uint8_t status = 0xFF;
  f.write(status);

  // Write Type
  uint8_t type = pAction->getType();
  f.write(type);

  // Write Topic Length & Topic
  const char *topic = pAction->getTopic();
  uint16_t topicLen = (topic != NULL) ? strlen(topic) : 0;
  f.write((uint8_t *)&topicLen, sizeof(topicLen));
  if (topicLen > 0)
    f.write((const uint8_t *)topic, topicLen);

  // Write Specific Payload
  if (type == POSTMESSAGE) {
    HAMessage *msg = (HAMessage *)pAction;
    const char *payload = msg->getMessage();
    uint16_t payloadLen = (payload != NULL) ? strlen(payload) : 0;
    f.write((uint8_t *)&payloadLen, sizeof(payloadLen));
    if (payloadLen > 0)
      f.write((const uint8_t *)payload, payloadLen);

    uint8_t retain = msg->isRetain() ? 1 : 0;
    f.write(retain);
  } else if (type == SUBSCRIPTION) {
    HASubscribCmd *sub = (HASubscribCmd *)pAction;
    uint8_t isSub = sub->isSubscribtion() ? 1 : 0;
    f.write(isSub);
  }
  f.close();
}

void HANode::swapInFromFlash() {
  if (!mSwapFileExists)
    return;

  File f = LittleFS.open(SWAP_FILE_PATH, "r+");
  if (!f)
    return;

  int loadedCount = 0;
  bool hasMoreUnread = false;

  while (f.available()) {
    size_t startPos = f.position();
    uint8_t status;
    f.read(&status, 1);
    uint8_t type;
    f.read(&type, 1);
    uint16_t topicLen;
    f.read((uint8_t *)&topicLen, sizeof(topicLen));

    // If already processed, skip to the next record
    if (status == 0x00) {
      f.seek(topicLen, SeekCur);
      if (type == POSTMESSAGE) {
        uint16_t payloadLen;
        f.read((uint8_t *)&payloadLen, sizeof(payloadLen));
        f.seek(payloadLen + 1, SeekCur);
      } else if (type == SUBSCRIPTION) {
        f.seek(1, SeekCur);
      }
      continue;
    }

    // We found an unread message, check if RAM is full
    if (loadedCount >= 5) {
      hasMoreUnread = true;
      break; // Stop loading, leave the rest for the next Swap-In
    }

    // Load Topic
    char *topic = NULL;
    if (topicLen > 0) {
      topic = (char *)malloc(topicLen + 1);
      f.read((uint8_t *)topic, topicLen);
      topic[topicLen] = '\0';
    }

    // Load Payload and Re-instantiate Object in RAM
    if (type == POSTMESSAGE) {
      uint16_t payloadLen;
      f.read((uint8_t *)&payloadLen, sizeof(payloadLen));
      char *payload = NULL;
      if (payloadLen > 0) {
        payload = (char *)malloc(payloadLen + 1);
        f.read((uint8_t *)payload, payloadLen);
        payload[payloadLen] = '\0';
      }
      uint8_t retain;
      f.read(&retain, 1);
      mOutboxAction.append(new HAMessage(topic, payload, retain > 0));
      if (payload)
        free(payload);
    } else if (type == SUBSCRIPTION) {
      uint8_t isSub;
      f.read(&isSub, 1);
      mOutboxAction.append(new HASubscribCmd(topic, isSub > 0));
    }

    if (topic)
      free(topic);

    // Mark the action as Read (0x00) in Flash
    size_t endPos = f.position();
    f.seek(startPos, SeekSet);
    uint8_t processed = 0x00;
    f.write(processed);
    f.seek(endPos, SeekSet);

    loadedCount++;
  }

  f.close();

  // If file is fully processed, perform garbage collection
  if (!hasMoreUnread) {
    LittleFS.remove(SWAP_FILE_PATH);
    mSwapFileExists = false;
  }
}

HAAction *HANode::peekAction() {
  // If RAM buffer is empty, try to fetch the next batch from Flash
  if (mOutboxAction.getSize() == 0) {
    swapInFromFlash();
  }

  // Guard: If it is still empty after a swap attempt, return safely
  if (mOutboxAction.getSize() == 0) {
    return NULL;
  }

  // Returns the first item safely
  return mOutboxAction.get(0);
}

void HANode::popAction() {
  // Simply remove the pointer from the LinkedList.
  // Memory deletion is handled by HADevice::treatActions()
  mOutboxAction.shift();
}
} // namespace HALIB_NAMESPACE