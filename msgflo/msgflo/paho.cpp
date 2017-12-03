#include "debug.h"
#include "paho.h"

Paho::Paho(const string& pahoDllPath) {
  trace();

  dbg("Loading %s ...\n", pahoDllPath.c_str());
  hDll_ = LoadLibrary(pahoDllPath.c_str());

  if (hDll_)
    printf("%s loaded to: 0x%p\n", pahoDllPath.c_str(), hDll_);
  else
    printf("Failed loading %s\n", pahoDllPath.c_str());

  pSetCallBacksFunc_      = reinterpret_cast<MqttClientSetCallBacks_t*>(GetProcAddress(hDll_,   "MQTTClient_setCallbacks"));
  pGetVersionInfoFunc_    = reinterpret_cast<MqttClientGetVersionInfo_t*>(GetProcAddress(hDll_, "MQTTClient_getVersionInfo"));
  pClientCreateFunc_      = reinterpret_cast<MqttClientCreate_t*>(GetProcAddress(hDll_,         "MQTTClient_create"));
  pClientConnectFunc_     = reinterpret_cast<MqttClientConnect_t*>(GetProcAddress(hDll_,        "MQTTClient_connect"));
  pClientDisconnectFunc_  = reinterpret_cast<MqttClientDisconnect_t*>(GetProcAddress(hDll_,     "MQTTClient_disconnect"));
  pClientIsConnectedFunc_ = reinterpret_cast<MqttClientIsConnected_t*>(GetProcAddress(hDll_,    "MQTTClient_isConnected"));
  pClientPublish_         = reinterpret_cast<MqttClientPublish_t*>(GetProcAddress(hDll_,        "MQTTClient_publish"));
  pClientPublishMessage_  = reinterpret_cast<MqttClientPublishMessage_t*>(GetProcAddress(hDll_, "MQTTClient_publishMessage"));
  pClientDestroy_         = reinterpret_cast<MqttClientDestroy_t*>(GetProcAddress(hDll_,        "MQTTClient_destroy"));

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

  MQTTClient_nameValue* pName = pGetVersionInfoFunc_();
  printf("MQTT Versoin info: Name: '%s', '%s'\n", pName->name, pName->value);
}

bool Paho::connect(const char* pBrokerHostName, const char* pClientId) {
  trace();

  MQTTClient_willOptions will = MQTTClient_willOptions_initializer;
  will.topicName = "werkstatt/c_nancy/online"; // TODO: add param to connect function instead of hard coding
  will.message = "false";
  will.retained = true;
  will.qos = 1;

  MQTTClient_connectOptions opts;
  opts.struct_id[0] = 'M';
  opts.struct_id[1] = 'Q';
  opts.struct_id[2] = 'T';
  opts.struct_id[3] = 'C';
  opts.struct_version = 5;
  opts.keepAliveInterval = 60;
  opts.cleansession = 1;
  opts.reliable = 1;
  opts.will = &will;
  opts.username = nullptr;
  opts.password = nullptr;
  opts.connectTimeout = 30;
  opts.retryInterval = 20;
  opts.ssl = nullptr;
  opts.serverURIcount = 0;
  opts.serverURIs = nullptr;
  opts.MQTTVersion = 0;
  opts.returned.serverURI = nullptr;
  opts.returned.MQTTVersion = 0;
  opts.returned.sessionPresent = 0;
  opts.binarypwd.len = 0;
  opts.binarypwd.data = nullptr;

  opts.keepAliveInterval = 20;
  opts.cleansession = 1;

  if (int error = pClientCreateFunc_(&hMqttClient_, pBrokerHostName, pClientId, 1, nullptr))
    return false;

  auto connLost = [](void* pContext, char* pCause) -> void {
    dbg("Lost connection to MQTT!\n");
  };

  auto msgArrived = [](void* pContext, char* pTopicName, int topicLen, MQTTClient_message* pMessage) -> int {
    dbg("Message arrived on MQTT!\n");

    return 0;
  };

  auto msgDeliveryComplete = [](void* pContext, MQTTClient_deliveryToken dt) -> void {
    dbg("Message delivery complete on MQTT!\n");
  };

  if (int error = pSetCallBacksFunc_(hMqttClient_, nullptr, connLost, msgArrived, msgDeliveryComplete))
    dbg("Failed setting callbacks: %d\n", error);
  else
    dbg("Callbacks set\n");

  if (int error = pClientConnectFunc_(hMqttClient_, &opts))
    return false;

  return true;
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

  MQTTClient_message msg;
  msg.struct_id[0] = 'M';
  msg.struct_id[1] = 'Q';
  msg.struct_id[2] = 'T';
  msg.struct_id[3] = 'M';
  msg.struct_version = 0;
  msg.payloadlen = len;
  msg.payload = const_cast<void*>(pPayload);
  msg.qos = qos;
  msg.retained = retain;
  msg.dup = 0;
  msg.msgid = 0;

  MQTTClient_deliveryToken dt;
  if (int error = pClientPublishMessage_(hMqttClient_, topic.c_str(), &msg, &dt))
    return false;

  return true;
}

Paho::~Paho() {
  trace();
}
