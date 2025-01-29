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
    private:
        const float m_ghost_speed = 100.0f;
        sf::Clock m_clock;
};
