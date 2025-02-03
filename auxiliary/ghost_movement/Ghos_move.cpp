#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>

const int WINDOW_WIDTH = 420;
const int WINDOW_HEIGHT = 465;

const int GRID_SIZE = WINDOW_HEIGHT / 21;

const int MAP_WIDTH = 19;
const int MAP_HEIGHT = 21;
std::vector<std::string> MAP = {
    "###################", 
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

sf::Color BLUE(0, 0, 254);
sf::Color YELLOW(255, 255, 0);
sf::Color RED(255, 0, 0);
sf::Color LIGHTBLUE(161, 255, 254);
sf::Color PINK(255, 192, 203);
sf::Color ORANGE(255, 165, 0);
sf::Color BLACK(0, 0, 0);

sf::Vector2i pixelPosToGridPos(int pixel_x, int pixel_y) {
    return sf::Vector2i(pixel_x / GRID_SIZE, pixel_y / GRID_SIZE);
}

sf::Vector2i gridPosToPixelPos(int grid_x, int grid_y) {
    return sf::Vector2i(grid_x * GRID_SIZE, grid_y * GRID_SIZE);
}

sf::Color getMapColour(int x, int y) {
    char symbol = MAP[y][x];
    if (symbol == '#') return BLUE;
    else if (symbol == 'c') return YELLOW;
    else if (symbol == 'b') return RED;
    else if (symbol == 'p') return PINK;
    else if (symbol == 'i') return LIGHTBLUE;
    else if (symbol == 'o') return ORANGE;
    return BLACK;
}

class GhostSprite : public sf::Sprite {
public:
    GhostSprite(int grid_x, int grid_y, sf::Color color) {
        this->setPosition(gridPosToPixelPos(grid_x, grid_y).x, gridPosToPixelPos(grid_x, grid_y).y);
        this->setColor(color);
        this->direction = static_cast<Direction>(rand() % 4);
    }

    void moveToGrid(int grid_x, int grid_y) {
        this->setPosition(gridPosToPixelPos(grid_x, grid_y).x, gridPosToPixelPos(grid_x, grid_y).y);
    }

    std::vector<Direction> availableMoves() {
        std::vector<Direction> exits;
        sf::Vector2i mapPos = pixelPosToGridPos(this->getPosition().x, this->getPosition().y);
        
        // handle wrap-around, where it's possible to go "off grid"
        if (mapPos.x <= 0 || mapPos.x >= MAP_WIDTH - 1) {
            exits = {EAST, WEST};
        } else {
            // otherwise consult the map
            if (MAP[mapPos.y - 1][mapPos.x] != '#') exits.push_back(NORTH);
            if (MAP[mapPos.y][mapPos.x + 1] != '#') exits.push_back(EAST);
            if (MAP[mapPos.y + 1][mapPos.x] != '#') exits.push_back(SOUTH);
            if (MAP[mapPos.y][mapPos.x - 1] != '#') exits.push_back(WEST);
        }
        return exits;
    }

    Direction getOppositeDirection() {
        if (direction == NORTH) return SOUTH;
        if (direction == SOUTH) return NORTH;
        if (direction == EAST) return WEST;
        return EAST;  // WEST
    }

    void moveForward() {
        sf::Vector2i mapPos = pixelPosToGridPos(this->getPosition().x, this->getPosition().y);
        
        if (MAP[mapPos.y][mapPos.x] == '<') {
            direction = WEST;
            this->move(-GRID_SIZE, 0);
        } else if (MAP[mapPos.y][mapPos.x] == '>') {
            direction = EAST;
            this->move(GRID_SIZE, 0);
        } else {
            if (direction == NORTH) this->move(0, -GRID_SIZE);
            if (direction == EAST) this->move(GRID_SIZE, 0);
            if (direction == SOUTH) this->move(0, GRID_SIZE);
            if (direction == WEST) this->move(-GRID_SIZE, 0);
        }
    }

    void update() {
        std::vector<Direction> exits = availableMoves();
        Direction opposite = getOppositeDirection();
        if (std::find(exits.begin(), exits.end(), direction) != exits.end() && (exits.size() == 1 || rand() % 100 < 60)) {
            // Continue in the same direction
        } else if (std::find(exits.begin(), exits.end(), opposite) != exits.end() && exits.size() == 1) {
            direction = exits[0];  // U-turn
        } else if (exits.size() > 1) {
            if (std::find(exits.begin(), exits.end(), opposite) != exits.end()) {
                exits.erase(std::remove(exits.begin(), exits.end(), opposite), exits.end());
            }
            direction = exits[rand() % exits.size()];
        }

        moveForward();
    }

private:
    enum class Direction { NORTH, EAST, SOUTH, WEST };
    Direction direction;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pac Algorithm");

    // Make the background image of the map
    sf::RenderTexture background;
    background.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    background.clear(sf::Color::Black);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            rect.setPosition(x * GRID_SIZE, y * GRID_SIZE);
            rect.setFillColor(getMapColour(x, y));
            background.draw(rect);
        }
    }
    background.display();

    // Make the Ghosts
    GhostSprite blinky(9, 7, RED);
    GhostSprite inky(8, 9, LIGHTBLUE);
    GhostSprite pinky(9, 9, PINK);
    GhostSprite pokey(10, 9, ORANGE);

    std::vector<GhostSprite> ghosts = {blinky, inky, pinky, pokey};

    sf::Clock clock;
    float next_ghost_movement = 1.0f;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        for (auto& event : sf::Event()) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle movement update
        if (next_ghost_movement <= deltaTime.asSeconds()) {
            for (auto& ghost : ghosts) {
                ghost.update();
            }
            next_ghost_movement = 1.0f;
        }

        // Drawing
        window.clear();
        window.draw(background.getTexture());
        for (auto& ghost : ghosts) {
            window.draw(ghost);
        }
        window.display();
    }

    return 0;
}

