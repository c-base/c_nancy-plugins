#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <stdio.h>
#include <windows.h>
#include "singleton.h"
#include "paho.h"

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  bool mqttConnect(const char* pBrokerHostName, const char* pClientId);
  bool mqttDisconnect();
  bool mqttPublish(const char* pTopic, const void* pPayload, int len, int qos, bool retain);

private:
  void attachDebugConsole();
  void detachDebugConsole();
  bool isConsoleAttached_;
  FILE* pDebugStream_{ nullptr };
  Paho* pPaho_{ nullptr };
};

#endif // _MSFGLO_H
