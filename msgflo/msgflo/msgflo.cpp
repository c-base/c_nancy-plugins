#include "msgflo.h"

MsgFlo::MsgFlo() {
  attachDebugConsole();
  trace();
}

MsgFlo::~MsgFlo() {
  trace();
  detachDebugConsole();
}

void MsgFlo::attachDebugConsole() {
  if (!isConsoleAttached_) {
    AllocConsole();

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    freopen_s(&pDebugStream_, "CONOUT$", "w", stdout);

    isConsoleAttached_ = true;
  }
}

void MsgFlo::detachDebugConsole() {
  if (isConsoleAttached_) {
    fclose(pDebugStream_);

    // TODO: set stdout to some empty stream, which does not crash when calling printf after this!?

    FreeConsole();

    isConsoleAttached_ = false;
  }
}

void MsgFlo::empty() {
  trace();
}

void MsgFlo::string(const char* pString) {
  trace();

  printf("pString: %s\n", pString);
}

void MsgFlo::stringAndNumber(const char* pString, int num) {
  trace();

  printf("pString: %s, num=%d\n", pString, num);
}

int MsgFlo::stringAndNumberWithIntRet(const char* pString, int num) {
  trace();

  printf("pString: %s, num=%d\n", pString, num);

  return 42;
}
