#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <string>
#include <stdio.h>
#include <windows.h>
#include "singleton.h"
#include "types.h"
#include "paho.h"

using namespace std;

constexpr const char* AUTHOR          = "coon@c-base.org";
constexpr const char* PLUGIN_NAME     = "msg-flo (c++)";
constexpr const char* PLUGIN_VERSION  = __DATE__;

constexpr const char* MQTT_BROKER_HOSTNAME = "tcp://c-beam:1883";
constexpr const char* MQTT_BASE_TOPIC = "werkstatt/c_nancy/";
constexpr const char* MQTT_CLIENT_ID = "c_nancy";

extern "C" {
  void   __cdecl uCgetField(char* pResult, int resultBufLen, bool isAS3, UcncField field);
  double __cdecl uCgetFieldDouble(bool isAS3, UcncField field);
  bool   __cdecl uCGetLed(UcncLed led);
  bool   __cdecl uCisMoving();
}

using GetFieldFunc        = decltype(uCgetField);
using GetFieldDoubleFunc  = decltype(uCgetFieldDouble);
using GetLedFunc          = decltype(uCGetLed);
using IsMovingFunc        = decltype(uCisMoving);

struct PluginInterfaceEntry {
  GetFieldFunc* pGetField;
  GetFieldDoubleFunc* pGetFieldDouble;
  GetLedFunc* pGetLed;
  IsMovingFunc* pIsMoving;
};

struct Position {
  double x;
  double y;
  double z;
  double a;
  double b;
  double c;
};

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  void setCallBacks(GetFieldDoubleFunc* pGetFieldDouble, PluginInterfaceEntry uc);
  void onFirstCycle();
  void onTick();
  void onShutdown();
  void buttonPressEvent(int buttonNumber, bool onScreen);
  void textFieldClickEvent(int labelNumber, bool isMainScreen);
  void textFieldTextTypedEvent(int labelnumber, bool isMainScreen, const char* pText);
  void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion);

private:
  Paho* pPaho_{ nullptr };
  string brokerHostname_{MQTT_BROKER_HOSTNAME};
  string baseTopic_{MQTT_BASE_TOPIC};
  long lastTick_{0};
  long lastTick2_{0};
  Position position_{0};
  PluginInterfaceEntry UC{0};
  bool isMilling_{false};
};

#endif // _MSFGLO_H
