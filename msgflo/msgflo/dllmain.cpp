#include <windows.h>
#include "uccncplugin.h"

static UccncPlugin* pUccncPlugin_ = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      pUccncPlugin_ = UccncPlugin::create();
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      if (pUccncPlugin_) {
        delete pUccncPlugin_;

        pUccncPlugin_ = nullptr;
      }

      break;
  }

  return TRUE;
}

extern "C" {
  __declspec(dllexport)
  void __cdecl setCallBacks(PluginInterfaceEntry uc) {
    return pUccncPlugin_->setCallBacks(uc);
  }

  __declspec(dllexport)
  void __cdecl onFirstCycle() {
    return pUccncPlugin_->onFirstCycle();
  }

  __declspec(dllexport)
  void __cdecl onTick() {
    return pUccncPlugin_->onTick();
  }

  __declspec(dllexport)
  void __cdecl onShutdown() {
    return pUccncPlugin_->onShutdown();
  }

  __declspec(dllexport)
  void __cdecl buttonpress_event(UccncButton button, bool onScreen) {
    pUccncPlugin_->buttonPressEvent(button, onScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldclick_event(UccncField label, bool isMainScreen) {
    pUccncPlugin_->textFieldClickEvent(label, isMainScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldtexttyped_event(UccncField label, bool isMainScreen, const char* pText) {
    pUccncPlugin_->textFieldTextTypedEvent(label, isMainScreen, pText);
  }

  __declspec(dllexport)
  void __cdecl getproperties_event(char* pAuthor, char* pPluginName, char* pPluginVersion) {
    pUccncPlugin_->getPropertiesEvent(pAuthor, pPluginName, pPluginVersion);
  }
}
