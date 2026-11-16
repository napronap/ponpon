@echo off

:: important to not make permanent changes to path
setlocal

call %~dp0\vars.bat

:: raylib is built for MinGW, so use the MSYS2 UCRT64 toolchain

set "PATH=%MSYS2_ROOT%\ucrt64\bin;%PATH%"

pushd "%~dp0"

if not exist intermediate mkdir intermediate
if not exist bin mkdir bin

call clang --std=c23 src\main.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\main.o &&^
clang --std=c23 src\graphutils.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\graphutils.o &&^
clang --std=c23 src\tileutils.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\tileutils.o &&^
clang --std=c23 src\game.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\game.o &&^
clang --std=c23 src\debug.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\debug.o &&^
clang --std=c23 src\agari.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\agari.o &&^
clang --std=c23 src\meld.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\meld.o &&^
clang intermediate\main.o intermediate\graphutils.o intermediate\tileutils.o intermediate\game.o intermediate\debug.o intermediate\agari.o intermediate\meld.o vendor\raylib-6.2_win64_mingw-w64\lib\libraylib.a -lwinmm -lgdi32 -o bin\ponpon.exe

set "BUILD_EXIT_CODE=%ERRORLEVEL%"
popd
exit /b %BUILD_EXIT_CODE%
