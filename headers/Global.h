#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include <SFML/Graphics.hpp>

constexpr int CELL_SIZE = 16;
constexpr int MAP_HEIGHT = 23;
constexpr int MAP_WIDTH = 23;

constexpr int WINDOW_HEIGHT = MAP_HEIGHT * (CELL_SIZE);
constexpr int WINDOW_WIDTH =  MAP_WIDTH * (CELL_SIZE);

constexpr int GRID_SIZE = WINDOW_WIDTH / MAP_WIDTH;

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
    UP,    // NORTH
    DOWN,  // SOUTH
    RIGHT, // EAST
    LEFT   // WEST
};

bool check_colision(float coord_x, float coord_y, const sf::RectangleShape& walls);


#endif
