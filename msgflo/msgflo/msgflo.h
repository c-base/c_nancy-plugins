#ifndef _MSFGLO_H
#define _MSFGLO_H

#include <windows.h>
#include <stdio.h>
#include "singleton.h"

#define dbg(...) printf(__VA_ARGS__)
#define trace() dbg("%s()\n", __FUNCTION__)

class MsgFlo : public Singleton<MsgFlo> {
public:
  MsgFlo();
  ~MsgFlo();

  void empty();
  void string(const char* pString);
  void stringAndNumber(const char* pString, int num);
  int stringAndNumberWithIntRet(const char* pString, int num);

private:
  void attachDebugConsole();
  void detachDebugConsole();
  bool isConsoleAttached_;
  FILE* pDebugStream_{ nullptr };
};

#endif // _MSFGLO_H
