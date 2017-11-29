#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <string>
#include <stdio.h>
#include <windows.h>
#include "singleton.h"
#include "paho.h"

using namespace std;

constexpr const char* AUTHOR          = "coon@c-base.org";
constexpr const char* PLUGIN_NAME     = "msg-flo (c++)";
constexpr const char* PLUGIN_VERSION  = "1.1000";

constexpr const char* MQTT_BROKER_HOSTNAME = "tcp://c-beam:1883";
constexpr const char* MQTT_BASE_TOPIC = "werkstatt/c_nancy/";
constexpr const char* MQTT_CLIENT_ID = "c_nancy";

double getFieldDouble(bool isAS3, int fieldnumber);
using GetFieldDoubleFunc = decltype(getFieldDouble);

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  void setCallBacks(GetFieldDoubleFunc pGetFieldDouble);
  void onFirstCycle();
  void onTick();
  void onShutdown();
  void buttonPressEvent(int buttonNumber, bool onScreen);
  void textFieldClickEvent(int labelNumber, bool isMainScreen);
  void textFieldTextTypedEvent(int labelnumber, bool isMainScreen, const char* pText);
  void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion);

private:
  Paho* pPaho_{ nullptr };
  string brokerHostname_;;
  string topic_;
  long lastTick_{0};
  long lastTick2_{0};

  GetFieldDoubleFunc* pGetFieldDouble_{nullptr};
};

#endif // _MSFGLO_H
