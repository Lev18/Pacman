#include <iostream>
#include <filesystem>
#include <cstdlib>

#include "../headers/ConvertMap.h"



Map::Map(std::array<std::string, MAP_HEIGHT>& i_map) : new_arr{} {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
      for (int j = 0; j < MAP_WIDTH; ++j) {
        
        new_arr[j][i] = Cell::Empty;
        
        switch (i_map[j][i]) {
          case '#': {
            new_arr[j][i] = Cell::Wall;
              sf::RectangleShape rect;
              rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
              rect.setFillColor(sf::Color::Blue);

              rect.setPosition(i * CELL_SIZE, j * CELL_SIZE);
              all_walls.push_back(rect);
            // attrib_coord.push_back({Cell::Wall, {CELL_SIZE * static_cast<float>(j), CELL_SIZE * static_cast<float>(i)}});
            break; 
          }

          case 'o': {
            new_arr[j][i] = Cell::Energy;
            //attrib_coord.push_back({Cell::Energy, {j, i}});
            break; 
          }

          case '.': {
            new_arr[j][i] = Cell::Pellet;
              sf::RectangleShape rect;
              rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
              rect.setFillColor(sf::Color::Blue);

              rect.setPosition(i * CELL_SIZE, j * CELL_SIZE);
              all_pallets.push_back(rect);

            //attrib_coord.push_back({Cell::Pellet, {j, i}});
            break; 
          }

          case '=': {
            new_arr[j][i] = Cell::Door;
           // attrib_coord.push_back({Cell::Energy, {j, i}});
            break; 
          }
          case 'P': {
            new_arr[j][i] = Cell::Pacman;
            attrib_coord.push_back({Cell::Pacman, {i, j}});
            break;
          }
        // this part is hardcoded
          case '1':{
            new_arr[j][i] = Cell::Ghost;
            attrib_coord.push_back({Cell::Ghost, {i, j}});
            break;
          }
          case '2':{
            new_arr[j][i] = Cell::Ghost;
            attrib_coord.push_back({Cell::Ghost, {i, j}});
            break;
          }
         case '3':{
            new_arr[j][i] = Cell::Ghost;
            attrib_coord.push_back({Cell::Ghost, {i, j}});
            break;
          }
        }
      }
    }
}

std::vector<std::pair<Cell, std::pair<float, float>>> Map::get_map_position() const {
    return attrib_coord;
}

const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> Map::get_mapped_array() const {
    return new_arr;
}

void Map::set_type(int i, int j, Cell cell) {
    new_arr[j][i] = cell ;
}

const std::vector<sf::RectangleShape> Map::get_walls() const {
    return all_walls;
}

const std::vector<sf::RectangleShape> Map::get_pallets() const {
    return all_pallets;
}

void Map::draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window) {
    sf::Texture texture;
    texture.setRepeated(true);

    std::filesystem::path curr_path   = std::filesystem::current_path();
    std::filesystem::path parent_path = curr_path.parent_path();

    if (!texture.loadFromFile(curr_path / "resources/Map16.png")) {
      std::cerr << "Unable to load map from file" << std::endl;
      return ;
    }


    sf::Sprite sprite(texture);

    sprite.setScale(1.0f, 1.0f);

    for (int i = 0; i < MAP_HEIGHT; ++i) {
      for (int j = 0; j < MAP_WIDTH; ++j) {
         sprite.setPosition(CELL_SIZE * static_cast<float>(i),CELL_SIZE * static_cast<float>(j));

         switch (map[j][i]) {
           case  Cell::Pellet: {
             sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));
             // attrib_coord.push_back({Cell::Pellet, {CELL_SIZE * i, CELL_SIZE * j}});
             window.draw(sprite);
             break;
            }

           case Cell::Door: {
             sprite.setTextureRect(sf::IntRect(2*CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
            // attrib_coord.push_back({Cell::Door, {i, j}});
             window.draw(sprite);
             break;
            }

           case Cell::Energy: {
             sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
             window.draw(sprite);
             break;
           }
           case Cell::Wall: {
               bool up = (j > 0) && (map[j - 1][i] == Cell::Wall);
               bool down = (j < MAP_HEIGHT - 1) && (map[j + 1][i] == Cell::Wall);
               bool left = (i > 0) && (map[j][i - 1] == Cell::Wall);
               bool right = (i < MAP_WIDTH - 1) && (map[j][i + 1] == Cell::Wall);

               int textureIndex = down + 2 * (left + 2 * (right + 2 * up));

              sprite.setTextureRect(sf::IntRect(CELL_SIZE * textureIndex, 0, CELL_SIZE, CELL_SIZE));
              /*
              sf::RectangleShape rect;
              rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
              rect.setFillColor(sf::Color::Blue);

              rect.setPosition(i * CELL_SIZE, j* CELL_SIZE);
              */
              window.draw(sprite);
              break;
            }
           case(Cell::Pacman): {
                attrib_coord.push_back({Cell::Pacman, {i, j}});
                break;
            }

            case (Cell::Empty): {
              sf::RectangleShape rect;
              rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
              rect.setFillColor(sf::Color::Black);

              rect.setPosition(i * CELL_SIZE, j * CELL_SIZE);
              window.draw(rect);
              break;
            }
            case (Cell::Ghost): {
            }
         }
      }
    }
}

int Map::find(std::vector<std::pair<Cell, std::pair<float, float>>>& cell_arr, std::pair<float, float> pacman_coords) const {
    int low = 0;
    int high = cell_arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

    std::cout << pacman_coords.second << ' ' <<pacman_coords.first  << ' ' << cell_arr[mid].second.first << ' ' <<cell_arr[mid].second.second << '\n';
        if (cell_arr[mid].second.first == pacman_coords.second 
                && cell_arr[mid].second.second == pacman_coords.first) {
            return 1;
        }
        if (pacman_coords.second <= cell_arr[mid].second.second) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return -1;

}
