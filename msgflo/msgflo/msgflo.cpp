#include <time.h>
#include <limits.h>
#include "json.hpp"
#include "debug.h"
#include "msgflo.h"

using json = nlohmann::json;

MsgFlo::MsgFlo() {
#ifdef _DEBUG
  attachDebugConsole();
#endif
  trace();

  string dllName = "paho-mqtt3c.dll";
  char pExePath[256];

  GetModuleFileName(NULL, pExePath, sizeof(pExePath));

  string exePath = pExePath;
  string dllPath = exePath.substr(0, exePath.find_last_of("\\") + 1);
  dllPath += "Plugins\\";
  dllPath += dllName;

  printf("Paho path is: %s", pExePath);

  pPaho_ = new Paho(dllPath);
}


MsgFlo::~MsgFlo() {
  if(pPaho_)
    delete pPaho_;

  trace();

#ifdef _DEBUG
  detachDebugConsole();
#endif
}

void MsgFlo::setCallBacks(GetFieldDoubleFunc* pGetFieldDouble, PluginInterfaceEntry uc) {
  trace();

  UC = uc;
}

void MsgFlo::onFirstCycle() {
  trace();

  pPaho_->connect(MQTT_BROKER_HOSTNAME, MQTT_CLIENT_ID);
  pPaho_->publish(baseTopic_ + "online", "true", 4, 1, true);

  lastTick_ = clock();
  lastTick2_ = clock();
}

void MsgFlo::onTick() {
  long tick = clock();
  long diffSeconds = (tick - lastTick_) / 1000;
  long diffMs2 = (tick - lastTick2_);

  bool isMoving = UC.pGetLed(UcncLed::Run);
  bool isMilling = UC.pGetLed(UcncLed::Cyclestart);

  if (isMilling != isMilling_) {
    isMilling_ = isMilling;

    json milling;
    milling["milling"] = isMilling;

    string millingStr = milling.dump();
    pPaho_->publish(baseTopic_ + "milling", millingStr.c_str(), millingStr.length(), 1, true);
  }

  if (diffMs2 >= 250) {
    position_.x = UC.pGetFieldDouble(true, UcncField::XposDRO) / 10000.0;
    position_.y = UC.pGetFieldDouble(true, UcncField::YposDRO) / 10000.0;
    position_.z = UC.pGetFieldDouble(true, UcncField::ZposDRO) / 10000.0;
    position_.a = UC.pGetFieldDouble(true, UcncField::AposDRO) / 10000.0;
    position_.b = UC.pGetFieldDouble(true, UcncField::BposDRO) / 10000.0;
    position_.c = UC.pGetFieldDouble(true, UcncField::CposDRO) / 10000.0;

    if (isMoving) {
      json position;
      position["X"] = position_.x;
      position["Y"] = position_.y;
      position["Z"] = position_.z;
      position["A"] = position_.a;
      position["B"] = position_.b;
      position["C"] = position_.c;

      string positionStr = position.dump();
      pPaho_->publish(baseTopic_ + "position", positionStr.c_str(), positionStr.length(), 1, false);
    }

    if (isMilling) {
      static int _cnt = 0; // temporary hack to only update every second

      if (_cnt++ == 2) {
        _cnt = 0;

        char pField[256];
        UC.pGetField(pField, sizeof(pField), true, UcncField::Worktimer);

        json worktime;
        worktime["worktime"] = pField;

        string worktimeStr = worktime.dump();
        pPaho_->publish(baseTopic_ + "worktime", worktimeStr.c_str(), worktimeStr.length(), 1, true);
      }
    }

    lastTick2_ = clock();
  }

  if (diffSeconds >= 60) {
    json root;
    root["protocol"] = "discovery";
    root["command"] = "participant";

    json payload;
    payload["component"] = "c-base/c_nancy";
    payload["label"] = "CNC mill status";
    payload["icon"] = "scissors";
    payload["inports"] = json::array();

    json outports = json::array();
    json onlinePort;
    onlinePort["id"] = "online";
    onlinePort["type"] = "boolean";
    onlinePort["queue"] = baseTopic_ + "online";

    json millingPort;
    millingPort["id"] = "milling";
    millingPort["type"] = "boolean";
    millingPort["queue"] = baseTopic_ + "milling";

    json positionPort;
    positionPort["id"] = "position";
    positionPort["type"] = "object";
    positionPort["queue"] = baseTopic_ + "position";

    json worktimePort;
    worktimePort["id"] = "worktime";
    worktimePort["type"] = "String";
    worktimePort["queue"] = baseTopic_ + "worktime";

    outports.push_back(onlinePort);
    outports.push_back(millingPort);
    outports.push_back(positionPort);
    outports.push_back(worktimePort);

    payload["outports"] = outports;

    root["role"] = "c_nancy";
    root["id"] = "c_nancy";
    root["payload"] = payload;

    printf("payload object:\n---\n%s\n---\n", root.dump(4).c_str());

    string discoveryStr = root.dump();
    pPaho_->publish("fbp", discoveryStr.c_str(), discoveryStr.length(), 1, true);

    lastTick_ = clock();
  }
}

void MsgFlo::onShutdown() {
  trace();

  pPaho_->publish(baseTopic_ + "online", "false", 5, 1, true);
  pPaho_->disconnect();
}

void MsgFlo::buttonPressEvent(int buttonNumber, bool onScreen) {
  // trace();

  if (onScreen) {
    if (buttonNumber == 128) {
      // TODO: implement

    }
  }
}

void MsgFlo::textFieldClickEvent(int labelNumber, bool isMainScreen) {
  trace();

  if (isMainScreen) {
    if (labelNumber == 1000) {
      // TODO: implement
    }
  }
}

void MsgFlo::textFieldTextTypedEvent(int labelNumber, bool isMainScreen, const char* pText) {
  trace();

  if (isMainScreen) {
    if (labelNumber == 1000) {
      // TODO: implement
    }
  }
}

void MsgFlo::getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion) {
  trace();

  strcpy_s(pAuthor, 256, AUTHOR);
  strcpy_s(pPluginName, 256, PLUGIN_NAME);
  strcpy_s(pPluginVersion, 256, PLUGIN_VERSION);
}
