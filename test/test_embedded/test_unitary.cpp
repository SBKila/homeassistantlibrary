#include <Arduino.h>
#include <unity.h>

#include "HALib/HALib.h"
#include "HALib/tools/BufferFIFO.hpp"

using namespace HALIB_NAMESPACE;

void UT_BufferFIFO(void)
{
    uint32_t freeBefore = ESP.getFreeHeap();
    BufferFIFO<int>* buffer;
    buffer = new BufferFIFO<int>(5);
    int val=1;
    int overload=1;
    TEST_ASSERT_EQUAL(NULL,buffer->push(val++));
    TEST_ASSERT_EQUAL(NULL,buffer->push(val++));
    TEST_ASSERT_EQUAL(NULL,buffer->push(val++));
    TEST_ASSERT_EQUAL(NULL,buffer->push(val++));
    TEST_ASSERT_EQUAL(NULL,buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    TEST_ASSERT_EQUAL(overload++,*buffer->push(val++));
    delete buffer;
    int32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");

}

void UT_LinkedList(void)
{
    HAComponent hAComponent1("Device1", BINARY_SENSOR);
    HAComponent hAComponent10("Device1", BINARY_SENSOR);
    HAComponent hAComponent2("Device2", BINARY_SENSOR);
    HAComponent hAComponent3("Device3", BINARY_SENSOR);
    HAComponent hAComponent4("Device4", BINARY_SENSOR);

    uint32_t freeBefore = ESP.getFreeHeap();

    LinkedList<HAComponent *> linkedList;
    TEST_ASSERT_EQUAL_INT(0, linkedList.getSize());

    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent1), "append should return true");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent2), "append hAComponent2 should return true");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent3), "append hAComponent3 should return true");
    TEST_ASSERT_EQUAL_INT(3, linkedList.getSize());

    // test find by address
    TEST_MESSAGE("test find by address");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, linkedList.find(&hAComponent2, false), "hAComponent2 should be found at position 1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, linkedList.find(&hAComponent3, false), "hAComponent3 should be found at position 2");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, linkedList.find(&hAComponent1, false), "hAComponent1 should be found at position 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, linkedList.find(&hAComponent4, false), "hAComponent4 should not be found");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, linkedList.find(&hAComponent10, false), "hAComponent10 should not be found");

    // test find by contant
    TEST_MESSAGE("test find by contant");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, linkedList.find(&hAComponent2, true), "hAComponent2 should be found at position 1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, linkedList.find(&hAComponent3, true), "hAComponent3 should be found at position 2");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, linkedList.find(&hAComponent1, true), "hAComponent1 should be found at position 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, linkedList.find(&hAComponent4, true), "hAComponent4 should not be found");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, linkedList.find(&hAComponent10, true), "hAComponent10 should be found at position 0");

    // test get by index
    TEST_MESSAGE("test get by index");
    HAComponent *item = NULL;
    item = linkedList.get(1);
    TEST_ASSERT_NOT_NULL_MESSAGE(item, "one element should be at position 1");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent2, item, "position 1 should be hAComponent2");
    item = linkedList.get(2);
    TEST_ASSERT_NOT_NULL_MESSAGE(item, "one element should be at position 2");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent3, item, "position 2 should be hAComponent3");
    item = linkedList.get(0);
    TEST_ASSERT_NOT_NULL_MESSAGE(item, "one element should be at position 0");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent1, item, "position 0 should be hAComponent1");

    // test remove by address
    TEST_MESSAGE("test remove by address");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.remove(&hAComponent2), "should be able to mid items");
    TEST_ASSERT_EQUAL_INT(2, linkedList.getSize());
    item = linkedList.get(0);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent1, item, "hAComponent1 should be at position 0");
    item = linkedList.get(1);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent3, item, "hAComponent3 should be at position 1");

    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent2), "append hAComponent2 should return true");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.remove(&hAComponent2), "should be able last item");
    TEST_ASSERT_EQUAL_INT(2, linkedList.getSize());
    item = linkedList.get(0);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent1, item, "hAComponent1 should be at position 0");
    item = linkedList.get(1);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent3, item, "hAComponent3 should be at position 1");

    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent2), "append hAComponent2 should return true");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.remove(&hAComponent1), "should be able root item");
    TEST_ASSERT_EQUAL_INT(2, linkedList.getSize());
    item = linkedList.get(0);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent3, item, "hAComponent3 should be at position 0");
    item = linkedList.get(1);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent2, item, "hAComponent2 should be at position 1");

    // test remove by content
    TEST_MESSAGE("test remove by contant");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.append(&hAComponent1), "append hAComponent2 should return true");
    TEST_ASSERT_TRUE_MESSAGE(linkedList.remove(&hAComponent10, true), "should be able root item");
    TEST_ASSERT_EQUAL_INT(2, linkedList.getSize());
    item = linkedList.get(0);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent3, item, "hAComponent3 should be at position 0");
    item = linkedList.get(1);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(&hAComponent2, item, "hAComponent2 should be at position 1");

    linkedList.~LinkedList();
    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HANode_constructor(void)
{

    char node1Name[] = "DEVICE1";
    char node2Name[] = "DEVICE2";
    char node1DeviceInfo[] = "{\"mf\":\"MANUF\",\"mdl\":\"MODEL\",\"sw\":\"RELEASE\",\"name\":\"DEVICE1\",\"ids\":\"XXXXXXXX\"}";
    char node2DeviceInfo[] = "{\"mf\":\"MANUF\",\"mdl\":\"MODEL\",\"name\":\"DEVICE2\",\"ids\":\"XXXXXXXX\"}";

    uint32_t freeBefore = ESP.getFreeHeap();

    TEST_MESSAGE("test constructor");
    HANode *pNode = new HANode(node1Name);
    HANode *pNode2 = new HANode(node2Name);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE1", node1Name, "Extern data modified by constructor");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE2", node2Name, "Extern data modified by constructor");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE1", pNode->getName(), "Wrong HANode name");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE2", pNode2->getName(), "Wrong HANode name");

    TEST_MESSAGE("test generated id");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(pNode->getId(), pNode2->getId(), "Id should be different");

    TEST_MESSAGE("test device info");
    pNode->setDeviceInfo("MANUF", "MODEL", "RELEASE");
    sprintf(node1DeviceInfo + 67, "%08X\"}", pNode->getId());
    TEST_ASSERT_EQUAL_STRING(node1DeviceInfo, pNode->getProperty(PROP_DEVICE));
    pNode2->setDeviceInfo("MANUF", "MODEL", NULL);
    sprintf(node2DeviceInfo + 52, "%08X\"}", pNode2->getId());
    TEST_ASSERT_EQUAL_STRING(node2DeviceInfo, pNode2->getProperty(PROP_DEVICE));

    delete pNode2;
    delete pNode;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HADevice_constructor(void)
{
    char deviceName[] = "DEVICE1";
    WiFiClient espClient;
    uint32_t freeBefore = ESP.getFreeHeap();

    HADevice *pDevice = new HADevice(deviceName, "MANUF", "MODEL", "RELEASE");
    pDevice->setup(espClient, "localhost", 8383);
    delete pDevice;
    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

void UT_HAComponentProperty(void)
{

    uint32_t freeBefore = ESP.getFreeHeap();

    HAComponentProperty *pProperty1 = new HAComponentProperty(PROP_STATE_TOPIC, "/state");
    HAComponentProperty *pProperty2 = new HAComponentProperty(PROP_STATE_TOPIC, "/toto");
    HAComponentProperty *pProperty3 = new HAComponentProperty(PROP_STATE_ON, "/toto");

    TEST_MESSAGE("Test getName");
    //TEST_MESSAGE(pProperty1->getName());
    // TEST_ASSERT_EQUAL_STRING_MESSAGE(DiscoveryMessageKeyLabel[PROP_STATE_TOPIC],pProperty1->getName(),"wrong name");
    // TEST_ASSERT_EQUAL_STRING_MESSAGE(DiscoveryMessageKeyLabel[PROP_STATE_TOPIC],pProperty2->getName(),"wrong name");
    // TEST_ASSERT_EQUAL_STRING_MESSAGE(DiscoveryMessageKeyLabel[PROP_STATE_ON],pProperty3->getName(),"wrong name");

    TEST_MESSAGE("Test getKey");
    // TEST_ASSERT_EACH_EQUAL_INT(PROP_STATE_TOPIC,pProperty1->getKey(),"wrong key");

    TEST_MESSAGE("Test getValue");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("/state", pProperty1->getValue(), "wrong value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("/toto", pProperty2->getValue(), "wrong value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("/toto", pProperty3->getValue(), "wrong value");

    TEST_MESSAGE("Test operators");
    TEST_ASSERT_TRUE_MESSAGE((*pProperty1 == *pProperty2), "== operator should be true");
    TEST_ASSERT_FALSE_MESSAGE((*pProperty1 == *pProperty3), "== operator should be false");
    TEST_ASSERT_FALSE_MESSAGE((*pProperty1 != *pProperty2), "== operator should be true");
    TEST_ASSERT_TRUE_MESSAGE((*pProperty1 != *pProperty3), "== operator should be false");

    TEST_MESSAGE("Test getJson");
    size_t length = pProperty1->getJson();
    char *json = (char *)malloc(length + 1);
    json[length] = 0;
    pProperty1->getJson(&json);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("\"stat_t\":\"/state\"", json, "wrong json");
    free(json);

    delete pProperty1;
    delete pProperty2;
    delete pProperty3;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

void UT_HAComponent_constructor(void)
{

    char component1Name[] = "DEVICE1";
    char component2Name[] = "DEVICE2";
    char refTopic[] = "homeassistant/binary_sensor/portal-DEVICE1-FFFFFFFF";

    uint32_t freeBefore = ESP.getFreeHeap();

    TEST_MESSAGE("test constructor");
    HAComponent *pHAComponent1 = new HAComponent(component1Name, BINARY_SENSOR);
    HAComponent *pHAComponent2 = new HAComponent(component2Name, BINARY_SENSOR);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE1", component1Name, "Extern data modified by constructor");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("DEVICE1", pHAComponent1->getName(), "Wrong HAComponent name");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(pHAComponent1->getId(), pHAComponent2->getId(), "component id should be different");

    TEST_MESSAGE("test root topic");
    sprintf(refTopic + strlen(refTopic) - 8, "%08X", pHAComponent1->getId());
    char *topic = pHAComponent1->buildRootTopic("portal");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(refTopic, topic, "wrong root in topic");
    free(topic);

    HANode *pNode = new HANode("portal");
    pHAComponent1->setNode(pNode);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(refTopic, pHAComponent1->getPropertyValue(PROP_ROOT_TOPIC), "wrong property ~");

    delete pNode;

    delete pHAComponent2;
    delete pHAComponent1;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HAComponentDeviceTrigger(void)
{

    char component1Name[] = "DEVICE1";
    uint32_t freeBefore = ESP.getFreeHeap();
    TEST_MESSAGE("test constructor");
    HAComponentDeviceTrigger *pDeviceTriger = new HAComponentDeviceTrigger(component1Name);

    delete (pDeviceTriger);
    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

void UT_HAComponent_buildDiscoveryTopic(void)
{
    char component1Name[] = "DEVICE1";
    uint32_t freeBefore = ESP.getFreeHeap();

    HAComponent *pHAComponent1 = new HAComponent(component1Name, BINARY_SENSOR);

    char *topic = pHAComponent1->buildDiscoveryTopic("discovery_prefix", "portal");
    // TEST_MESSAGE(topic);
    char *referenceId = (char *)malloc(9 * sizeof(char));
    memset(referenceId, 0, 9 * sizeof(char));
    sprintf(referenceId, "%08X", pHAComponent1->getId());
    // TEST_MESSAGE(referenceId);
    topic[strlen(topic) - 7] = 0;

    TEST_ASSERT_EQUAL_STRING_MESSAGE(referenceId, topic + strlen(topic) - 8, "wrong deviceid in topic");
    delete referenceId;

    topic[strlen(topic) - 8] = 0;
    //TEST_MESSAGE(topic);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("discovery_prefix/binary_sensor/portal/DEVICE1-", topic, "postfix of deviceid not the device name");

    free(topic);

    delete pHAComponent1;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HAComponent_GenerateId(void)
{
    char component1Name[] = "DEVICE1";
    char component2Name[] = "DEVICE2";
    uint32_t freeBefore = ESP.getFreeHeap();

    HAComponent *pHAComponent1 = new HAComponent(component1Name, BINARY_SENSOR);
    uint32_t id1 = pHAComponent1->getId();
    HAComponent *pHAComponent2 = new HAComponent(component1Name, BINARY_SENSOR);
    uint32_t id2 = pHAComponent2->getId();
    HAComponent *pHAComponent3 = new HAComponent(component2Name, BINARY_SENSOR);
    uint32_t id3 = pHAComponent3->getId();
    HAComponent *pHAComponent4 = new HAComponent(component1Name, SWITCH);
    uint32_t id4 = pHAComponent4->getId();

    TEST_ASSERT_EQUAL(id1, id2);
    TEST_ASSERT_NOT_EQUAL(id1, id4);
    TEST_ASSERT_NOT_EQUAL(id1, id3);

    delete pHAComponent1;
    delete pHAComponent2;
    delete pHAComponent3;
    delete pHAComponent4;
    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HAComponent_GetTopicPath(void)
{

    char component1Name[] = "DEVICE1";
    char expextedTopic[] = "homeassistant/binary_sensor/testnode-DEVICE1-XXXXXXXX/osccmd";
    char referenceId[] = "XXXXXXXX";

    char *topic = NULL;
    uint32_t freeBefore = ESP.getFreeHeap();
    size_t size = 0;

    HAComponent *pHAComponent1 = new HAComponent(component1Name, BINARY_SENSOR);

    pHAComponent1->addProperty(PROP_OSCILLATION_COMMAND_TOPIC);
    size = pHAComponent1->getTopicPath(PROP_OSCILLATION_COMMAND_TOPIC);
    topic = (char *)calloc(size + 1, sizeof(char));
    pHAComponent1->getTopicPath(PROP_OSCILLATION_COMMAND_TOPIC, topic);
    TEST_ASSERT_EQUAL_STRING("/osccmd", topic);
    free(topic);

    HANode *pNode = new HANode("testnode");
    pNode->addComponent(pHAComponent1);
    size = pHAComponent1->getTopicPath(PROP_OSCILLATION_COMMAND_TOPIC);
    topic = (char *)calloc(size + 1, sizeof(char));
    pHAComponent1->getTopicPath(PROP_OSCILLATION_COMMAND_TOPIC, topic);
    sprintf(referenceId, "%08X", pHAComponent1->getId());
    memcpy(expextedTopic + 45, referenceId, 8);
    TEST_ASSERT_EQUAL_STRING(expextedTopic, topic);

    free(topic);
    delete pNode;
    delete pHAComponent1;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_HAUtils_generateId(void)
{
    char component10Name[] = "Pompes_instant";
    char component20Name[] = "Lavage_cumulat";
    char component11Name[] = "Pompes_cumulat";
    char component21Name[] = "Lavage_instant";
    
    uint32_t id10 = HAUtils::generateId(component10Name,SENSOR);
    uint32_t id100 = HAUtils::generateId(component10Name,SENSOR);
    uint32_t id101 = HAUtils::generateId(component10Name,SENSOR);
    uint32_t id102 = HAUtils::generateId(component10Name,SENSOR);
    uint32_t id103 = HAUtils::generateId(component10Name,BINARY_SENSOR);

    uint32_t id20 = HAUtils::generateId(component20Name,SENSOR);
    uint32_t id11 = HAUtils::generateId(component11Name,SENSOR);
    uint32_t id21 = HAUtils::generateId(component21Name,SENSOR);
    
    TEST_ASSERT_FALSE_MESSAGE(id10 == id20,"Pompes_instant = Lavage_cumulat");
    TEST_ASSERT_FALSE_MESSAGE(id11 == id21,"Pompes_instant = Pompes_cumulat");
    TEST_ASSERT_FALSE_MESSAGE(id10 == id103,"Pompes_instant with different type");
    TEST_ASSERT_FALSE_MESSAGE(id10 != id100,"Pompes_instant should be same") ;
    TEST_ASSERT_FALSE_MESSAGE(id10 != id101,"Pompes_instant should be same");
    TEST_ASSERT_FALSE_MESSAGE(id10 != id102,"Pompes_instant should be same");
    
}

int callbackCalled = 0;
void resetCallbackCall()
{
    callbackCalled = 0;
}
void callback()
{
    callbackCalled++;
}
void basicLigthPowerOn(boolean powerOn)
{
    callbackCalled++;
}

#define HA_CALLBACK std::function<void()>

void UT_HAComponent_DiscoveryMessage(void)
{
    char component1Name[] = "DEVICE1";
    char refDisconeryMsgTemplate[] = "{\"name\":\"DEVICE1\",\"uniq_id\":\"%08X\",\"~\":\"homeassistant/binary_sensor/portal-DEVICE1-%08X\",\"cmd_off_tpl\":\"OFF\",\"cmd_on_tpl\":\"ON\",\"cmd_t\":\"~/cmd\",\"stat_t\":\"homeassistant/toto\"}";

    uint32_t freeBefore = ESP.getFreeHeap();

    HAComponent *pHAComponent1 = new HAComponent(component1Name, BINARY_SENSOR);
    char *refDisconeryMsg = (char *)malloc(strlen(refDisconeryMsgTemplate) - 4 + 8 - 4 + 8 + 1);
    sprintf(refDisconeryMsg, refDisconeryMsgTemplate, pHAComponent1->getId(), pHAComponent1->getId());
    HANode *pNode = new HANode("portal");
    pHAComponent1->setNode(pNode);

    HAComponentPropertyAction<HA_CALLBACK> *l_pAction = new HAComponentPropertyAction<HA_CALLBACK>(PROP_COMMAND_TOPIC, callback);
    pHAComponent1->addAction(l_pAction);
    l_pAction = new HAComponentPropertyAction<HA_CALLBACK>(PROP_STATE_TOPIC, callback, "homeassistant/toto");
    pHAComponent1->addAction(l_pAction);
    pHAComponent1->addProperty(PROP_COMMAND_OFF_TEMPLATE, "OFF");
    pHAComponent1->addProperty(PROP_COMMAND_ON_TEMPLATE, "ON");

    char *discovery = pHAComponent1->buildDiscoveryMessage();
    TEST_ASSERT_EQUAL_STRING(refDisconeryMsg, discovery);
    free(discovery);

    free(refDisconeryMsg);

    delete pHAComponent1;
    delete pNode;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}
void UT_OnHAMessage(void)
{
    uint32_t freeBefore = ESP.getFreeHeap();

    HANode *pNode = new HANode("portal");

    HAComponent *pHAComponent3 = new HAComponentLigthBasic("DEVICE3", [](boolean powerOn) { callbackCalled++; });
    pNode->addComponent(pHAComponent3);

    resetCallbackCall();
    TEST_ASSERT_FALSE(pNode->onHAMessage("toto", NULL, (unsigned int)0));
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, callbackCalled, "No callback should ben called");

    const char *c3Root = pHAComponent3->getRootTopic();
    const char *c3Act = pHAComponent3->getPropertyValue(PROP_COMMAND_TOPIC);

    char *mqttTopic = (char *)malloc(strlen(c3Root) + strlen(c3Act + 1) + 1);
    strcpy(mqttTopic, c3Root);
    strcat(mqttTopic, c3Act + 1);
    //TEST_MESSAGE(mqttTopic);
    TEST_ASSERT_TRUE_MESSAGE(pNode->onHAMessage(mqttTopic, (const byte *)"ON", ((const unsigned int)2)), "action topic do not work");
    delete mqttTopic;
    TEST_ASSERT_FALSE(pNode->onHAMessage("ezr/zer/zer", (const byte *)"ON", ((const unsigned int)2)));
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, callbackCalled, "Only 1 callback should ben called");

    delete pHAComponent3;
    delete pNode;

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

void UT_HAAdapter_constructor(void)
{

    char name[] = "SENSOR1";

    uint32_t freeBefore = ESP.getFreeHeap();
    HAAdapter *pAdapter = new HAAdapterPushBtn(name, D1);

    pAdapter->setup();

    delete (pAdapter);
    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

class DDS328Mock
{

public:
    void save()
    {
        savePerformed = true;
    }

private:
    boolean savePerformed = false;
};

void UC_SETUP(void)
{
    WiFiClient espClient;

    uint32_t freeBefore = ESP.getFreeHeap();

    HADevice *p_MyDevice = new HADevice("MyDevice4Creation");
    p_MyDevice->setup(espClient, "localhost", 8383);

    HAAdapter *p_Adapter = new HAAdapterPushBtn("MyButton", D1);
    p_Adapter->setup();
    p_Adapter->setDevice(p_MyDevice);

    DDS328Mock *pDSS238PesistantMock = new DDS328Mock();
    HAAdapter *p_Adapter2 = new HAAdapterDDS238("MyButton", D2, 1000, 220, 20, [pDSS238PesistantMock](DDS238Data data) { pDSS238PesistantMock->save(); });
    p_Adapter2->setup();
    p_Adapter2->setDevice(p_MyDevice);

    delete (pDSS238PesistantMock);
    delete (p_Adapter2);
    delete (p_Adapter);
    delete (p_MyDevice);

    uint32_t freeAfter = ESP.getFreeHeap();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
}

void UC_WIFICOnnect()
{
}

byte CONNECTACKOK[4] = {32, 2, 2, 0};
byte SUBACKOK[5] = {144, 3, 0, 0, 0};
class mockHAAdapterLed : public HAAdapterLed
{
public:
    mockHAAdapterLed(const char *name, uint8_t ioReference) : HAAdapterLed(name, ioReference)
    {
        m_poweron = false;
    }
    boolean m_poweron = false;
    virtual void powerOn(boolean powerOn)
    {
        m_poweron = powerOn;
    };
};
class mockClient : public Client
{

    byte *readBuffer = NULL;
    uint readBuffSize = 0;
    byte *writeBuffer = NULL;
    uint writeBuffSize = 0;
    byte nbWrittenMessage = 0;
    uint currentWriteIndex = 0;
    uint currentReadIndex = 0;
    uint isConnected = 0;

public:
    void setReadBuffer(byte *pBuff, uint pBuffSize)
    {
        readBuffer = pBuff;
        readBuffSize = pBuffSize;
        currentReadIndex = 0;
    }
    void setWriteBuffer(byte *pBuff, uint pBuffSize)
    {
        writeBuffer = pBuff;
        writeBuffSize = pBuffSize;
        currentWriteIndex = 1;
        nbWrittenMessage = 0;
    }

    virtual int connect(IPAddress ip, uint16_t port)
    {
        Serial.println("Wifi connect ip ");
        isConnected = (uint)1;
        return 1;
    }
    virtual int connect(const char *host, uint16_t port)
    {
        Serial.println("Wifi connect host");
        isConnected = 1;
        return 1;
    }
    virtual size_t write(uint8_t data)
    {
        Serial.print("Wifi Write ");
        Serial.println((char)data);
        if (currentWriteIndex < writeBuffSize)
        {
            writeBuffer[currentWriteIndex++] = (char)data;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    virtual size_t write(const uint8_t *buf, size_t size) override
    {
        Serial.print("Wifi Write (");
        Serial.print(size);
        Serial.print(") ");

        nbWrittenMessage++;
        writeBuffer[0] = nbWrittenMessage;

        byte MQTTControlPacketType = *buf;
        MQTTControlPacketType = MQTTControlPacketType >> 4;
        int qos = ((*buf) & 0x6) >> 1;
        Serial.print("type ");
        Serial.print(MQTTControlPacketType);
        Serial.print(" qos ");
        Serial.println(qos);

        if (MQTTControlPacketType == 3)
        {
            size_t offset = 1;
            Serial.print(" PUBLISH ");

            int remainingLength = *(buf + offset++);

            if ((remainingLength & 0x80) != 0)
            {
                remainingLength = remainingLength & 0X7F;
                remainingLength = *(buf + offset++) << 7 | remainingLength;
            }

            size_t topicSize = *(buf + offset++);
            topicSize = topicSize << 8;
            topicSize += *(buf + offset++);
            Serial.print("topic: ");
            for (size_t index = 0; index < topicSize; index++)
            {
                Serial.print(*((char *)(buf + offset++)));
            }
            if (qos != 0)
            {
                Serial.print(" Packet Identifier: ");
                topicSize = *(buf + offset++);
                topicSize = topicSize << 8;
                topicSize += *(buf + offset++);
                Serial.print(topicSize);
            }

            remainingLength -= 2 + topicSize;

            Serial.print(" message: ");
            for (; offset < size; offset++)
            {
                Serial.print(*((char *)(buf + offset)));
                remainingLength--;
            }

            if (remainingLength != 0)
            {
                Serial.println(remainingLength);
            }
            else
            {
                Serial.println();
            }
        }
        else if (MQTTControlPacketType == 8)
        {
            Serial.print(" SUBSCRIBE ");

            int packetId = ((*(buf + 2)) << 8) + *(buf + 3);
            SUBACKOK[2] = *(buf + 2);
            SUBACKOK[3] = *(buf + 3);

            Serial.print(" paketid : ");
            Serial.print(packetId);

            Serial.print(" topic : ");
            int offset = 4;
            int topicSize = ((*(buf + (offset+1))) << 8) + *(buf + offset+2);
            offset+=2;

            for (int index = 0; index < topicSize; index++)
            {
                Serial.print(*((char *)(buf + offset++)));
            }

            Serial.print(" qos : ");
            Serial.print(*(buf + offset));

            Serial.println("");
            setReadBuffer(SUBACKOK, 5);
        }
        else if (MQTTControlPacketType == 1)
        {
            Serial.print(" CONNECT ");
            for (size_t index = 0; index < 12; index++)
            {

                if (index < 4 || index > 8)
                {
                    Serial.print("(");
                    Serial.print(*(buf + index));
                    Serial.print(")");
                }
                else
                {
                    Serial.print(*((char *)(buf + index)));
                }
            }
            for (size_t index = 12; index < size; index++)
            {
                Serial.print(" | ");
                size_t dataSize = (*(buf + index++) << 4);
                dataSize += *(buf + index++);
                size_t dataEnd = index + dataSize;
                for (; (index < dataEnd) && (index < size); index++)
                {
                    Serial.print(*((char *)(buf + index)));
                }
                index--;
            }

            Serial.println("");

            setReadBuffer(CONNECTACKOK, 4);
        }
        else
        {
            for (size_t index = 0; index < size; index++)
            {
                Serial.print(*((char *)(buf + index)));
            }
            Serial.println();
        }

        if (currentWriteIndex + size < writeBuffSize)
        {
            writeBuffer[currentWriteIndex++] = (byte)(size >> 8);
            writeBuffer[currentWriteIndex++] = (byte)size;
            memcpy(writeBuffer + currentWriteIndex, buf, size);
            currentWriteIndex += size;
            return size;
        }
        else
        {
            return 0;
        }
    }
    virtual int available()
    {
        // Serial.print("Wifi available ");
        // Serial.println(readBuffSize - currentReadIndex);
        return readBuffSize - currentReadIndex;
    }
    virtual int read()
    {
        //Serial.println("Wifi read");
        return readBuffer[currentReadIndex++];
    }
    virtual int read(uint8_t *buf, size_t size)
    {
        Serial.println("Wifi Read buf");
        return 0;
    }
    virtual int peek()
    {
        Serial.println("Wifi peek");
        return 1;
    }
    virtual void flush()
    {
        Serial.println("Wifi flush");
        return;
    }
    virtual void stop()
    {
        Serial.println("Wifi stop");
        return;
    }
    virtual uint8_t connected()
    {
        // Serial.println("Wifi connected");
        return isConnected;
    }
    virtual operator bool()
    {
        return true;
    }
};
// void UC_StreamHAMessages(void)
// {
//     byte *wifiInputBuffer = (byte *)malloc(60);
//     byte *wifiOutputBuffer = (byte *)malloc(512);
//     mockClient espClient;
//     uint32_t freeBefore = ESP.getFreeHeap();

//     TEST_MESSAGE("LOOP 1");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     int messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);

//     HADevice *p_MyDevice = new HADevice("portal");
//     p_MyDevice->setup(espClient, "localhost", 8383);

//     HAAdapterPushBtn *p_Adapter = new HAAdapterPushBtn("MyButton", A0);
//     p_Adapter->setup();
//     p_Adapter->setDevice(p_MyDevice);

//     p_MyDevice->loop(WL_CONNECTED);
//     TEST_ASSERT_EQUAL_MESSAGE(2, wifiOutputBuffer[0], "2 messages expected");

//     int messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(1, wifiOutputBuffer[messageOffset] >> 4, "CONNECT message expected");
//     messageOffset += messageSize;

//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(3, wifiOutputBuffer[messageOffset] >> 4, "PUBLISH available message expected");
//     messageOffset += messageSize;

//     TEST_MESSAGE("LOOP 2");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);
//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();

//     // Check  message
//     TEST_ASSERT_EQUAL_MESSAGE(2, wifiOutputBuffer[0], "2 messages expected"); // it is a long message
//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(3, wifiOutputBuffer[messageOffset] >> 4, "PUBLISH discovery message expected");

//     TEST_MESSAGE("LOOP 3");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);

//     mockHAAdapterLed *p_Adapter2 = new mockHAAdapterLed("MyLed", A0);
//     p_Adapter2->setup();
//     p_Adapter2->setDevice(p_MyDevice);

//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();
//     p_Adapter2->loop();

//     // Check  message
//     TEST_ASSERT_EQUAL_MESSAGE(4, wifiOutputBuffer[0], "4 message expected"); // it is a long message

//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(8, wifiOutputBuffer[messageOffset] >> 4, "SUBSCRIBE message expected");
//     messageOffset += messageSize;

//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(3, wifiOutputBuffer[messageOffset] >> 4, "PUBLISH discovery message expected");
//     messageOffset += messageSize;

//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2]; // skip discovery info
//     messageOffset += messageSize+2;
    
//     messageSize = (wifiOutputBuffer[messageOffset+1] << 8) + wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(3, wifiOutputBuffer[messageOffset] >> 4, "PUBLISH status message expected");

//     TEST_MESSAGE("LOOP 4");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);
//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();
//     p_Adapter2->loop();

//     // check no message
//     TEST_ASSERT_EQUAL_MESSAGE(0, wifiOutputBuffer[0] >> 4, "No message expected");

    

//     TEST_MESSAGE("LOOP 5");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);
    
//     // TEST_MESSAGE("BTNPressed");
//     p_Adapter->onBtPressed();
    
//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();
//     p_Adapter2->loop();

//     // check submit message
//     messageSize = (wifiOutputBuffer[messageOffset+1]<<8)+ wifiOutputBuffer[messageOffset+2];
//     messageOffset+=2;
//     TEST_ASSERT_EQUAL_MESSAGE(3, wifiOutputBuffer[messageOffset] >> 4, "SUBMIT  message expected");
//     messageOffset+=messageSize;

//     TEST_MESSAGE("LOOP 6");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     messageOffset = 1;
//     memset(wifiOutputBuffer, 0, 512);
//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();
//     p_Adapter2->loop();

//     // check no message
//     TEST_ASSERT_EQUAL_MESSAGE(0, wifiOutputBuffer[0] >> 4, "No message expected");

//     memset(wifiInputBuffer, 0, 60);
//     wifiInputBuffer[0] = 48; // submit qos0 retain0 rplay0
//     wifiInputBuffer[1] = 49; // total length
//     wifiInputBuffer[2] = 0;
//     wifiInputBuffer[3] = 45;
//     memcpy(wifiInputBuffer + 4, "homeassistant/light/portal-MyLed-60909E20/cmd", 45);
//     memcpy(wifiInputBuffer + 4 + 45, "ON", 2);
//     espClient.setReadBuffer(wifiInputBuffer, 51);

//     // TEST_MESSAGE("LOOP 7");
//     espClient.setWriteBuffer(wifiOutputBuffer, 512);
//     memset(wifiOutputBuffer, 0, 512);
//     p_MyDevice->loop(WL_CONNECTED);
//     p_Adapter->loop();
//     p_Adapter2->loop();

//     TEST_ASSERT_EQUAL_MESSAGE(true, p_Adapter2->m_poweron, "power on callback should have een called");

//     delete (p_Adapter2);
//     delete (p_Adapter);
//     delete (p_MyDevice);

//     uint32_t freeAfter = ESP.getFreeHeap();
//     TEST_ASSERT_EQUAL_INT32_MESSAGE(freeBefore, freeAfter, "Memory leak");
//     free(wifiInputBuffer);
//     free(wifiOutputBuffer);
// }

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);

    delay(500);
    RUN_TEST(UT_BufferFIFO);
    RUN_TEST(UT_HAUtils_generateId);
    RUN_TEST(UT_HANode_constructor);
    RUN_TEST(UT_HADevice_constructor);
    RUN_TEST(UT_HAComponentProperty);
    RUN_TEST(UT_LinkedList);
    RUN_TEST(UT_HAComponent_constructor);
    RUN_TEST(UT_HAComponent_buildDiscoveryTopic);
    RUN_TEST(UT_HAComponent_GenerateId);
    RUN_TEST(UT_HAComponent_GetTopicPath);
    RUN_TEST(UT_HAComponent_DiscoveryMessage);
    RUN_TEST(UT_HAComponentDeviceTrigger);
    RUN_TEST(UT_HAAdapter_constructor);
    RUN_TEST(UT_OnHAMessage);
    // RUN_TEST(UC_SETUP);
    // RUN_TEST(UC_StreamHAMessages);

    UNITY_END(); // stop unit testing
}

void loop()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
}