#include <Arduino.h>
#include <unity.h>

#include <HALib.h>

using namespace HALIB_NAMESPACE;
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
    TEST_ASSERT_TRUE_MESSAGE(pNode->onHAMessage(mqttTopic, (const byte*)"ON", ((const unsigned int)2)),"action topic do not work");
    delete mqttTopic;
    TEST_ASSERT_FALSE(pNode->onHAMessage("ezr/zer/zer", (const byte*)"ON", ((const unsigned int)2)));
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

void UC_OnHAMessage(void)
{
    WiFiClient espClient;
    uint32_t freeBefore = ESP.getFreeHeap();

    HADevice *p_MyDevice = new HADevice("portal");
    p_MyDevice->setup(espClient, "localhost", 8383);

    HAAdapterLed *p_Adapter = new HAAdapterLed("MyButton", D0);
    p_Adapter->setup();
    p_Adapter->setDevice(p_MyDevice);

    const char *c3Root = p_Adapter->m_pComponent->getRootTopic();
    const char *c3Act = p_Adapter->m_pComponent->getPropertyValue(PROP_COMMAND_TOPIC);

    char *mqttTopic = (char *)malloc(strlen(c3Root) + strlen(c3Act + 1) + 1);
    strcpy(mqttTopic, c3Root);
    strcat(mqttTopic, c3Act + 1);


    p_MyDevice->onMQTTMessage(mqttTopic, (unsigned char*) "ON", (unsigned int) 2);
    delete (mqttTopic);

    delete (p_Adapter);
    delete (p_MyDevice);

}

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
    
    RUN_TEST(UC_SETUP);
    RUN_TEST(UC_OnHAMessage);
    
    
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