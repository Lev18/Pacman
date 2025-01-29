#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>


int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Window");

    sf::CircleShape pacman(20,30);
    pacman.setFillColor(sf::Color(255,255, 0));

    sf::CircleShape pac(20,30);
    pac.setFillColor(sf::Color(255,255, 0));
    pac.setPosition({150, 120});

    sf::CircleShape ghost(20,30);
    ghost.setFillColor(sf::Color(255, 0, 0));
    ghost.setPosition({400, 300});

    int opposite = 0;
    int adjacent = 0.0; 

    float angle = 0.0;

    sf::Clock clock;

    const float ghost_speed = 100.0f;
    float last_move_time = 0.0f;

    sf::Mouse mouse;
    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
             
           
        }
          sf::Vector2i mouse_pos = mouse.getPosition(window);
          pacman.setPosition({static_cast<float>(mouse_pos.x - 25), static_cast<float>( mouse_pos.y - 25)});

          sf::Vector2f pacman_pos = pacman.getPosition();
          sf::Vector2f ghost_pos = ghost.getPosition();
          sf::Vector2f direction = pacman_pos - ghost_pos;


          float lenght = std::sqrt(direction.x * direction.x + direction.y * direction.y);

          if (lenght != 0) {
              direction /= lenght;
          }
            
          float delta_time = clock.restart().asSeconds();
          ghost.move(direction*ghost_speed*delta_time);
                  /*
          if (last_move_time >= ghost_move_interval) {
            int ghost_pos_x = ghost.getPosition().x;
            int ghost_pos_y = ghost.getPosition().y; 

            opposite = pac.getPosition().x - ghost_pos_x;    
            adjacent = pac.getPosition().y - ghost_pos_y;

            if (adjacent != 0) { 
                angle = std::atan(opposite/adjacent);
            }
            else {
                if (opposite > 0) {
                    angle = 3.14159265f / 2; // 90 degrees in radians
                }
                else if (opposite < 0) {
                    angle = 3.14159265f / 2; // -90 degrees in radians
                }
                else {
                    angle = 0;
                }
            }
        
             
            if (pacman.getPosition().x < ghost_pos_x) {
              angle += 3.14159265f; // 180 degree in radians
            }

            int velocity = 1;

            float vx = velocity * cos(angle);
            float vy = velocity * sin(angle);
           
            std::cout << vx << ' ' << vy <<'\n';

            ghost.move(vx, vy);
          }
          */

               // std::cout << angle << '\n';
        window.clear();
        window.draw(pacman);
        window.draw(pac);
        window.draw(ghost);
        window.display();
    }
    return 0;
}
