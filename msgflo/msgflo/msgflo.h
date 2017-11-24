#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <windows.h>
#include <stdio.h>
#include "singleton.h"

#define dbg printf

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

private:
  void attachDebugConsole();
  void detachDebugConsole();
  bool isConsoleAttached_;
  FILE* pDebugStream_{ nullptr };
};

#endif // _MSFGLO_H
