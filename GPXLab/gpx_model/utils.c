#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include "utils.h"

void UTILS_setenv(const char *name, const char *value)
{
  #ifdef WIN32
    char str[128];
    sprintf_s(str, 128, "%s=%s", name, value);
    _putenv(str);
  #else
    setenv(name, value, 1);
  #endif
    setlocale(LC_ALL, "C");
}

void UTILS_unsetenv(const char *name)
{
  #ifdef WIN32
    char str[128];
    sprintf_s(str, 128, "%s=", name);
    _putenv(str);
  #else
    unsetenv(name);
  #endif
    setlocale(LC_ALL, "");
}
