#ifndef CONVET_MAP_H
#define CONVET_MAP_H

#include <array>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "../headers/Global.h"


class Map {
 public:
    Map(std::array<std::string, MAP_HEIGHT>& i_map);
    std::vector<std::pair<Cell, std::pair<int, int>>> get_map_position() const;
    const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> get_mapped_array() const;
    void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
 private:
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> new_arr;
    std::vector<std::pair<Cell, std::pair<int, int>>>  attrib_coord;
};
#endif
