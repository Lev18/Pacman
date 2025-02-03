#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
//#include "../../headers/Global.h"

const int WINDOW_HEIGHT = 465;
const int WINDOW_WIDTH  = 420;
const int GRID_SIZE = WINDOW_HEIGHT / 21;


const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 19;
const sf::Color BLACK = {0,   0,   0};
const sf::Color YELLOW   = {255, 255,   0};
const sf::Color BLUE     = {  0,   0, 254};
const sf::Color RED      = {255,   0,   0};
const sf::Color LIGHTBLUE= {161, 255, 254};
const sf::Color PINK     = {255, 192, 203};
const sf::Color ORANGE   ={255, 165,   0};

std::vector<std::string> MAP = 
    {   "###################", 
        "#        #        #",
        "# ## ### # ### ## #",
        "#                 #",
        "# ## # ##### # ## #",
        "#    #   #   #    #",
        "#### ### # ### ####",
        "#### #       # ####",
        "#### # ## ## # ####",
        "<      #   #      >",
        "#### # ##### # ####",
        "#### #   c   # ####",
        "#### # ##### # ####",
        "#        #        #",
        "# ## ### # ### ## #",
        "#  #           #  #",
        "## # # ##### # # ##",
        "#    #   #   #    #",
        "# ###### # ###### #",
        "#                 #",
        "###################" 
    };

const sf::Vector2i pixelPosToGridPos( float pixel_x, float pixel_y ){
    //  Map a window-pixel position to a map-grid position 
    return (sf::Vector2i){(int)(pixel_x / GRID_SIZE), (int)(pixel_y / GRID_SIZE)} ;
}

const sf::Vector2f gridPosToPixelPos( float pixel_x, float pixel_y ){
    //  Map a window-pixel position to a map-grid position 
    return sf::Vector2f(pixel_x * GRID_SIZE, pixel_y * GRID_SIZE);
}   

sf::Color getMapColour(const int x, const int y ){
    // """ Convert map symbols into colours """
    char symbol = MAP[y][x];
    if ( symbol == '#' )
        return BLUE;
    else if (symbol == 'c') 
        return YELLOW;
    else if ( symbol == 'b' )  // # "Shadow" / "Blinky"
        return RED;
    else if ( symbol == 'p' )   // # "Speedy" / "Pinky"
        return PINK;
    else if ( symbol == 'i' )   // # "Bashful" / "Inky"
        return LIGHTBLUE;
    else if ( symbol == 'o' )   // # "Pokey" / "Clyde"
        return ORANGE;
    return BLACK;
}
enum class Direction {NORTH, EAST, SOUTH, WEST};

class GhostSprite : public sf::RectangleShape {
    // A pacman-like ghost sprite 
public:
    GhostSprite (int grid_x, int grid_y, sf::Color color ) {
        this->setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        this->setPosition(gridPosToPixelPos(grid_x, grid_y).x, gridPosToPixelPos(grid_x, grid_y).y);
        this->setFillColor(color);
        this->direction = static_cast<Direction>(rand() % 4);
    }


    void moveToGrid(float grid_x, float grid_y ) {
        //  Allow position to be reset 
        this->setPosition(gridPosToPixelPos( grid_x, grid_y ).x, gridPosToPixelPos(grid_x, grid_y).y);
    }
    std::vector<Direction> availableMoves() {
        // Consult the map to see where is good to go from here.
        //  We only consider walls, not other NPCs """
        std::vector<Direction> exits;

        sf::Vector2i map_pos = pixelPosToGridPos( this->getPosition().x, this->getPosition().y);
        // handle wrap-around, where it's possible to go "off grid"
   
        // checking NORTH
        if (map_pos.y > 0 && MAP[ map_pos.y - 1 ][ map_pos.x ] != '#' ) {
                exits.push_back(Direction::NORTH);
        }

        // checking EAST
        if (map_pos.x < MAP_WIDTH - 1 && MAP[ map_pos.y ][ map_pos.x + 1 ] != '#' ) {
                exits.push_back(Direction::EAST);
        }

        // checking SOUTH
        if (map_pos.y < MAP_HEIGHT - 1 && MAP[ map_pos.y +  1 ][ map_pos.x ] != '#' ) {
                exits.push_back(Direction::SOUTH);
        }

        // checking WEST
        if (map_pos.x > 0 && MAP[ map_pos.y][ map_pos.x - 1 ] != '#' ) {
                exits.push_back(Direction::WEST);
        }
    
        return exits;
    }

    Direction getOppositeDirection() {
        // Return the compass-opposite of our current movement direction 
        if (direction == Direction::NORTH) return Direction::SOUTH;
        if (direction == Direction::SOUTH) return Direction::NORTH;
        if (direction == Direction::EAST) return  Direction::WEST;
        return Direction::EAST; 
    }

    void moveForward() {
        /* Move in the current direction.  Generally we use the map
            to keep us in-bounds, but on the wrap-around we can get
            close to the edge of the map, so use special handling for
            warping 
          handle wrap-around avenue
        */

        sf::Vector2i map_pos = pixelPosToGridPos(this->getPosition().x, this->getPosition().y);

        if (MAP[ map_pos.y ][ map_pos.x ] == '<' ) {
            direction = Direction::WEST;
            // self.rect.x = (MAP_WIDTH-1) * GRID_SIZE
            this->setPosition((MAP_WIDTH - 2) * GRID_SIZE, getPosition().y);
            return;
        }
        else if ( MAP[ map_pos.y ][ map_pos.x ] == '>' ) {
            direction = Direction::EAST;
            // self.rect.x = 0
            this->setPosition( GRID_SIZE, getPosition().y);
            return;
        }  
        // Whichever direction we're moving in, go forward
        if (direction == Direction::NORTH )
                this->move(0, -GRID_SIZE);
        else if (direction == Direction::SOUTH)
                 this->move(0, GRID_SIZE);
        else if (direction == Direction::EAST )
                this->move(GRID_SIZE, 0);
        else if (direction == Direction::WEST)
                this->move(-GRID_SIZE, 0);
    }

    void update() {
        // Move the ghost, mostly forward, never backwards (unless dead-end)
        //  At an intersection, possibly turn """
        //
        sf::Vector2f pos = this->getPosition();
        bool is_alligned = (static_cast<int>(pos.x) % GRID_SIZE == 0) &&
                           (static_cast<int>(pos.y) % GRID_SIZE == 0);
        if (!is_alligned) {
            moveForward();
        }   

        std::vector<Direction> exits = availableMoves();
        // Generally: Keep moving in current direction, never u-turn 
        Direction opposite = getOppositeDirection();

        exits.erase( std::remove(exits.begin(),exits.end(), opposite), exits.end());
                // std::cout << exits.size() << '0';
        if (!exits.size()) {
            exits.push_back(opposite);
        }
                    
        direction = exits[rand() % exits.size()];
    
        //  Move-it- Move-it
        moveForward();
    }
 private: 
        Direction direction;
};

int main() {
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ghost"); 
     sf::RenderTexture back_ground;

    back_ground.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    back_ground.clear(sf::Color::Black);
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH ; ++x) {
            sf::RectangleShape rect (sf::Vector2f(GRID_SIZE, GRID_SIZE));
            rect.setPosition(x *GRID_SIZE, y * GRID_SIZE);
            rect.setFillColor(getMapColour(x, y));
            back_ground.draw(rect);
        }
    }   
    back_ground.display();

    // Make the Ghosts
    GhostSprite blinky(9, 7, RED);
    GhostSprite inky(8, 9, LIGHTBLUE);
    GhostSprite pinky(9, 9, PINK);
    GhostSprite pokey(10, 9, ORANGE);

    std::vector<GhostSprite*> ghosts = {&blinky, &inky, &pinky, &pokey};
    //std::cout << "Ghosts: " << ghosts.size() << '\n';

    sf::Clock clock;
    float movement_interval = 0.1f;
    float time_since_last_move = 0.0f;


    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        time_since_last_move += deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // Handle movement update
        
        if (time_since_last_move > movement_interval) {
            for (auto& ghost : ghosts) {
                ghost->update();
            }
            time_since_last_move = 0.0f;
        }
        
        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(back_ground.getTexture());

        // Drawing
        window.clear();
        window.draw(backgroundSprite);
        for (auto* ghost : ghosts) {
            window.draw(*ghost);
        }
        window.display();
    }

    return 0;
}
