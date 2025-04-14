#include <iostream>
#include <SFML/Graphics.hpp>
#include "playGame.hpp"
#include <vector>
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine

//each tile
struct tile {

public:
    bool hidden;
    bool mine;
    bool flag;
    sf::Texture hiddenTexture; sf::Texture mineTexture; sf::Texture flagTexture; sf::Texture revealedTexture; sf::Texture adjacentMineTexture;
    sf::Sprite hiddenSprite; sf::Sprite mineSprite; sf::Sprite flagSprite; sf::Sprite revealedSprite; sf::Sprite adjacentMineSprite;
    int xPos;
    int yPos;
    std::vector<tile*> adjacentTiles;

    tile(const int x, const int y) {
        hidden = true;
        mine = false;
        flag = false;

        xPos = x;
        yPos = y;

        //sets sprite to hidden texture and sets its position

        setHiddenTexture();
        applyHiddenTexture();

        setFlagTexture();
        applyFlagTexture();

        setRevealedTexture();
        applyRevealedTexture();

        setPosition();
    }

    void setHiddenTexture() {
        if (!hiddenTexture.loadFromFile("files/images/tile_hidden.png"))
        {
            std::cout << "Failed to load tile Hidden texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }
    void setMineTexture() {
        if (!mineTexture.loadFromFile("files/images/mine.png"))
        {
            std::cout << "Failed to load tile Hidden texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }
    void setFlagTexture() {
        if (!flagTexture.loadFromFile("files/images/flag.png"))
        {
            std::cout << "Failed to load tile Hidden texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }
    void setRevealedTexture() {
        if (!revealedTexture.loadFromFile("files/images/tile_revealed.png"))
        {
            std::cout << "Failed to load tile Revealed texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }
    void setAdjacentMineTexture(int x) {

        std::string num = std::to_string(x);
        std::string alpha = "files/images/number_" + num + ".png";
        //to be built
        if (!adjacentMineTexture.loadFromFile(alpha))
        {
            std::cout << "Failed to load adjacent mine texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }

    void applyHiddenTexture() {
        hiddenSprite.setTexture(hiddenTexture);
    }
    void applyMineTexture() {
        mineSprite.setTexture(mineTexture);
    }
    void applyFlagTexture() {
        flagSprite.setTexture(flagTexture);
    }
    void applyRevealedTexture() {
        revealedSprite.setTexture(revealedTexture);
    }
    void applyAdjacentMineTexture() {
        adjacentMineSprite.setTexture(adjacentMineTexture);
    }

    sf::Sprite& returnSprite() {
        return hiddenSprite;
    }

    //sets the sprites position of the tile
    void setPosition() {
        hiddenSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
        flagSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
        mineSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
        revealedSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
        adjacentMineSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
    }

    //Functions
    void reveal() {

        //end game
        if (mine == true) {
            setMineTexture();
            applyMineTexture();
            return;
        }

        hidden = false; //switches to revealed
        int mineCount = 0;

        //count Mines of surroundings
        for (tile* pointer : adjacentTiles) {
            if (pointer->getIfMine() == true) {
                    mineCount++;
            }
        }

        if (mineCount == 0) {
            return;
        }
        setAdjacentMineTexture(mineCount);
        applyAdjacentMineTexture();
    }
    void flagIt() {

        if (flag == false) {
            flag = true;
        }
        else {
            flag = false;
        }

    }

    //set pointers to nearby tiles
    void addPointer(tile* pointer) {
        adjacentTiles.push_back(pointer);
    }

    bool getIfMine() const {
        return mine;
    }

};

//sets text to the center
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

//open and play game window
void playGame(sf::Font font, int mines, int widthOri, int heightOri){

    sf::RenderWindow window(sf::VideoMode(widthOri * 32, (heightOri * 32) + 100), "Minesweeper");

    //set the list of the tiles
    std::vector<std::vector<tile>> fullTileList;
    for (int y = 0; y < heightOri; y++) {

        std::vector<tile> listOfTiles;
        for (int x = 0; x < widthOri; x++) {
            tile first(x, y);
            listOfTiles.push_back(first);
        }

        fullTileList.push_back(listOfTiles);
    }

    //set Pointers
    for (int y = 0; y < fullTileList.size(); y++) {
        for (int x = 0; x < fullTileList[y].size(); x++) {
            bool firstLine = true;
            bool secondLine = false;
            bool thirdLine = false;
            int secondaryCounter = 0;
            int yCoordinate = 0;
            int xCoordinate = 0;
            for (int counter = 0; counter < 9; counter++) {

                tile* pointerToTile;
                if (firstLine) {
                    yCoordinate = y - 1;
                    xCoordinate = x - 1 + counter;
                }
                if (secondLine) {
                    yCoordinate = y;
                    xCoordinate = x - 1 + secondaryCounter;
                    secondaryCounter++;
                }
                if (thirdLine) {
                    yCoordinate = y + 1;
                    xCoordinate = x - 1 + secondaryCounter;
                    secondaryCounter++;
                }

                if (yCoordinate >= 0 && yCoordinate <= heightOri) {
                    if (xCoordinate >= 0 && xCoordinate <= widthOri) {
                        pointerToTile= &fullTileList[yCoordinate][xCoordinate];
                        fullTileList[y][x].addPointer(pointerToTile);
                    }
                }
                if (counter == 2) {
                    firstLine = false;
                    secondLine = true;
                }
                if (counter == 5) {
                    secondLine = false;
                    thirdLine = true;
                    secondaryCounter = 0;
                }

            }

        }
    }

    //random assignment of mines
    std::vector<tile*> compressedVector;
    for (int y = 0; y < fullTileList.size(); y++) {
        for (int x = 0; x < fullTileList[y].size(); x++) {
            compressedVector.push_back(&fullTileList[y][x]);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(compressedVector.begin(), compressedVector.end(), g);
    for (int i = 0; i < mines; i++) {
        compressedVector[i]->mine = true;
    }

    //Game Loop
    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            //mouse button
            if (event.type == sf::Event::MouseButtonReleased)
            {

                int pixelX = event.mouseButton.x / 32;
                int pixelY = event.mouseButton.y / 32;

                //right click to flag
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    tile& element = fullTileList[pixelY][pixelX];

                    element.flagIt();
                }

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    tile& element = fullTileList[pixelY][pixelX];

                    element.hidden = false;
                    element.reveal();
                }

            }

        }


        window.clear(sf::Color::White);

        for (int y = 0; y < fullTileList.size(); y++) {

            for (int x = 0; x < fullTileList[y].size(); x++) {
                if (fullTileList[y][x].hidden == true) {
                    window.draw(fullTileList[y][x].hiddenSprite);
                }
                if (fullTileList[y][x].hidden == false) {
                    window.draw(fullTileList[y][x].revealedSprite);
                }
                if (fullTileList[y][x].flag == true) {
                    window.draw(fullTileList[y][x].flagSprite);
                }

                window.draw(fullTileList[y][x].mineSprite); //will only draw when clicked, and sprite is loaded.
                window.draw(fullTileList[y][x].adjacentMineSprite); //will load only if that item is clicked.
            }
        }

        window.display();
    }

}