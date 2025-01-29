#include <filesystem>
#include <iostream>
#include <cmath>

#include "../headers/Global.h"
#include "../headers/Ghost.h"

Ghost::Ghost(float ghost_x, float ghost_y, sf::Color color, const std::string resource) :
            m_ghost_x(ghost_x), m_ghost_y(ghost_y), m_ghost_color(color), m_resource(resource) {

            std::filesystem::path curr_path = std::filesystem::current_path();
            std::cout << curr_path / m_resource << '\n';
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

    float delta_time = m_clock.restart().asSeconds();
    sf::Vector2f new_pos = (direction*m_ghost_speed*delta_time);

    bool can_move = false;
    for (auto& rect : walls) {
        if (check_colision(new_pos.x, new_pos.y, rect)) {
            can_move = true;
        }
    }
      if (can_move) {
        m_ghost.move(new_pos);
        m_ghost_eyes.move(new_pos);
    }
}
