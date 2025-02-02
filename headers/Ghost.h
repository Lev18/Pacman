#include <SFML/Graphics.hpp>
#include <vector>
#include "../headers/Pacman.h"

class Ghost {
public:
    Ghost (float ghost_x, float ghost_y, sf::Color color, const std::string resource);
    void ghost_animation(int order);
    void draw_ghost(sf::RenderWindow& window);
    void running_to_catch(const std::array<std::string, MAP_HEIGHT>& map);
private:
    float m_ghost_x;
    float m_ghost_y;
    sf::Color m_ghost_color;
    Move_Direction m_direction;
    const std::string m_resource;
   
 
    sf::Sprite m_ghost;
    sf::Sprite m_ghost_eyes;
    sf::Texture m_ghost_texture;
    sf::Vector2f m_target_pos;
    sf::Vector2f target_corner = sf::Vector2f(0, 0);
        
    const float m_ghost_speed = 2.0f;
    sf::Clock m_clock;

private:
    Move_Direction get_opposite_direction();
    std::vector<Move_Direction> available_moves(const std::array<std::string, MAP_HEIGHT>& map);
    void move_forward(const std::array<std::string, MAP_HEIGHT>& map);
};
