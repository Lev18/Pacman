#ifndef PACKMAN_H
#define PACKMAN_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "Global.h"
#include "ConvertMap.h"

class Pacman {
public:
  Pacman(float pac_x, float pac_y, const std::string texture_path);
  void draw_packman(sf::RenderWindow& window);
  void animation_render(const int anim_speed, int pac_ord, int pac_dir);
  void move_to_dir(Move_Direction dir, std::vector<sf::RectangleShape>& all_rect);
  void eating_pallet(Map& map,  std::vector<sf::RectangleShape>& all_pallets);
  void increase_frm_speed();
  const sf::Sprite get_pacman() const;

private:
  int pacman_ord;
  int pacman_dir; 
  int frame_speed;

  float m_pacman_x;
  float m_pacman_y;

  sf::Sprite m_pacman;
  sf::Texture m_pac_texture;
  std::string m_pac_path;

private:
  bool is_pacman_eats(float pac_x, float pac_y, sf::RectangleShape& pallet);

 };
#endif
