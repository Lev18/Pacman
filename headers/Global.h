#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include <SFML/Graphics.hpp>

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

enum class Move_Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

bool check_colision(float coord_x, float coord_y, const sf::RectangleShape& walls);


#endif
