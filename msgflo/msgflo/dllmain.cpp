#include <windows.h>
#include "uccnc_types.h"
#include "msgflo.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
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
void __cdecl setCallBacks(PluginInterfaceEntry uc) {
  return MsgFlo::getInstance()->setCallBacks(uc);
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

extern "C" __declspec(dllexport)
void __cdecl buttonpress_event(UccncButton button, bool onScreen) {
  MsgFlo::getInstance()->buttonPressEvent(button, onScreen);
}

extern "C" __declspec(dllexport)
void __cdecl textfieldclick_event(UccncField label, bool isMainScreen) {
  MsgFlo::getInstance()->textFieldClickEvent(label, isMainScreen);
}

extern "C" __declspec(dllexport)
void __cdecl textfieldtexttyped_event(UccncField label, bool isMainScreen, const char* pText) {
  MsgFlo::getInstance()->textFieldTextTypedEvent(label, isMainScreen, pText);
}

extern "C" __declspec(dllexport)
void __cdecl getproperties_event(char* pAuthor, char* pPluginName, char* pPluginVersion) {
  MsgFlo::getInstance()->getPropertiesEvent(pAuthor, pPluginName, pPluginVersion);
}
