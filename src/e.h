#ifndef _ERR_H_
#define _ERR_H_

#include <stdarg.h>

void err(int eval, const char* fmt, ...);
void warn(const char* fmt, ...);

#endif // _ERR_H_
