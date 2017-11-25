#include <string>
#include "debug.h"
#include "msgflo.h"

using namespace std;

MsgFlo::MsgFlo() {
  attachDebugConsole();
  trace();

  string dllName = "paho-mqtt3c.dll";
  char pExePath[256];

  GetModuleFileName(NULL, pExePath, sizeof(pExePath));

  string exePath = pExePath;
  string dllPath = exePath.substr(0, exePath.find_last_of("\\") + 1);
  dllPath += "Plugins\\";
  dllPath += dllName;

  printf("Paho path is: %s", pExePath);

  pPaho_ = new Paho(dllPath.c_str());
}

MsgFlo::~MsgFlo() {
  if(pPaho_)
    delete pPaho_;

  trace();
  detachDebugConsole();
}

void MsgFlo::attachDebugConsole() {
  if (!isConsoleAttached_) {
    AllocConsole();

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    freopen_s(&pDebugStream_, "CONOUT$", "w", stdout);

    isConsoleAttached_ = true;
  }
}

void MsgFlo::detachDebugConsole() {
  if (isConsoleAttached_) {
    fclose(pDebugStream_);

    // TODO: set stdout to some empty stream, which does not crash when calling printf after this!?

    FreeConsole();

    isConsoleAttached_ = false;
  }
}

bool MsgFlo::mqttConnect(const char* pBrokerHostName, const char* pClientId) {
  trace();

  return pPaho_->connect(pBrokerHostName, pClientId);
}

bool MsgFlo::mqttDisconnect() {
  trace();

  return pPaho_->disconnect();
}

bool MsgFlo::mqttPublish(const char* pTopic, const void* pPayload, int len, int qos, bool retain) {
  trace();

  return pPaho_->publish(pTopic, pPayload, len, qos, retain);
}
