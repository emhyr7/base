
@echo off
cd /d "%~dp0"
setlocal

if not exist build mkdir build
cd build

set CFLAGS=-O0 -g -Wall -Wextra -Wno-char-subscripts -Wno-bitwise-op-parentheses

clang %CFLAGS% -c ..\code\*.c || exit /b 1
lib /nologo /out:base.lib *.o || exit /b 1
