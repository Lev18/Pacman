#!/bin/bash

g++ -std=c++2a main.cpp GameEngine.cpp ./sources/ConvertMap.cpp ./sources/Pacman.cpp ./sources/Global.cpp -Wall -Wextra  -o pacman -lsfml-graphics -lsfml-window -lsfml-system -Im -ggdb
