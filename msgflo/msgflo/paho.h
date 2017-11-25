#ifndef _PAHO_H
#define _PAHO_H

#include <windows.h>
#include "MQTTClient.h"
#include "paho.h"

class Paho {
public:
  Paho(const char* pPahoDllPath);
  ~Paho();

  bool connect(const char* pBrokerHostName, const char* pClientId);
  bool disconnect();
  bool publish(const char* pTopic, const void* pPayload, int len, int qos, bool retain);

private:
  using MqttClientGetVersionInfo_t = decltype(MQTTClient_getVersionInfo);
  using MqttClientCreate_t = decltype(MQTTClient_create);
  using MqttClientConnect_t = decltype(MQTTClient_connect);
  using MqttClientDisconnect_t = decltype(MQTTClient_disconnect);
  using MqttClientIsConnected_t = decltype(MQTTClient_isConnected);
  using MqttClientPublish_t = decltype(MQTTClient_publish);
  using MqttClientDestroy_t = decltype(MQTTClient_destroy);
  using MqttClientPublishMessage_t = decltype(MQTTClient_publishMessage);

  MqttClientGetVersionInfo_t* pGetVersionInfoFunc_{ nullptr };
  MqttClientCreate_t* pClientCreateFunc_{ nullptr };
  MqttClientConnect_t* pClientConnectFunc_{ nullptr };
  MqttClientDisconnect_t* pClientDisconnectFunc_{ nullptr };
  MqttClientIsConnected_t* pClientIsConnectedFunc_{ nullptr };
  MqttClientPublish_t* pClientPublish_{ nullptr };
  MqttClientDestroy_t* pClientDestroy_{ nullptr };
  MqttClientPublishMessage_t* pClientPublishMessage_{ nullptr };

  HMODULE hDll_{ NULL };
  MQTTClient hMqttClient_{ NULL };
};

#endif // _PAHO_H
