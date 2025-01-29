#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "./headers/Global.h"
#include "./headers/ConvertMap.h"
#include "./headers/GameEngine.h"
#include "./headers/Pacman.h"
/*
bool check_colision(float pac_x, float pac_y, const sf::RectangleShape& walls) {

    float pac_height = 16;
    float pac_width  = 16;
    sf::FloatRect wall_bounds = walls.getGlobalBounds();
    sf::FloatRect pac_bounds(pac_x, pac_y, pac_width, pac_height);

    return pac_bounds.intersects(wall_bounds);

}
*/

bool is_pacman_eats(float pac_x, float pac_y, sf::RectangleShape& pallet) {
    float pac_height = 16;
    float pac_width  = 16;

    sf::FloatRect pal_bounds = pallet.getGlobalBounds();
    sf::FloatRect pac_bounds(pac_x, pac_y, pac_width, pac_height);
    return pac_bounds.intersects(pal_bounds);
}

void eating_pallet(sf::Sprite& spritesheet, Map& map,  std::vector<sf::RectangleShape>& all_pallets) {
    float new_x = spritesheet.getPosition().x;
    float new_y = spritesheet.getPosition().y;

    for (auto& it : all_pallets) {
        if (is_pacman_eats(new_x, new_y, it)) {
            map.set_type((int)(new_x / CELL_SIZE), (int)(new_y / CELL_SIZE), Cell::Empty);
          std::cout << "it intesects with pallet" << '\n';
        }
    }

}

void pacman_move_dir(const int& dir, sf::Sprite& spritesheet, std::vector<sf::RectangleShape>& all_rect) {

    sf::Vector2f offset(0.0f, 0.0f);
    float step_size = 8.0;
    switch(dir) {
            case 0:
                offset.y = -step_size; break;
            case 1:
                offset.y = step_size; break;
            case 2:
                offset.x = step_size; break;
            case 3:
                offset.x = -step_size; break;
        }

    float new_x = spritesheet.getPosition().x + offset.x;
    float new_y = spritesheet.getPosition().y + offset.y;
    bool can_move = true;

    for (auto& rect : all_rect) {
        if (check_colision(new_x, new_y, rect)) {
            can_move = false;
            break;
        }
    }
    if (can_move) {
        spritesheet.move(offset);
    }
    else {
        std::cout << "Collision detected, movement blocked!" << std::endl;

    }
}

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
    Map map(i_map);

    std::vector<sf::RectangleShape> walls = map.get_walls();
    auto all_pallets = map.get_pallets();
    
    auto attrib_coords = map.get_map_position();

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE + 2)), "SFML works!"); 

    Pacman pacman((float)(attrib_coords[2].second.first *  CELL_SIZE),
                  (float)(attrib_coords[2].second.second * CELL_SIZE),
                        "resources/Pacman16.png");
   
    int pacman_ord = 0;
    int pacman_dir = 0;
    int frame_speed = 0;
    const int anim_speed = 100;

    sf::Texture ghost_tex;

    if(!ghost_tex.loadFromFile("./resources/Ghost16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
   

    sf::Sprite ghost1(ghost_tex);
    ghost1.setScale(1.0f, 1.0f);
    ghost1.setPosition((float)(attrib_coords[0].second.first * (CELL_SIZE)), (float)(attrib_coords[0].second.second * (CELL_SIZE)));
    ghost1.setColor(sf::Color::Red);
    
    sf::Sprite ghost3(ghost_tex);
    ghost3.setScale(1.0f, 1.0f);
    ghost3.setPosition((float)(attrib_coords[3].second.first * (CELL_SIZE)), (float)(attrib_coords[3].second.second * (CELL_SIZE)));

    ghost3.setColor(sf::Color::Yellow);
   
    sf::Sprite ghost_eyes1(ghost_tex);
    ghost_eyes1.setScale(1.0f, 1.0f);
    ghost_eyes1.setPosition((float)(attrib_coords[3].second.first * (CELL_SIZE)), (float)(attrib_coords[3].second.second * (CELL_SIZE)));
   
    sf::Sprite ghost_eyes2(ghost_tex);
    ghost_eyes2.setScale(1.0f, 1.0f);
    ghost_eyes2.setPosition((float)(attrib_coords[0].second.first * (CELL_SIZE)), (float)(attrib_coords[0].second.second * (CELL_SIZE)));

    std::string file_path {"./energy_cord.txt"};
    std::fstream file {file_path, std::ios::out | std::ios::trunc};

    if (!file.is_open()) {
        std::cerr << "Unable to load file" << '\n';
    }

    for (const auto rect : walls) {
      //  file << rect.getSize().x << '\t' << rect.getSize().y << '\n';
        file << rect.getPosition().x << '\t' << rect.getPosition().y << '\n';
    }
    file.close();

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

       pacman.animation_render(anim_speed, pacman_ord, pacman_dir);
       // sprite.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));  
       ghost1.setTextureRect(sf::IntRect(pacman_ord, 0, 16,16));
       ghost3.setTextureRect(sf::IntRect(pacman_ord, 0, 16,16));
       ghost_eyes1.setTextureRect(sf::IntRect(16, 16, 16, 16));
       ghost_eyes2.setTextureRect(sf::IntRect(16, 16, 16, 16));

       pacman.increase_frm_speed();

       window.draw(ghost1);
       window.draw(ghost3);
       window.draw(ghost_eyes1);
       window.draw(ghost_eyes2);
       pacman.draw_packman(window);
       // window.draw(pacman.get_pacman());
       // window.draw(sprite);
       window.display();
    }

}
