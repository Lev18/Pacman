#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

constexpr int CELL_SIZE = 16;

constexpr int MAP_HEIGHT = 23;
constexpr int MAP_WIDTH = 23;

enum class Cell {
  Door,
  Empty,
  Wall,
  Energy,
  Pellet,
  Pacman,
  Ghost
};


#endif
