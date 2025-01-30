#include <SFML/Graphics.hpp>
#include <vector>
#include "../headers/Pacman.h"

class Ghost {
    public:
        Ghost (float ghost_x, float ghost_y, sf::Color color, const std::string resource);
        void ghost_animation(int order);
        void draw_ghost(sf::RenderWindow& window);
        void running_to_catch(const Pacman& pacman, const std::vector<sf::RectangleShape>& walls);
    private:
        float m_ghost_x;
        float m_ghost_y;
        sf::Color m_ghost_color;
        const std::string m_resource;


        sf::Sprite m_ghost;
        sf::Sprite m_ghost_eyes;
        sf::Texture m_ghost_texture;
        
        const float m_ghost_speed = 10.0f;
        sf::Clock m_clock;
    private:
        void move_adj_cell(const sf::Vector2f& direction, const std::vector<sf::RectangleShape>& walls,
                          const sf::Vector2f& pacman_pos);      
        bool reached_to_dest(const sf::Vector2f& direction, const sf::Vector2f& pacman_pos);
        Move_Direction get_direction(int rand_dir);

};
