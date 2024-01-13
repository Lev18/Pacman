#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include "Global.h"
#include <SFML/Graphics.hpp>
#include <array> 

void draw_map(const std::array<std::array<Cell, MAP_WIDTH>,MAP_HEIGHT>& mp, sf::RenderWindow& window);

#endif
