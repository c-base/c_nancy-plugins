#ifndef _UCCNCPLUGIN_H
#define _UCCNCPLUGIN_H

#include "singleton.h"
#include "uccnc_types.h"
#include "debug.h"

// This function definitions do only exist so they can be used in decltype.
// None of the functions is implemented:

extern "C" {
  bool   __cdecl uCisMoving();
  void   __cdecl uCgetField(char* pResult, int resultBufLen, bool isAS3, UccncField field);
  int    __cdecl uCgetFieldInt(bool isAS3, UccncField field);
  double __cdecl uCgetFieldDouble(bool isAS3, UccncField field);
  bool   __cdecl uCGetLed(UccncLed led);
  double __cdecl uCgetXpos();
  double __cdecl uCgetYpos();
  double __cdecl uCgetZpos();
  double __cdecl uCgetApos();
  double __cdecl uCgetBpos();
  double __cdecl uCgetCpos();
}

struct PluginInterfaceEntry {
  decltype(uCgetField)*       pGetField;
  decltype(uCgetFieldInt)*    pGetFieldInt;
  decltype(uCgetFieldDouble)* pGetFieldDouble;
  decltype(uCGetLed)*         pGetLed;
  decltype(uCisMoving)*       pIsMoving;
  decltype(uCgetXpos)*        pCgetXpos;
  decltype(uCgetYpos)*        pCgetYpos;
  decltype(uCgetZpos)*        pCgetZpos;
  decltype(uCgetApos)*        pCgetApos;
  decltype(uCgetBpos)*        pCgetBpos;
  decltype(uCgetCpos)*        pCgetCpos;
};

template <typename T>
class UccncPlugin : public Singleton<T> {
public:
  void setCallBacks(PluginInterfaceEntry uc);
  virtual void onFirstCycle() = 0;
  virtual void onTick() = 0;
  virtual void onShutdown() = 0;
  virtual void buttonPressEvent(UccncButton button, bool onScreen) = 0;
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen) = 0;
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) = 0;
  virtual void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion) = 0;

protected:
  PluginInterfaceEntry UC{ 0 };
};

template <typename T>
void UccncPlugin<T>::setCallBacks(PluginInterfaceEntry uc) {
  trace();

  UC = uc;
}

#endif // _UCCNCPLUGIN_H

