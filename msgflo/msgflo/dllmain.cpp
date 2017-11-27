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
void __cdecl setCallBacks(GetFieldDoubleFunc pGetFieldDouble) {
  return MsgFlo::getInstance()->setCallBacks(pGetFieldDouble);
}

extern "C" __declspec(dllexport)
void __cdecl onFirstCycle() {
  return MsgFlo::getInstance()->onFirstCycle();
}

extern "C" __declspec(dllexport)
void __cdecl onTick() {
  return MsgFlo::getInstance()->onTick();
}

extern "C" __declspec(dllexport)
void __cdecl onShutdown() {
  return MsgFlo::getInstance()->onShutdown();
}
