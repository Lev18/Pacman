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
    std::vector<std::pair<Cell, std::pair<float, float>>> get_map_position() const;
    const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> get_mapped_array() const;
    void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
    int find(std::vector<std::pair<Cell, std::pair<float, float>>>& cell_arr, std::pair<float, float> pacman_coords) const; 
    const std::vector<sf::RectangleShape> get_walls() const;
    const std::vector<sf::RectangleShape> get_pallets() const;
    void set_type(int x, int y, Cell cell_type);
 private:
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> new_arr;
    std::vector<std::pair<Cell, std::pair<float, float>>>  attrib_coord;
    std::vector<sf::RectangleShape> all_walls;
    std::vector<sf::RectangleShape> all_pallets;

};
#endif
