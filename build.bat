@echo off
setlocal

:: raylib is built for MinGW, so use the MSYS2 UCRT64 toolchain.
if not defined MSYS2_ROOT set "MSYS2_ROOT=C:\msys64"
if not exist "%MSYS2_ROOT%\ucrt64\bin\clang.exe" (
    echo ERROR: MSYS2 UCRT64 clang not found. Set MSYS2_ROOT to your MSYS2 installation.
    exit /b 1
)
set "PATH=%MSYS2_ROOT%\ucrt64\bin;%PATH%"

pushd "%~dp0" || exit /b 1
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
