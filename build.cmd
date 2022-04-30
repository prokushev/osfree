@echo off
@set os2_shell >nul 2>&1
if errorlevel 1 goto _win

:_os2
call build-os2 %1 %2 %3 %4 %5 %6 %7 %8 %9
exit

:_win
if %PROCESSOR_ARCHITECTURE% == x86 goto _w32
call build-w64 %1 %2 %3 %4 %5 %6 %7 %8 %9
exit

:_w32
call build-w32 %1 %2 %3 %4 %5 %6 %7 %8 %9
