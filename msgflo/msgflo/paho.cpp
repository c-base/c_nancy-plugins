#include "debug.h"
#include "paho.h"

Paho::Paho(const char* pPahoDllPath) {
  trace();

  dbg("Loading %s ...\n", pPahoDllPath);
  hDll_ = LoadLibrary(pPahoDllPath);

  if (hDll_)
    printf("%s loaded to: 0x%p\n", pPahoDllPath, hDll_);
  else
    printf("Failed loading %s\n", pPahoDllPath);

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

  MQTTClient_connectOptions opts;
  opts.struct_id[0] = 'M';
  opts.struct_id[1] = 'Q';
  opts.struct_id[2] = 'T';
  opts.struct_id[3] = 'C';
  opts.struct_version = 5;
  opts.keepAliveInterval = 60;
  opts.cleansession = 1;
  opts.reliable = 1;
  opts.will = nullptr;
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

bool Paho::publish(const char* pTopic, const void* pPayload, int len, int qos, bool retain) {
  trace();

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
  if (int error = pClientPublishMessage_(hMqttClient_, pTopic, &msg, &dt))
    return false;

  return true;
}

Paho::~Paho() {
  trace();
}
