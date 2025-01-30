#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "../headers/Global.h"
#include "../headers/ConvertMap.h"
#include "../headers/GameEngine.h"
#include "../headers/Pacman.h"
#include "../headers/Ghost.h"
/*
bool check_colision(float pac_x, float pac_y, const sf::RectangleShape& walls) {

    float pac_height = 16;
    float pac_width  = 16;
    sf::FloatRect wall_bounds = walls.getGlobalBounds();
    sf::FloatRect pac_bounds(pac_x, pac_y, pac_width, pac_height);

    return pac_bounds.intersects(wall_bounds);

}
*/
void GameEngine::game_start() {
   
    std::array<std::string, MAP_HEIGHT> i_map { {
       " ###################### ",
       " #..........#.........# ",
       " #o##.#####.#.#####.#o# ",
       " ##.........#........## ",
       " #.##.#.##.##.###..#. # ",
       " #....#....#...#.....## ",
       " ####.#.##.###.####.### ",
       " #....#..  ........#    ",
       " ####.#  ##   ## ##.### ",
       "    #.#  #  0  # # .  # ",
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

    // Map conversion 
    Map map(i_map);

    std::vector<sf::RectangleShape> walls = map.get_walls();
    auto all_pallets = map.get_pallets(); 
    auto attrib_coords = map.get_map_position();

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE + 2)), "Pacman"); 

    // Creating Pacman 
    Pacman pacman((float)(attrib_coords[2].second.first *  CELL_SIZE),
                  (float)(attrib_coords[2].second.second * CELL_SIZE),
                        "resources/Pacman16.png");
   
    int pacman_ord = 0;
    int frame_speed = 0;
    const int anim_speed = 100;

    // creating ghosts 
    std::string ghost_resource = "resources/Ghost16.png";
    Ghost yellow_ghost((float)(attrib_coords[0].second.first * CELL_SIZE), 
                      (float)(attrib_coords[0].second.second * CELL_SIZE),
                      sf::Color::Yellow, ghost_resource);
    Ghost red_ghost((float)(attrib_coords[1].second.first * CELL_SIZE),
                    (float)(attrib_coords[1].second.second * CELL_SIZE),
                    sf::Color::Red, ghost_resource);
    Ghost blue_ghost((float)(attrib_coords[3].second.first * CELL_SIZE),
                    (float)(attrib_coords[3].second.second * CELL_SIZE),
                    sf::Color::Blue, ghost_resource); 
       // std::cout << map.get_mapped_array().size() << ' ' << texture.getSize().y << std::endl;
    while (window.isOpen()) {    
        sf::Event event;

        while (window.pollEvent(event)) {
            
            switch (event.type)  {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                case sf::Event::KeyPressed:
                    pacman.eating_pallet(map, all_pallets);
                    switch (event.key.code) {
                        case sf::Keyboard::Right:
                            pacman.move_to_dir(Move_Direction::RIGHT, walls);
                            break;
                        case sf::Keyboard::Left:
                            pacman.move_to_dir(Move_Direction::LEFT, walls);
                            break;
                        case sf::Keyboard::Up:
                            pacman.move_to_dir(Move_Direction::UP, walls);
                            break;
                        case sf::Keyboard::Down:
                            pacman.move_to_dir(Move_Direction::DOWN, walls);
                            break;
                        default: {
                        }
                    }
                default: {
                
                }
            }
        }

        // TODO change when it possible
        if (frame_speed % anim_speed == 0) {
            pacman_ord += 16;
            if (pacman_ord >= 96) {
                pacman_ord = 0;
                //   break;
            }
        }


       window.clear(); 
       map.draw_map(map.get_mapped_array(), window);

       pacman.animation_render(anim_speed);

       // TODO: integrate ghost movement
        red_ghost.running_to_catch(pacman, walls);
       
       yellow_ghost.ghost_animation(pacman_ord);
       blue_ghost.ghost_animation(pacman_ord);
       red_ghost.ghost_animation(pacman_ord);

       pacman.increase_frm_speed();

       yellow_ghost.draw_ghost(window);
       blue_ghost.draw_ghost(window);
       red_ghost.draw_ghost(window);
       pacman.draw_packman(window);
     
       window.display();
    }

}
