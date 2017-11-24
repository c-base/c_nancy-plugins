#include <windows.h>

#include "msgflo.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      MsgFlo::create();

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      MsgFlo::destroy();

      break;
    }

    return TRUE;
}

extern "C" __declspec(dllexport)
void empty() {
  MsgFlo::getInstance()->empty();
}

extern "C" __declspec(dllexport)
void string(const char* pString) {
  MsgFlo::getInstance()->string(pString);
}

extern "C" __declspec(dllexport)
void stringAndNumber(const char* pString, int num) {
  MsgFlo::getInstance()->stringAndNumber(pString, num);
}

extern "C" __declspec(dllexport)
int stringAndNumberWithIntRet(const char* pString, int num) {
  return MsgFlo::getInstance()->stringAndNumberWithIntRet(pString, num);
}
