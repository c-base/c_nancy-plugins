#ifndef _PAHO_H
#define _PAHO_H

#include <string>
#include <windows.h>
#include "MQTTClient.h"
#include "paho.h"

using namespace std;

class Paho {
public:
  Paho(const string& pahoDllPath);
  ~Paho();

  bool connect(const string& brokerHostName, const string& clientId);
  bool connect(const string& brokerHostName, const string& clientId, const string& lastWillTopic,
    const void* pLastWillMsg, int msgLen);

  bool disconnect();
  bool isConnected();
  bool publish(const string& topic, const void* pPayload, int len, int qos, bool retain);

private:
  using MqttClientSetCallBacks_t      = decltype(MQTTClient_setCallbacks);
  using MqttClientGetVersionInfo_t    = decltype(MQTTClient_getVersionInfo);
  using MqttClientCreate_t            = decltype(MQTTClient_create);
  using MqttClientConnect_t           = decltype(MQTTClient_connect);
  using MqttClientDisconnect_t        = decltype(MQTTClient_disconnect);
  using MqttClientIsConnected_t       = decltype(MQTTClient_isConnected);
  using MqttClientPublish_t           = decltype(MQTTClient_publish);
  using MqttClientDestroy_t           = decltype(MQTTClient_destroy);
  using MqttClientPublishMessage_t    = decltype(MQTTClient_publishMessage);
  using MqttClientWaitForCompletion_t = decltype(MQTTClient_waitForCompletion);

  static void onConnectionLost(void* pContext, char* pCause);
  static int onMsgArrived(void* pContext, char* pTopicName, int topicLen, MQTTClient_message* pMessage);
  static void onMsgDeliveryComplete(void* pContext, MQTTClient_deliveryToken dt);

  MqttClientSetCallBacks_t* pSetCallBacksFunc_{ nullptr };
  MqttClientGetVersionInfo_t* pGetVersionInfoFunc_{ nullptr };
  MqttClientCreate_t* pClientCreateFunc_{ nullptr };
  MqttClientConnect_t* pClientConnectFunc_{ nullptr };
  MqttClientDisconnect_t* pClientDisconnectFunc_{ nullptr };
  MqttClientIsConnected_t* pClientIsConnectedFunc_{ nullptr };
  MqttClientPublish_t* pClientPublish_{ nullptr };
  MqttClientDestroy_t* pClientDestroy_{ nullptr };
  MqttClientPublishMessage_t* pClientPublishMessage_{ nullptr };
  MqttClientWaitForCompletion_t* pClientWaitForCompletion_{ nullptr };

  HMODULE hDll_{ NULL };
  MQTTClient hMqttClient_{ NULL };
};

#endif // _PAHO_H
