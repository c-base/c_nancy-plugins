#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#define dbg(...) printf(__VA_ARGS__)
#define trace() dbg("%s()\n", __FUNCTION__)

#endif // _DEBUG_H
