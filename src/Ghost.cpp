#include <filesystem>
#include <iostream>
#include <cmath>

#include "../headers/Global.h"
#include "../headers/Ghost.h"



Ghost::Ghost(float ghost_x, float ghost_y, sf::Color color, const std::string resource) :
            m_ghost_x(ghost_x), m_ghost_y(ghost_y), m_ghost_color(color), m_resource(resource) {

            std::filesystem::path curr_path = std::filesystem::current_path();
            // std::cout << curr_path / m_resource << '\n';
            if (!m_ghost_texture.loadFromFile(curr_path / m_resource)) {
                    std::cerr << "Unable open ghost resource file\n";
                    std::exit(0);
            }
            // creating ghost body from texture
            m_ghost.setTexture(m_ghost_texture);
            m_ghost.setScale(1.0f, 1.0f);
            m_ghost.setPosition(m_ghost_x, m_ghost_y);
            m_ghost.setColor(m_ghost_color);
            
            // creating ghost eyes from texture
            m_ghost_eyes.setTexture(m_ghost_texture);
            m_ghost_eyes.setScale(1.0f, 1.0f);
            m_ghost_eyes.setPosition(m_ghost_x, m_ghost_y);
}

void Ghost::ghost_animation(int order) {
    m_ghost.setTextureRect(sf::IntRect(order, 0, 16, 16));
    m_ghost_eyes.setTextureRect(sf::IntRect(16,16, 16, 16));
}

void Ghost::draw_ghost(sf::RenderWindow& window) {
    window.draw(m_ghost);
    window.draw(m_ghost_eyes);
}

void Ghost::running_to_catch(const Pacman& pacman,  const std::vector<sf::RectangleShape>& walls) {
    sf::Vector2f pac_pos = pacman.get_pacman().getPosition();
    sf::Vector2f ghost_pos = m_ghost.getPosition();

    sf::Vector2f direction = pac_pos - ghost_pos;

    float lenght = std::sqrt(direction.x * direction.y + direction.y * direction.y);

    if (lenght != 0) {
        direction /= lenght;
    }

    // sf::Vector2f new_pos = (direction*m_ghost_speed*delta_time);
     move_adj_cell(direction, walls, pac_pos);
}

bool move_to(const std::vector<sf::RectangleShape>& walls, float coord_x, float coord_y) {

    for (const sf::RectangleShape& vec : walls) {
        if (check_colision(coord_x, coord_y, vec)) {
            return false;
        }
    }

    return true;
}

bool Ghost::reached_to_dest(const sf::Vector2f& pacman_pos, const sf::Vector2f& ghsot_pos) {
    return std::abs(pacman_pos.x - ghsot_pos.x) < 16 && std::abs(pacman_pos.y - ghsot_pos.y) < 16;
}

Move_Direction Ghost::get_direction(int move_dir) {
    Move_Direction direction;
    switch (move_dir) {
        case 0:
            direction = Move_Direction::UP;
            break;
        case 1:
            direction = Move_Direction::DOWN;
            break;
        case 2:
            direction = Move_Direction::LEFT;
            break;
        case 3:
            direction = Move_Direction::RIGHT;
            break;
    }
    return direction;
}

void Ghost::move_adj_cell(const sf::Vector2f& direction, 
        const std::vector<sf::RectangleShape>& walls, 
        const sf::Vector2f& pacman_pos) {

     if(reached_to_dest(pacman_pos, m_ghost.getPosition())) {
         return;
     }
    bool can_move_up = move_to(walls, m_ghost.getPosition().x, m_ghost.getPosition().y - 8);
    bool can_move_down = move_to(walls, m_ghost.getPosition().x, m_ghost.getPosition().y + 8);
    bool can_move_left = move_to(walls, m_ghost.getPosition().x - 8, m_ghost.getPosition().y );
    bool can_move_right = move_to(walls, m_ghost.getPosition().x + 8, m_ghost.getPosition().y);
   


    std::cout << "Can move up: " << can_move_up << std::endl;
    std::cout << "Can move down: " << can_move_down << std::endl;
    std::cout << "Can move left: " << can_move_left << std::endl;
    std::cout << "Can move right: " << can_move_right << std::endl;

    float delta_time = m_clock.restart().asSeconds();
    sf::Vector2f move_offset(0.0f, 0.0f);

    if (std::abs(direction.x) > std::abs(direction.y)) {
        // Prefer horizontal movement
        if (direction.x < 0 && can_move_left) {
            move_offset.x = -8.0f;
        } 
        else if (direction.x > 0 && can_move_right) {
            move_offset.x = 8.0f;
        }
    }
    else {
        if (direction.y < 0 && can_move_up) {
            move_offset.y = -8.0f;
        } 
        else if (direction.y > 0 && can_move_down) {
            move_offset.y = 8.0f;
        }
    }
    
    if (move_offset.x == 0 && move_offset.y == 0) {
        if (direction.y < 0 && can_move_up) {
            move_offset.y = -8.0f;
        }        
        else if (direction.y > 0 && can_move_down) {
            move_offset.y = 8.0f;
        }
        else if (direction.x < 0 && can_move_left) {
            move_offset.x = -8.0f;
        }
        else if (direction.x > 0 && can_move_right) {
            move_offset.x = 8.0f;
        }
    }

  // If ghost is blocked in both directions, pick a random available move
    if (move_offset.x == 0 && move_offset.y == 0) {
        std::vector<sf::Vector2f> possible_moves;
        if (can_move_up)    possible_moves.push_back({0, -8.0f});
        if (can_move_down)  possible_moves.push_back({0, 8.0f});
        if (can_move_left)  possible_moves.push_back({-8.0f, 0});
        if (can_move_right) possible_moves.push_back({8.0f, 0});

        if (!possible_moves.empty()) {
            move_offset = possible_moves[rand() % possible_moves.size()];
        }
    }
    // Move ghost
    m_ghost.move(move_offset * m_ghost_speed * delta_time);
    m_ghost_eyes.move(move_offset * m_ghost_speed * delta_time);

    m_ghost_x = m_ghost.getPosition().x;
    m_ghost_y = m_ghost.getPosition().y;

}

