#include <SFML/Graphics.hpp>
#include <iostream>
#include "headers/Global.h"
#include "headers/ConvertMap.h"
#include "headers/DrawBoard.h"

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
      while (window.isOpen()) {    
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
       window.clear();
       draw_map(map,window);
       window.display();
    }

    return 0;
}
