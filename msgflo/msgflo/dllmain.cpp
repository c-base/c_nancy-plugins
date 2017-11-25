#include <windows.h>

#include "msgflo.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      MsgFlo::create();
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      MsgFlo::destroy();

      break;
    }

    return TRUE;
}

extern "C" __declspec(dllexport)
bool __cdecl mqttConnect(const char* pBrokerHostName, const char* pClientId) {
  return MsgFlo::getInstance()->mqttConnect(pBrokerHostName, pClientId);
}

extern "C" __declspec(dllexport)
bool __cdecl mqttDisconnect() {
  return MsgFlo::getInstance()->mqttDisconnect();
}

extern "C" __declspec(dllexport)
bool __cdecl mqttPublish(const char* pTopic, const char* pPayload, int len, int qos, bool retain) {
  return MsgFlo::getInstance()->mqttPublish(pTopic, pPayload, len, qos, retain);
}
