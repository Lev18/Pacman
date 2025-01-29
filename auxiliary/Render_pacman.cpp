#include <SFML/Graphics.hpp>
#include <iostream>
#include "./headers/Global.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * (CELL_SIZE + 2), MAP_HEIGHT * (CELL_SIZE + 2)), "SFML works!"); 

    sf::Texture pac_text;

    if(!pac_text.loadFromFile("./resources/Pacman16.png")){
            std::cerr << "Unable to load from file";
            exit(1);
    }
   

    sf::Sprite pacman(pac_text);
    pacman.setScale(1.0f, 1.0f);
   
    // std::cout << map.get_mapped_array().size() << ' ' << texture.getSize().y << std::endl;
    int pacman_ord = 0;
    int pacman_dir = 0;
    int frame_speed = 0;
    const int anim_speed = 100;

    while (window.isOpen()) {    
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    exit(0);
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

       pacman.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));  
       window.draw(pacman);
       // window.draw(sprite);
       window.display();
    }

}

