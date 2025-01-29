#!/bin/bash

g++ -std=c++2a ./src/main.cpp ./src/GameEngine.cpp ./src/ConvertMap.cpp ./src/Pacman.cpp ./src/Global.cpp  ./src/Ghost.cpp -Wall -Wextra  -o pacman -lsfml-graphics -lsfml-window -lsfml-system -Im -ggdb
