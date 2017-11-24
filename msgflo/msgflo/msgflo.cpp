#include "msgflo.h"

MsgFlo::MsgFlo() {
  attachDebugConsole();

  dbg("MsgFlo::ctor");
}

MsgFlo::~MsgFlo() {
  dbg("MsgFlo::dtor");

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
