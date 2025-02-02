#include "../headers/Global.h"


bool check_colision(float coord_x, float coord_y, const sf::RectangleShape& walls) {
    float pac_height = 16;
    float pac_width  = 16;
    sf::FloatRect wall_bounds = walls.getGlobalBounds();
    sf::FloatRect pac_bounds(coord_x, coord_y, pac_width, pac_height);

    return pac_bounds.intersects(wall_bounds);// pac_bounds.intersects(wall_bounds);


}
