#ifndef PACKMAN_H
#define PACKMAN_H
#include <SFML/Graphics.hpp>
class Packman {
public:
  Packman();
  void draw_packman(sf::RenderWindow& window);

private:
    bool m_is_dead;
    bool m_animation_is_ower;
    unsigned char m_direction;
    unsigned short m_animation_time;
};
#endif
