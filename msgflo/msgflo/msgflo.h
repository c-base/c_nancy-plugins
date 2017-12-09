#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <string>
#include <stdio.h>
#include <windows.h>
#include "uccncplugin.h"
#include "json.hpp"
#include "paho.h"

using namespace std;
using json = nlohmann::json;

constexpr const char* AUTHOR          = "coon@c-base.org";
constexpr const char* PLUGIN_NAME     = "msgflo (c++)";
constexpr const char* PLUGIN_VERSION  = "09.12.17";

constexpr const char* MQTT_BROKER_HOSTNAME = "tcp://c-beam:1883";
constexpr const char* MQTT_BASE_TOPIC      = "werkstatt/c_nancy/";
constexpr const char* MQTT_CLIENT_ID       = "c_nancy";

enum class MsgRetain {
  DoNotRetain,
  Retain,
};

class MsgFlo : public UccncPlugin {
public:
  MsgFlo();
  ~MsgFlo();

  virtual void onFirstCycle() final override;
  virtual void onTick() final override;
  virtual void onShutdown() final override;
  virtual void buttonPressEvent(UccncButton button, bool onScreen) final override;
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen) final override;
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) final override;

private:
  bool mqttConnect();
  void mqttDisconnect();
  bool mqttIsConnected();
  bool mqttPublish(const string& topic, const string& subTopic, const json& jsonObj,
      MsgRetain retain = MsgRetain::DoNotRetain);
  bool mqttPublish(const string& subTopic, const json& jsonObj, MsgRetain retain = MsgRetain::DoNotRetain);

  bool msgFloOnline(bool isOnline);

  bool isMilling();

  // Tick handlers:
  void handleDiscovery(long timeMs);
  void handleMillingState(long timeMs);
  void handlePositionState(long timeMs);
  void handleWorkTime(long timeMs);

  Paho* pPaho_{ nullptr };
  string brokerHostname_{MQTT_BROKER_HOSTNAME};
  string baseTopic_{MQTT_BASE_TOPIC};
  bool isMilling_{false};
};

#endif // _MSFGLO_H
