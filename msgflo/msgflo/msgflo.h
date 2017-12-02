#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <string>
#include <stdio.h>
#include <windows.h>
#include "json.hpp"
#include "singleton.h"
#include "uccnc_types.h"
#include "paho.h"

using namespace std;
using json = nlohmann::json;

constexpr const char* AUTHOR          = "coon@c-base.org";
constexpr const char* PLUGIN_NAME     = "msgflo (c++)";
constexpr const char* PLUGIN_VERSION  = "02.12.17";

constexpr const char* MQTT_BROKER_HOSTNAME = "tcp://c-beam:1883";
constexpr const char* MQTT_BASE_TOPIC = "werkstatt/c_nancy/";
constexpr const char* MQTT_CLIENT_ID = "c_nancy";

extern "C" {
  bool   __cdecl uCisMoving();
  void   __cdecl uCgetField(char* pResult, int resultBufLen, bool isAS3, UccncField field);
  int    __cdecl uCgetFieldInt(bool isAS3, UccncField field);
  double __cdecl uCgetFieldDouble(bool isAS3, UccncField field);
  bool   __cdecl uCGetLed(UccncLed led);
  double __cdecl uCgetXpos();
  double __cdecl uCgetYpos();
  double __cdecl uCgetZpos();
  double __cdecl uCgetApos();
  double __cdecl uCgetBpos();
  double __cdecl uCgetCpos();
}

using GetFieldFunc        = decltype(uCgetField);
using GetFieldIntFunc     = decltype(uCgetFieldInt);
using GetFieldDoubleFunc  = decltype(uCgetFieldDouble);
using GetLedFunc          = decltype(uCGetLed);
using IsMovingFunc        = decltype(uCisMoving);
using GetXpos             = decltype(uCgetXpos);
using GetYpos             = decltype(uCgetYpos);
using GetZpos             = decltype(uCgetZpos);
using GetApos             = decltype(uCgetApos);
using GetBpos             = decltype(uCgetBpos);
using GetCpos             = decltype(uCgetCpos);

struct PluginInterfaceEntry {
  GetFieldFunc*       pGetField;
  GetFieldIntFunc*    pGetFieldInt;
  GetFieldDoubleFunc* pGetFieldDouble;
  GetLedFunc*         pGetLed;
  IsMovingFunc*       pIsMoving;
  GetXpos*            pCgetXpos;
  GetYpos*            pCgetYpos;
  GetZpos*            pCgetZpos;
  GetApos*            pCgetApos;
  GetBpos*            pCgetBpos;
  GetCpos*            pCgetCpos;
};

enum class MsgRetain {
  DoNotRetain,
  Retain,
};

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  void setCallBacks(PluginInterfaceEntry uc);
  void onFirstCycle();
  void onTick();
  void onShutdown();
  void buttonPressEvent(UccncButton button, bool onScreen);
  void textFieldClickEvent(UccncField label, bool isMainScreen);
  void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText);
  void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion);

private:
  bool mqttConnect();
  void mqttDisconnect();
  bool mqttIsConnected();
  bool mqttPublish(const string& topic, const string& subTopic, const json& jsonObj,
      MsgRetain retain = MsgRetain::DoNotRetain);
  bool mqttPublish(const string& subTopic, const json& jsonObj, MsgRetain retain = MsgRetain::DoNotRetain);

  bool isMilling();

  // Tick handlers:
  void handleDiscovery(long timeMs);
  void handleMillingState(long timeMs);
  void handlePositionState(long timeMs);
  void handleWorkTime(long timeMs);

  Paho* pPaho_{ nullptr };
  string brokerHostname_{MQTT_BROKER_HOSTNAME};
  string baseTopic_{MQTT_BASE_TOPIC};
  PluginInterfaceEntry UC{0};
  bool isMilling_{false};
};

#endif // _MSFGLO_H
