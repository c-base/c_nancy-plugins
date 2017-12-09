#include "debug.h"
#include "paho.h"

Paho::Paho(const string& pahoDllPath) {
  trace();

  dbg("Loading %s ...\n", pahoDllPath.c_str());
  hDll_ = LoadLibrary(pahoDllPath.c_str());

  if (hDll_)
    dbg("%s loaded to: 0x%p\n", pahoDllPath.c_str(), hDll_);
  else {
    dbg("Failed loading %s\n", pahoDllPath.c_str());
    return;
  }

  pSetCallBacksFunc_         = reinterpret_cast<MqttClientSetCallBacks_t*>(GetProcAddress(hDll_,      "MQTTClient_setCallbacks"));
  pGetVersionInfoFunc_       = reinterpret_cast<MqttClientGetVersionInfo_t*>(GetProcAddress(hDll_,    "MQTTClient_getVersionInfo"));
  pClientCreateFunc_         = reinterpret_cast<MqttClientCreate_t*>(GetProcAddress(hDll_,            "MQTTClient_create"));
  pClientConnectFunc_        = reinterpret_cast<MqttClientConnect_t*>(GetProcAddress(hDll_,           "MQTTClient_connect"));
  pClientDisconnectFunc_     = reinterpret_cast<MqttClientDisconnect_t*>(GetProcAddress(hDll_,        "MQTTClient_disconnect"));
  pClientIsConnectedFunc_    = reinterpret_cast<MqttClientIsConnected_t*>(GetProcAddress(hDll_,       "MQTTClient_isConnected"));
  pClientPublish_            = reinterpret_cast<MqttClientPublish_t*>(GetProcAddress(hDll_,           "MQTTClient_publish"));
  pClientPublishMessage_     = reinterpret_cast<MqttClientPublishMessage_t*>(GetProcAddress(hDll_,    "MQTTClient_publishMessage"));
  pClientDestroy_            = reinterpret_cast<MqttClientDestroy_t*>(GetProcAddress(hDll_,           "MQTTClient_destroy"));
  pClientWaitForCompletion_  = reinterpret_cast<MqttClientWaitForCompletion_t*>(GetProcAddress(hDll_, "MQTTClient_waitForCompletion"));

  auto printFuncAddr = [](const char* pFuncName, const void* pFunc) {
    printf("%s: 0x%p\n", pFuncName, pFunc);
  };

  printFuncAddr("MQTTClient_setCallBacks", pSetCallBacksFunc_);
  printFuncAddr("MQTTClient_getVersionInfo", pGetVersionInfoFunc_);
  printFuncAddr("MQTTClient_create", pClientCreateFunc_);
  printFuncAddr("MQTTClient_connect", pClientConnectFunc_);
  printFuncAddr("MQTTClient_disconnect", pClientDisconnectFunc_);
  printFuncAddr("MQTTClient_isConnected", pClientIsConnectedFunc_);
  printFuncAddr("MQTTClient_publish", pClientPublish_);
  printFuncAddr("MQTTClient_publishMessage", pClientPublishMessage_);
  printFuncAddr("MQTTClient_destroy", pClientDestroy_);
  printFuncAddr("MQTTClient_waitForCompletion", pClientWaitForCompletion_);

  MQTTClient_nameValue* pName = pGetVersionInfoFunc_();
  printf("MQTT Version info: Name: '%s', '%s'\n", pName->name, pName->value);
}

void Paho::onConnectionLost(void* pContext, char* pCause) {
  Paho* pThis = static_cast<Paho*>(pContext);

  dbg("Lost connection to MQTT!\n");
}

int Paho::onMsgArrived(void* pContext, char* pTopicName, int topicLen, MQTTClient_message* pMessage) {
  Paho* pThis = static_cast<Paho*>(pContext);

  dbg("Message arrived on MQTT!\n");

  return 0;
}

void Paho::onMsgDeliveryComplete(void* pContext, MQTTClient_deliveryToken dt) {
  Paho* pThis = static_cast<Paho*>(pContext);

  dbg("Message delivery complete on MQTT!\n");
}

bool Paho::connect(const string& brokerHostName, const string& clientId, const string& lastWillTopic,
    const void* pLastWillMsg, int msgLen) {
  trace();

  if (int error = pClientCreateFunc_(&hMqttClient_, brokerHostName.c_str(), clientId.c_str(), 1, nullptr))
    return false;

  if (int error = pSetCallBacksFunc_(hMqttClient_, this, onConnectionLost, onMsgArrived, onMsgDeliveryComplete))
    dbg("Paho::connect; Failed setting callbacks: %d\n", error);
  else
    dbg("Paho::connect; Callbacks set\n");

  MQTTClient_willOptions will = MQTTClient_willOptions_initializer;
  will.topicName = lastWillTopic.c_str();
  will.payload.data = pLastWillMsg;
  will.payload.len = msgLen;
  will.retained = true;
  will.qos = 1;

  MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
  opts.keepAliveInterval = 20;
  opts.cleansession = 1;
  opts.will = &will;

  if (int error = pClientConnectFunc_(hMqttClient_, &opts))
    return false;

  return true;
}

bool Paho::connect(const string& brokerHostName, const string& clientId) {
  trace();

  return connect(brokerHostName, clientId, "", nullptr, 0);
}

bool Paho::disconnect() {
  trace();

  if (int error = pClientDisconnectFunc_(hMqttClient_, 10000))
    return false;

  return true;
}

bool Paho::isConnected() {
  // trace()

  return pClientIsConnectedFunc_(hMqttClient_);
}

bool Paho::publish(const string& topic, const void* pPayload, int len, int qos, bool retain) {
  // trace();

  MQTTClient_message msg = MQTTClient_message_initializer;
  msg.payloadlen = len;
  msg.payload = const_cast<void*>(pPayload);
  msg.qos = qos;
  msg.retained = retain;

  MQTTClient_deliveryToken dt;
  if (int error = pClientPublishMessage_(hMqttClient_, topic.c_str(), &msg, &dt))
    return false;

  if (int error = pClientWaitForCompletion_(hMqttClient_, dt, 3000))
    return false;

  return true;
}

Paho::~Paho() {
  trace();
}
