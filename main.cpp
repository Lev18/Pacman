#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "headers/Global.h"
#include "headers/ConvertMap.h"


bool check_colision(float pac_x, float pac_y, const sf::RectangleShape& walls) {
    
    float pac_height = 16;
    float pac_width  = 16;
   /* 
    sf::Vector2f rect_size = walls.getSize();
    sf::Vector2f rect_pos = walls.getPosition();
    
    return pac_x + pac_height >= rect_pos.x &&
            pac_x <= rect_pos.x + rect_size.x &&
             pac_y + pac_width >= rect_pos.y &&
              pac_y <= rect_pos.y + rect_size.y;
              */
            
   sf::FloatRect wall_bounds = walls.getGlobalBounds();
   sf::FloatRect pac_bounds(pac_x, pac_y, pac_width, pac_height);

    return pac_bounds.intersects(wall_bounds);

} 

bool is_pacman_eats(float pac_x, float pac_y, sf::RectangleShape& pallet) {
    float pac_height = 16;
    float pac_width  = 16;
   
   /* sf::Vector2f pallet_coords = pallet.getPosition();

    if (pallet_coords.x == pac_x && pallet_coords.y == pac_y) {
        return true;
    }*/
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

int main() {
   
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
    
    auto attrib_cords = map.get_map_position();

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE + 2)), "SFML works!"); 

    sf::Texture texture;

    if(!texture.loadFromFile("./Pacman16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
    int pacman_ord = 0;
    int pacman_dir = 0;
    int frame_speed = 0;
    int anim_speed = 100;

    sf::Sprite sprite(texture);
    sprite.setScale(1.0f, 1.0f);
    sprite.setPosition((float)(attrib_cords[2].second.first * (CELL_SIZE)), (float)(attrib_cords[2].second.second * (CELL_SIZE)));

    sf::Texture ghost_tex;

    if(!ghost_tex.loadFromFile("./Ghost16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
   

    sf::Sprite ghost1(ghost_tex);
    ghost1.setScale(1.0f, 1.0f);
    ghost1.setPosition((float)(attrib_cords[0].second.first * (CELL_SIZE)), (float)(attrib_cords[0].second.second * (CELL_SIZE)));
    ghost1.setColor(sf::Color::Red);
    
    sf::Sprite ghost3(ghost_tex);
    ghost3.setScale(1.0f, 1.0f);
    ghost3.setPosition((float)(attrib_cords[3].second.first * (CELL_SIZE)), (float)(attrib_cords[3].second.second * (CELL_SIZE)));
    ghost3.setColor(sf::Color::Yellow);
   
    sf::Sprite ghost_eyes(ghost_tex);
    ghost_eyes.setScale(1.0f, 1.0f);
    ghost_eyes.setPosition((float)(attrib_cords[3].second.first * (CELL_SIZE)), (float)(attrib_cords[3].second.second * (CELL_SIZE)));


    auto cell_cord = map.get_map_position();

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

    std::cout << map.get_mapped_array().size() << ' ' << texture.getSize().y << std::endl;
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
                            eating_pallet(sprite, map, all_pallets);
                            pacman_move_dir(2, sprite, walls);
                            pacman_dir = 0;
                            break;
                        case sf::Keyboard::Left:
                            eating_pallet(sprite, map, all_pallets);
                            pacman_move_dir(3, sprite, walls);
                            pacman_dir = 32;
                            break;
                        case sf::Keyboard::Up:
                            eating_pallet(sprite, map, all_pallets);
                            pacman_move_dir(0, sprite, walls);
                            pacman_dir = 16;
                            break;
                        case sf::Keyboard::Down:
                            eating_pallet(sprite, map, all_pallets);
                            pacman_move_dir(1, sprite, walls);
                            pacman_dir = 48;
                            break;
                        default: {
                        }
                    }
                default: {
                
                }
            }
        }

        if (frame_speed % anim_speed == 0) {
            pacman_ord += 16;
            if (pacman_ord >= 96) {
                pacman_ord = 0;
                //   break;
            }
        }


       window.clear(); 
       map.draw_map(map.get_mapped_array(), window);

       sprite.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));  
       ghost1.setTextureRect(sf::IntRect(pacman_ord, 0, 16,16));
       ghost3.setTextureRect(sf::IntRect(pacman_ord, 0, 16,16));
       ghost_eyes.setTextureRect(sf::IntRect(16, 16, 16, 16));

       ++frame_speed;

       window.draw(ghost1);
       window.draw(ghost3);
       window.draw(ghost_eyes);
       window.draw(sprite);
       window.display();
    }

    return 0;
}
