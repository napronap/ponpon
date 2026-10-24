rem @echo off

:: using clang from msys2!!!

call clang --std=c23 src\main.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\main.o &&^
clang --std=c23 src\graphutils.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\graphutils.o &&^
clang --std=c23 src\tileutils.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\tileutils.o &&^
clang --std=c23 src\game.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\game.o &&^
clang --std=c23 src\debug.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\debug.o &&^
clang --std=c23 src\agari.c -c -I vendor\raylib-6.2_win64_mingw-w64\include -o intermediate\agari.o &&^
clang intermediate\main.o intermediate\graphutils.o intermediate\tileutils.o intermediate\game.o intermediate\debug.o intermediate\agari.o vendor\raylib-6.2_win64_mingw-w64\lib\libraylib.a -lwinmm -lgdi32 -o bin\ponpon.exe
