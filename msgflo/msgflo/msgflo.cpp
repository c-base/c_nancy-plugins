﻿#include <time.h>
#include <limits.h>
#include <fstream>
#include "msgflo.h"

MsgFlo::MsgFlo() {
  if constexpr (isDebug())
    attachDebugConsole();

  trace();

  const string dllName = "paho-mqtt3c.dll";
  char pExePath[256];

  GetModuleFileName(NULL, pExePath, sizeof(pExePath));

  string exePath = pExePath;
  string dllPath = exePath.substr(0, exePath.find_last_of("\\") + 1);
  dllPath += "Plugins\\cpp\\";
  dllPath += dllName;

  dbg("Paho path is: %s\n", dllPath.c_str());

  pPaho_ = new Paho(dllPath);
}

MsgFlo::~MsgFlo() {
  trace();

  if (pPaho_->isConnected()) {
    msgFloOnline(false);
  }

  if(pPaho_)
    delete pPaho_;

  if constexpr (isDebug())
    detachDebugConsole();
}

bool MsgFlo::mqttConnect() {
  json j = "false";
  string lastWillStr = j.dump();

  return pPaho_->connect(MQTT_BROKER_HOSTNAME, MQTT_CLIENT_ID, baseTopic_ + "online", lastWillStr.c_str(),
      lastWillStr.length());
}

void MsgFlo::mqttDisconnect() {
  pPaho_->disconnect();
}

bool MsgFlo::mqttIsConnected() {
  return pPaho_->isConnected();
}

bool MsgFlo::mqttPublish(const string& baseTopic, const string& subTopic, const json& jsonObj,
    MsgRetain retain) {
  string jsonStr = jsonObj.dump();

  return pPaho_->publish(baseTopic + subTopic, jsonStr.c_str(), jsonStr.length(), 1,
      retain == MsgRetain::Retain);
}

bool MsgFlo::mqttPublish(const string& subTopic, const json& jsonObj, MsgRetain retain) {
  return mqttPublish(baseTopic_, subTopic, jsonObj, retain);
}

void MsgFlo::onFirstCycle() {
  trace();

  if (!mqttConnect())
    dbg("Connection to MQTT broker '%s' failed!\n", MQTT_BROKER_HOSTNAME);
  else
    dbg("Connection to MQTT broker '%s' established\n", MQTT_BROKER_HOSTNAME);

  msgFloOnline(true);
}

void MsgFlo::onTick() {
  // trace();

  long timeMs = clock();

  if (!mqttIsConnected())
    return;

  handleMillingState(timeMs);
  handleDiscovery(timeMs);
  handlePositionState(timeMs);
  handleWorkTime(timeMs);
}

void MsgFlo::handleDiscovery(long timeMs) {
  static long lastTick = 0;

  if ((timeMs - lastTick < 60 * 1000) && lastTick != 0)
    return;

  dbg("MsgFlo::handleDiscovery; timeMs=%d, lastTick=%d\n", timeMs, lastTick);

  json j;
  j["protocol"] = "discovery";
  j["command"] = "participant";

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

  json gcodePort;
  gcodePort["id"] = "gcode";
  gcodePort["type"] = "String";
  gcodePort["queue"] = baseTopic_ + "gcode";

  outports.push_back(onlinePort);
  outports.push_back(millingPort);
  outports.push_back(positionPort);
  outports.push_back(worktimePort);
  outports.push_back(gcodePort);

  payload["outports"] = outports;

  j["role"] = "c_nancy";
  j["id"] = "c_nancy";
  j["payload"] = payload;

  // printf("payload object:\n---\n%s\n---\n", j.dump(4).c_str());

  mqttPublish("fbp", "", j);
  dbg("Sent msgflo discovery message\n");

  lastTick = timeMs;
}

bool MsgFlo::isMilling() {
  return UC.pGetLed(UccncLed::Cyclestart);
}

void MsgFlo::handleMillingState(long timeMs) {
  static int _lastLine = -1;
  static ifstream _gCodeFile;

  bool m = isMilling();

  if (isMilling_ != m) {
    isMilling_ = m;

    json j = m;
    mqttPublish("milling", j);

    if (isMilling_) {
      char pField[256];
      UC.pGetField(pField, sizeof(pField), true, UccncField::Diagnostics_Filename);

      // The field has a leading white space (0x20) which will cause an error when trying to
      // open the file. Therefore this leading whitespace will be skipped:

      const char* pFileName = &pField[1];
      _gCodeFile.open(pFileName);

      if (!_gCodeFile.is_open()) {
        dbg("Error: failed to open gcode file: %s!\n", pFileName);
      }
    }
    else
      _gCodeFile.close();
  }

  int currentLine = UC.pGetFieldInt(true, UccncField::Setnextlinefield);

  if (m && _lastLine != currentLine) {
    string line;

    if (currentLine < _lastLine)
      _lastLine = 0;

    int diff = currentLine - _lastLine;

    // Skip lines if needed:
    for (int i = 1; i < diff; i++)
      getline(_gCodeFile, line);

    getline(_gCodeFile, line);
    dbg("Line: %d: %s\n", currentLine, line.c_str());

    json j = line;
    mqttPublish("gcode", j);

    _lastLine = currentLine;
  }
}

void MsgFlo::handlePositionState(long timeMs) {
  if (!UC.pIsMoving())
    return;

  json j;
  j["X"] = UC.pCgetXpos();
  j["Y"] = UC.pCgetYpos();
  j["Z"] = UC.pCgetZpos();
  j["A"] = UC.pCgetApos();
  j["B"] = UC.pCgetBpos();
  j["C"] = UC.pCgetCpos();

  dbg(j.dump(4).c_str());
  dbg("\n");

  mqttPublish("position", j);
}

void MsgFlo::handleWorkTime(long timeMs) {
  long lastTick = 0;

  // Only update every 250 ms:
  if (lastTick != 0 && timeMs - lastTick < 250)
    return;

  if (!isMilling())
    return;

  char pField[256];
  UC.pGetField(pField, sizeof(pField), true, UccncField::Worktimer);

  json j = pField;
  mqttPublish("worktime", j);

  lastTick = timeMs;
}

void MsgFlo::onShutdown() {
  trace();

  msgFloOnline(false);
}

void MsgFlo::buttonPressEvent(UccncButton button, bool onScreen) {
  // trace();

  if (onScreen) {
    if (button == UccncButton::Cyclestart) {
      // TODO: implement

    }
  }
}

void MsgFlo::textFieldClickEvent(UccncField label, bool isMainScreen) {
  const char* pLabel;

  switch (label) {
    case UccncField::Mdi:
      pLabel = "Mdi";
      break;

    case UccncField::Setnextlinefield:
      pLabel = "Setnextlinefield";
      break;

    default:
      pLabel = "Unknown";
  }

  dbg("textFieldClickEvent; label=%s, isMainScreen=%d\n", pLabel, isMainScreen);
}

void MsgFlo::textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) {
  trace();

  if (isMainScreen) {
    if (label == UccncField::Mdi) {
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

bool MsgFlo::msgFloOnline(bool isOnline) {
  trace();

  json j = isOnline;
  return mqttPublish("online", j, MsgRetain::Retain);
}