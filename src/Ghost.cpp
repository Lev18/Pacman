#include <filesystem>
#include <iostream>
#include <cmath>
#include <queue>

#include "../headers/Global.h"
#include "../headers/Ghost.h"

const sf::Vector2i pixelPosToGridPos( float pixel_x, float pixel_y ){
    //  Map a window-pixel position to a map-grid position
    return (sf::Vector2i){(int)(pixel_x / GRID_SIZE), (int)(pixel_y / GRID_SIZE)} ;
}

const sf::Vector2f gridPosToPixelPos( float pixel_x, float pixel_y ){
    //  Map a window-pixel position to a map-grid position
    return sf::Vector2f(pixel_x * GRID_SIZE, pixel_y * GRID_SIZE);
}

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

            // direction 
            m_direction = static_cast<Move_Direction>(rand() % 4);
}

void Ghost::ghost_animation(int order) {
    m_ghost.setTextureRect(sf::IntRect(order, 0, 16, 16));
    m_ghost_eyes.setTextureRect(sf::IntRect(16,16, 16, 16));
}

void Ghost::draw_ghost(sf::RenderWindow& window) {
    window.draw(m_ghost);
    window.draw(m_ghost_eyes);
}

Move_Direction Ghost::get_opposite_direction() {
    // Return the compass-opposite of our current movement direction 
        if (m_direction == Move_Direction::UP)    return Move_Direction::DOWN;
        if (m_direction == Move_Direction::DOWN) return Move_Direction::UP;
        if (m_direction == Move_Direction::RIGHT)  return Move_Direction::LEFT;
        return Move_Direction::RIGHT;
}


  std::vector<Move_Direction> Ghost::available_moves(const std::array<std::string, MAP_HEIGHT>& map) {
        // Consult the map to see where is good to go from here.
        //  We only consider walls, not other NPCs """
        std::vector<Move_Direction> exits;

        sf::Vector2i map_pos = pixelPosToGridPos(m_ghost.getPosition().x, m_ghost.getPosition().y);
        // handle wrap-around, where it's possible to go "off grid"

        // checking NORTH
        if (map_pos.y > 0 && map[ map_pos.y - 1 ][ map_pos.x ] != '#' ) {
            exits.push_back(Move_Direction::UP);
        }

        // checking EAST
        if (map_pos.x < MAP_WIDTH - 1 && map[ map_pos.y ][ map_pos.x + 1 ] != '#' ) {
            exits.push_back(Move_Direction::RIGHT);
        }

        // checking SOUTH
        if (map_pos.y < MAP_HEIGHT - 1 && map[ map_pos.y +  1 ][ map_pos.x ] != '#' ) {
            exits.push_back(Move_Direction::DOWN);
        }

        // checking WEST
        if (map_pos.x > 0 && map[ map_pos.y][ map_pos.x - 1 ] != '#' ) {
            exits.push_back(Move_Direction::LEFT);
        }

        return exits;
    }

    void Ghost::move_forward(const std::array<std::string, MAP_HEIGHT>& map) {
        /* Move in the current direction.  Generally we use the map
            to keep us in-bounds, but on the wrap-around we can get
            close to the edge of the map, so use special handling for
            warping
          handle wrap-around avenue
        */

        sf::Vector2i map_pos = pixelPosToGridPos(m_ghost.getPosition().x, m_ghost.getPosition().y);

        if (map[ map_pos.y ][ map_pos.x ] == '<' ) {
            m_direction = Move_Direction::LEFT;
            // self.rect.x = (MAP_WIDTH-1) * GRID_SIZE
            m_ghost.setPosition((MAP_WIDTH - 2) * GRID_SIZE, m_ghost.getPosition().y);
            m_ghost_eyes.setPosition((MAP_WIDTH - 2) * GRID_SIZE, m_ghost.getPosition().y);
            return;
        }
        else if ( map[map_pos.y ][ map_pos.x ] == '>' ) {
            m_direction = Move_Direction::RIGHT;
            // self.rect.x = 0
            m_ghost.setPosition( GRID_SIZE, m_ghost.getPosition().y);
            m_ghost_eyes.setPosition( GRID_SIZE, m_ghost.getPosition().y);
            return;
        }
        // Whichever direction we're moving in, go forward
        float dx = 0.0f;
        float dy = 0.0f;

        switch (m_direction) {
            case Move_Direction::UP:
                dy = -GRID_SIZE;
                break;
            case Move_Direction::DOWN:
                dy = GRID_SIZE;
                break;
            case Move_Direction::RIGHT:
                dx = GRID_SIZE;
                break;
            case Move_Direction::LEFT:
                dx = -GRID_SIZE;
                break;
        }
        /*
        if (m_direction == Move_Direction::UP ) {
            m_ghost.move(0, -GRID_SIZE);
            m_ghost_eyes.move(0, -GRID_SIZE);
        }
        else if (m_direction == Move_Direction::DOWN) {
            m_ghost.move(0, GRID_SIZE);
            m_ghost_eyes.move(0, -GRID_SIZE);
        }
        else if (m_direction == Move_Direction::RIGHT ) {
            m_ghost.move(GRID_SIZE, 0);
            m_ghost_eyes.move(0, -GRID_SIZE);
        }
        else if (m_direction == Move_Direction::LEFT) {
            m_ghost.move(-GRID_SIZE, 0);
            m_ghost_eyes.move(-GRID_SIZE, 0);     
        }
        */
        m_ghost.move(dx, dy);
        m_ghost_eyes.move(dx, dy); 
    }


void Ghost::running_to_catch(const std::array<std::string, MAP_HEIGHT>& map) {
    sf::Vector2f pos = m_ghost.getPosition();
    bool is_alligned = (static_cast<int>(pos.x) % GRID_SIZE == 0) &&
                       (static_cast<int>(pos.y) % GRID_SIZE == 0);

     if (!is_alligned) {
         move_forward(map);
     }

    std::vector<Move_Direction> exits = available_moves(map);
    // Generally: Keep moving in current direction, never u-turn
    Move_Direction opposite = get_opposite_direction();

    exits.erase( std::remove(exits.begin(),exits.end(), opposite), exits.end());
            // std::cout << exits.size() << '0';
    if (!exits.size()) {
        exits.push_back(opposite);
    }

    m_direction = exits[rand() % exits.size()];

    //  Move-it- Move-it
    move_forward(map);
}
