@echo off

:: using clang from msys2!!!

call clang --std=c23 src\main.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\main.o && clang --std=c23 src\graphutils.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\graphutils.o && clang intermediate\main.o intermediate\graphutils.o vendor\raylib-6.2_win64_mingw-w64\lib\libraylib.a -lwinmm -lgdi32 -o bin\ponpon.exe