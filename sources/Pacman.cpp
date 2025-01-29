#include "../headers/Pacman.h"
#include "../headers/Global.h"
#include <iostream>
#include <filesystem>

Pacman::Pacman(float pac_x, float pac_y, const std::string text_path) :
                                                     m_pacman_x(pac_x), m_pacman_y(pac_y),
                                                      m_pac_path(text_path){
    std::filesystem::path curr_path = std::filesystem::current_path();
    std::cout << curr_path / m_pac_path << '\n';
    
    m_pacman.setScale(1.0f, 1.0f);
    m_pacman.setPosition(m_pacman_x, m_pacman_y);

    if(!m_pac_texture.loadFromFile(curr_path / m_pac_path)){
            std::cerr << "Unable to load from file";
            exit(1);
    }

    m_pacman.setTexture(m_pac_texture);
   
    pacman_ord = 0;
    pacman_dir = 0;
    frame_speed = 0;
}

void Pacman::draw_packman(sf::RenderWindow& window) {
    window.draw(m_pacman);
}

const sf::Sprite Pacman::get_pacman() const {
    return m_pacman;
}

void Pacman::animation_render(const int anim_speed, int pac_ord, int pc_dir) {
    if (frame_speed % anim_speed == 0) {
        pacman_ord += 16;
        if (pacman_ord >= 96) {
            pacman_ord = 0;
              //   break;
        }
    }
    m_pacman.setTextureRect(sf::IntRect(pacman_ord, pacman_dir, 16, 16));
}


bool Pacman::is_pacman_eats(float pac_x, float pac_y, sf::RectangleShape& pallet) {
    float pac_height = 16;
    float pac_width  = 16;

    sf::FloatRect pal_bounds = pallet.getGlobalBounds();
    sf::FloatRect pac_bounds(pac_x, pac_y, pac_width, pac_height);
    return pac_bounds.intersects(pal_bounds);
}

void Pacman::eating_pallet(Map& map,  std::vector<sf::RectangleShape>& all_pallets) {
    float new_x = m_pacman.getPosition().x;
    float new_y = m_pacman.getPosition().y;

    for (auto& it : all_pallets) {
        if (is_pacman_eats(new_x, new_y, it)) {
            map.set_type((int)(new_x / CELL_SIZE), (int)(new_y / CELL_SIZE), Cell::Empty);
          std::cout << "it intesects with pallet" << '\n';
        }
    }

}

void Pacman::move_to_dir(Move_Direction dir, std::vector<sf::RectangleShape>& all_rect) {
    sf::Vector2f offset(0.0f, 0.0f);
    float step_size = 8.0;
    switch(dir) {
        case Move_Direction::UP:
                offset.y = -step_size; 
                pacman_dir = 16;
                break;
        case Move_Direction::DOWN:
                offset.y = step_size; 
                pacman_dir = 48;
                break;
        case Move_Direction::RIGHT:
                offset.x = step_size; 
                pacman_dir = 0;
                break;
        case Move_Direction::LEFT:
                offset.x = -step_size;
                pacman_dir = 32;
                break;
        }

    float new_x = m_pacman.getPosition().x + offset.x;
    float new_y = m_pacman.getPosition().y + offset.y;
    bool can_move = true;

    for (auto& rect : all_rect) {
        if (check_colision(new_x, new_y, rect)) {
            can_move = false;
            break;
        }
    }
    if (can_move) {
        m_pacman.move(offset);
    }
    else {
        std::cout << "Collision detected, movement blocked!" << std::endl;

    }
}

void Pacman::increase_frm_speed() {
    ++frame_speed;
}
