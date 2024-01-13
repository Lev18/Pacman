#ifndef CONVET_MAP_H
#define CONVET_MAP_H

#include <array>
#include "../headers/Global.h"

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> convert_map(std::array<std::string, MAP_HEIGHT>& i_map);

#endif
