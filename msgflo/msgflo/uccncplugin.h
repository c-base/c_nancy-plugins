#ifndef _UCCNCPLUGIN_H
#define _UCCNCPLUGIN_H

#include "singleton.h"
#include "uccnc_types.h"
#include "debug.h"

// These function definitions do only exist so they can be used in decltype.
// None of these functions is meant to be implemented:

extern "C" {
  bool   __cdecl _isMoving();
  void   __cdecl _getField(char* pResult, int resultBufLen, bool isAS3, UccncField field);
  int    __cdecl _getFieldInt(bool isAS3, UccncField field);
  double __cdecl _getFieldDouble(bool isAS3, UccncField field);
  bool   __cdecl _GetLed(UccncLed led);
  double __cdecl _getXpos();
  double __cdecl _getYpos();
  double __cdecl _getZpos();
  double __cdecl _getApos();
  double __cdecl _getBpos();
  double __cdecl _getCpos();
}

struct PluginInterfaceEntry {
  decltype(_getField)*       pGetField;
  decltype(_getFieldInt)*    pGetFieldInt;
  decltype(_getFieldDouble)* pGetFieldDouble;
  decltype(_GetLed)*         pGetLed;
  decltype(_isMoving)*       pIsMoving;
  decltype(_getXpos)*        pCgetXpos;
  decltype(_getYpos)*        pCgetYpos;
  decltype(_getZpos)*        pCgetZpos;
  decltype(_getApos)*        pCgetApos;
  decltype(_getBpos)*        pCgetBpos;
  decltype(_getCpos)*        pCgetCpos;
};

class UccncPlugin {
public:
  UccncPlugin() {
    if constexpr (isDebug())
      attachDebugConsole();

    // Disable close button of console windows to prevent closing UCCNC accidentally:
    HWND hwnd = GetConsoleWindow();
    HMENU hmenu = GetSystemMenu(hwnd, FALSE);
    EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

    SetConsoleTitle("UCCNC C++ plugin debug console. (Build: " __DATE__ ", " __TIME__ ")\n");
    trace();
  }

  virtual ~UccncPlugin() = 0 {
    trace();

    if constexpr (isDebug())
      detachDebugConsole();
  }

  virtual UccncPlugin* create() = 0;
  virtual void onFirstCycle() = 0;
  virtual void onTick() = 0;
  virtual void onShutdown() = 0;
  virtual void buttonPressEvent(UccncButton button, bool onScreen) = 0;
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen) = 0;
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) = 0;
  virtual void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion) = 0;

  void setCallBacks(PluginInterfaceEntry uc) {
    trace();
    UC = uc;
  }

protected:
  PluginInterfaceEntry UC{ 0 };
};

#endif // _UCCNCPLUGIN_H

