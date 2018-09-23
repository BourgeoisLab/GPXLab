/****************************************************************************
 *   Copyright (c) 2014 - 2016 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

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
