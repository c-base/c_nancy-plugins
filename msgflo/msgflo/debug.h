#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

constexpr bool isDebug() {
#ifdef _DEBUG
  return true;
#else
  return false;
#endif
}

#ifdef _DEBUG
  #define dbg(...) printf(__VA_ARGS__)
  #define trace() dbg("%s()\n", __FUNCTION__)
#else
  #define dbg(...) __noop
  #define trace()  __noop
#endif

void attachDebugConsole();
void detachDebugConsole();

#endif // _DEBUG_H
