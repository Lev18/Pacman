#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "headers/Global.h"
#include "headers/ConvertMap.h"
#include "headers/DrawBoard.h"


void pacman_move_dir(const int& dir, sf::Sprite& spritesheet) {
        switch(dir) {
            case 0:
                spritesheet.move(sf::Vector2f(0, -10));
                break;
            case 1:
                spritesheet.move(sf::Vector2f(0, 10));
                break;
            case 2:
                spritesheet.move(sf::Vector2f(10, 0));
                break;

            case 3:
                spritesheet.move(sf::Vector2f(-10, 0));
                break;
        }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE +2)), "SFML works!"); 
   
    std::array<std::string, MAP_HEIGHT> i_map { {
       " ###################### ",
       " #..........#.........# ",
       " #o##.#####.#.#####.#o# ",
       " #..........#.........# ",
       " #.##.#.##.##.###..#. # ",
       " #....#....#...#.....## ",
       " ####.#.##.###.####.### ",
       " #....#..#.#...#....#    ",
       " ####.###  #  ## ##.### ",
       "    #.#  #  0  # ##.#    ",
       "#####.#  ###=### ##.####",
       "     .   #1 2 3# # .     ",
       "#####.## ####### ##.####",
       " #..#.......##........# ",
       " #.##.#############.# # ",
       " #...........#........# ",
       " #.##.#####.#######.### ",
       " #o#........P........o# ",
       " ####.###.###.##.##.### ",
       " #....###.###.##.##...# ",
       " #.##.###.###.##.####.# ",
       " #....................# ", 
       " ###################### ",
       } 
      };

    std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map = convert_map(i_map);
    sf::Texture texture;

    if(!texture.loadFromFile("./Pacman16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
    int pacman_ord = 0;
    int pacman_dir = 0;
    int frame_speed = 0;
    int anim_speed = 301;
    sf::Sprite sprite(texture);
    sprite.setScale(1.0f, 1.0f);

      while (window.isOpen()) {    
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type)  {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Right:
                            pacman_move_dir(2, sprite);
                            pacman_dir = 0;
                            break;
                        case sf::Keyboard::Left:
                            pacman_move_dir(3, sprite);
                            pacman_dir = 32;
                            break;
                        case sf::Keyboard::Up:
                            pacman_move_dir(0, sprite);
                            pacman_dir = 16;
                            break;
                        case sf::Keyboard::Down:
                            pacman_move_dir(1, sprite);
                            pacman_dir = 48;
                            break;
                    }
            }
        }
        window.clear();
        draw_map(map,window);
        

        if (frame_speed % anim_speed == 0) {
            pacman_ord += 16;
            if (pacman_ord >= 96) {
                pacman_ord = 0;
                //   break;
            }
        }

        sprite.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));  
        window.draw(sprite);

       ++frame_speed;
       window.display();
    }

    return 0;
}
