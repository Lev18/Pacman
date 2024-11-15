#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "../headers/Global.h"


bool check_colision(float pac_x, float pac_y, sf::RectangleShape& rect) {
    float pac_height = 16;
    float pac_width  = 16;
    sf::Vector2f rect_size = rect.getSize();
    sf::Vector2f rect_pos = rect.getPosition();
    
    return pac_x + pac_width  >= rect_pos.x &&
            pac_x <= rect_pos.x + rect_size.x &&
             pac_y + pac_height >= rect_pos.y &&
              pac_y <= rect_pos.y + rect_size.y;

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
                offset.x =  -step_size; break;
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
}


int main() {
    
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE + 2)), "SFML works!"); 

    sf::Texture texture;

    if(!texture.loadFromFile("../Pacman16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
    int pacman_ord = 0;
    int pacman_dir = 0;
    int frame_speed = 0;
    int anim_speed = 301;
    sf::Sprite sprite(texture);
    sprite.setScale(1.0f, 1.0f);
    sprite.setPosition((MAP_WIDTH * (CELL_SIZE + 2)) / 2 - 15, (MAP_HEIGHT * (CELL_SIZE + 2)) / 2 - 31);

    std::string file_path {"./energy_cord.txt"};
    std::fstream file {file_path, std::ios::out | std::ios::trunc};

    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(16, 96));
    rect1.setFillColor(sf::Color::Red);
    rect1.setPosition(300, 200);
   
    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(16, 16));
    rect2.setFillColor(sf::Color::Red);
    rect2.setPosition(316, 184);
    
    sf::RectangleShape rect3;
    rect3.setSize(sf::Vector2f(96, 16));
    rect3.setFillColor(sf::Color::Red);
    rect3.setPosition(204, 296);

    std::vector<sf::RectangleShape> all_rect;
    all_rect.push_back(rect1);
    all_rect.push_back(rect2);
    all_rect.push_back(rect3);

    while (window.isOpen()) {    
        sf::Event event;
        
        float pacman_x_cord = sprite.getPosition().x;
        float pacman_y_cord = sprite.getPosition().y;
        std::cout << pacman_x_cord << ' ' << pacman_y_cord << '\n';

        while (window.pollEvent(event)) {
            switch (event.type)  {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::Right:
                                pacman_move_dir(2, sprite, all_rect);
                                pacman_dir = 0;
                            break;
                        case sf::Keyboard::Left:
                                pacman_move_dir(3, sprite, all_rect);
                                pacman_dir = 32;
                            break;
                        case sf::Keyboard::Up:
                                pacman_move_dir(0, sprite, all_rect);
                                pacman_dir = 16;
 
                            break;
                        case sf::Keyboard::Down:
                                pacman_move_dir(1, sprite, all_rect);
                                pacman_dir = 48;

                            break;
                    }
                }
            }
        }
        window.clear(); 

        if (frame_speed % anim_speed == 0) {
            pacman_ord += 16;
            if (pacman_ord >= 96) {
                pacman_ord = 0;
                //   break;
            }
        }

        
        sprite.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));  

       ++frame_speed;
       window.draw(rect1);
       window.draw(rect2);
       window.draw(rect3);
       window.draw(sprite);
       window.display();
    }

    return 0;
}
