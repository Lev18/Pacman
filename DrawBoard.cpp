#include "headers/DrawBoard.h"
#include <iostream>

void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window) {
    sf::Texture texture;
    texture.setRepeated(true);

    if (!texture.loadFromFile("Map16.png")) {
      std::cerr << "Unable to load from file" << std::endl;
      return ;
    }


    sf::Sprite sprite(texture);

    sprite.setScale(1.0f, 1.0f);
    for (int i = 0; i < MAP_HEIGHT; ++i) {
      for (int j = 0; j < MAP_WIDTH; ++j) {
         sprite.setPosition(CELL_SIZE * static_cast<float>(i),CELL_SIZE * static_cast<float>(j));

         switch (map[j][i]) {
           case  Cell::Pellet: {
             sprite.setTextureRect(sf::IntRect(0, CELL_SIZE,CELL_SIZE,CELL_SIZE));
             window.draw(sprite);
             break;
            }

           case Cell::Door: {
             sprite.setTextureRect(sf::IntRect(2*CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
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
              window.draw(sprite);
            }
         }
      }
    }
}
