@echo off
g++ -std=c++14 ../Source/main.cpp ../Source/midiplayer.cpp ../Source/notation.cpp ../Source/test.cpp ../Source/parse.cpp -lWinmm -o midiplayer
