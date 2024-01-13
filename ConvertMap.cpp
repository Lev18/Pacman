#include "headers/ConvertMap.h"

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> convert_map(std::array<std::string, MAP_HEIGHT>& i_map) {
    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> new_arr;
    for (int i = 0; i < MAP_HEIGHT; ++i) {
      for (int j = 0; j < MAP_WIDTH; ++j) {
        
        new_arr[j][i] = Cell::Empty;
        
        switch (i_map[j][i]) {
          case '#': {
            new_arr[j][i] = Cell::Wall;
            break; 
          }

          case 'o': {
            new_arr[j][i] = Cell::Energy;
            break; 
          }

          case '.': {
            new_arr[j][i] = Cell::Pellet;
            break; 
          }

          case '=': {
            new_arr[j][i] = Cell::Door;
            break; 
          }
        }
      }
    }
    return new_arr;
}
