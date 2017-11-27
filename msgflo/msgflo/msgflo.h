#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <string>
#include <stdio.h>
#include <windows.h>
#include "singleton.h"
#include "paho.h"

using namespace std;

double getFieldDouble(bool isAS3, int fieldnumber);
using GetFieldDoubleFunc = decltype(getFieldDouble);

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  void setCallBacks(GetFieldDoubleFunc pGetFieldDouble);
  void onFirstCycle();
  void onTick();
  void onShutdown();

private:
  void attachDebugConsole();
  void detachDebugConsole();
  bool isConsoleAttached_;
  FILE* pDebugStream_{ nullptr };
  Paho* pPaho_{ nullptr };

  string brokerHostname_;;
  string topic_;
  long lastTick_{0};
  long lastTick2_{0};

  GetFieldDoubleFunc* pGetFieldDouble_{nullptr};
};

#endif // _MSFGLO_H
