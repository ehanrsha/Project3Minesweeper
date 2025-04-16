#include <iostream>
#include <SFML/Graphics.hpp>
#include "playGame.hpp"
#include <vector>
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
//Buttons
sf::Sprite debugButton;
sf::Sprite happyFaceButton;
sf::Sprite pauseButton;
sf::Sprite leaderBoardButton;
sf::Texture leaderBoardTexture;
sf::Texture pauseButtonTexture;
sf::Texture happyFaceTexture;
sf::Texture debugButtonTexture;

sf::Clock gameClock;
sf::Texture digitsTexture;
sf::Sprite clockDigit1;
sf::Sprite clockDigit2;
sf::Sprite clockDigit3;
sf::Sprite clockDigit4;
int totalSeconds = 0;
int pausedSeconds = 0;

bool debug = false;
bool pause = false;

//each tile
struct tile {
public:
    bool hidden;
    bool mine;
    bool flag;
    sf::Texture hiddenTexture; sf::Texture mineTexture; sf::Texture flagTexture; sf::Texture revealedTexture; sf::Texture adjacentMineTexture; sf::Texture debugTexture;
    sf::Sprite hiddenSprite; sf::Sprite mineSprite; sf::Sprite flagSprite; sf::Sprite revealedSprite; sf::Sprite adjacentMineSprite; sf::Sprite debugMineSprite;
    int xPos;
    int yPos;
    int mineCount;
    std::vector<tile*> adjacentTiles;

    //default constructor
    tile(const int x, const int y) {
        hidden = true;
        mine = false;
        flag = false;

        mineCount = 0;

        xPos = x;
        yPos = y;

        //sets sprite to hidden texture and sets its position

        setHiddenTexture();
        applyHiddenTexture();

        setFlagTexture();
        applyFlagTexture();

        setRevealedTexture();
        applyRevealedTexture();

        setPosition(); //applies position for each sprite
        setDebugMineTexture(); //sets debug texture for debug mode
    }

    //set all the textures
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
    void setDebugMineTexture() {
        if (!debugTexture.loadFromFile("files/images/mine.png"))
        {
            std::cout << "Failed to load tile Hidden texture" << std::endl;
            exit(EXIT_FAILURE); //closes the program
        }
    }

    //apply textures to sprites
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
    void applyDebugMineTexture() {
        debugMineSprite.setTexture(debugTexture);
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
        debugMineSprite.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
    }

    //reveals the tile and assigns number
    void reveal() {

        //end game
        if (mine == true) {
            setMineTexture();
            applyMineTexture();
            return;
        }

        hidden = false; //switches to revealed

        if (mineCount == 0) {
            std::cout << "minecount =0 is the issue" << std::endl;
            return;
        }
        setAdjacentMineTexture(mineCount);
        applyAdjacentMineTexture();
    }

    //upon right-clicking makes flag sprite viewable
    void flagIt() {

        if (flag == false) {
            flag = true;
        }
        else {
            flag = false;
        }

    }

    //adds pointers to nearby tiles
    void addPointer(tile* pointer) {
        adjacentTiles.push_back(pointer);
    }

    //returns mine variable
    bool getIfMine() const {
        return mine;
    }

    void mineCounter() {

        //count Mines of surroundings
        for (tile* pointer : adjacentTiles) {
            if (pointer->getIfMine() == true) {
                mineCount++;
            }
        }
    }
};

//assign buttons
void assignDebugButton(int widthOri, int heightOri){
    if (!debugButtonTexture.loadFromFile("files/images/debug.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    debugButton.setTexture(debugButtonTexture);
    debugButton.setPosition(sf::Vector2f((widthOri * 32) - 304, 32 * (heightOri + 0.5)));
}
void assignHappyFaceButton(int widthOri, int heightOri) {
    if (!happyFaceTexture.loadFromFile("files/images/face_happy.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    happyFaceButton.setTexture(happyFaceTexture);
    happyFaceButton.setPosition(sf::Vector2f(((widthOri/2) * 32) - 32, 32 * (heightOri+0.5)));
}
void assignPauseButton(int widthOri, int heightOri) {
    if (!pauseButtonTexture.loadFromFile("files/images/pause.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    pauseButton.setTexture(pauseButtonTexture);
    pauseButton.setPosition(sf::Vector2f((widthOri * 32) - 240, 32 * (heightOri + 0.5)));
}
void assignLeaderBoardButton(int widthOri, int heightOri) {
    if (!leaderBoardTexture.loadFromFile("files/images/leaderboard.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    leaderBoardButton.setTexture(leaderBoardTexture);
    leaderBoardButton.setPosition(sf::Vector2f((widthOri * 32) - 176, 32 * (heightOri + 0.5)));
}

//End Of Game
void gameWon(bool gameWon) {
    //show all mines
    //change face icon
    //disable debug
    //disable play/pause
    //open leaderboard
}

//check if debug button is pressed
bool pressedDebugButton(int xClick, int yClick) {
    sf::IntRect rect = debugButton.getTextureRect();
    sf::Vector2f pos = debugButton.getPosition(); // Top-left corner of sprite

    std::cout << "got to this point" << std::endl;
    int width = rect.width;
    int height = rect.height;

    if ((xClick >= pos.x) && (xClick <= pos.x + width)) {
        if ((yClick >= pos.y) && (yClick <= pos.y + height)) {
            std::cout << "pressedDebug is not at fault" << std::endl;
            return true;
        }
    }

    return false;
}

//sets text to the center
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

//Recursive call reveals all that don't have any mine relations
//if a tile has a zero mine count, it calls this method which checks if its fellows have zero mines
//if a fellow has zero it goes in depth into that otherwise continues searching
void jumpClear(tile* currentTile) {

    if (currentTile->hidden == false) {
        return;
    }

    if (currentTile->getIfMine()) {
        currentTile->reveal();
        return;
    }
    if (currentTile->mineCount != 0) {
        currentTile->reveal();
        return;
    }
    if (currentTile->mineCount == 0) {
        currentTile->reveal();
        for (tile* nextTile : currentTile->adjacentTiles) {
            jumpClear(nextTile);
        }
    }
}

//applying constraints to the texture for the clock
void applyClockDigit(int digit, int DigitNum) {
    sf::IntRect rect(digit * 21, 0, 21, 32);
    if (DigitNum == 1) {
        clockDigit1.setTextureRect(rect);
    }
    else if (DigitNum == 2) {
        clockDigit2.setTextureRect(rect);
    }
    else if (DigitNum == 3) {
        clockDigit3.setTextureRect(rect);
    }
    else {
        clockDigit4.setTextureRect(rect);
    }
}

void beginGame() {
    gameClock.restart(); //restart clock to 0
}

//return time since game has begun
int timeElapsed() {
    sf::Time elapsed = gameClock.getElapsedTime();
    totalSeconds = elapsed.asSeconds();
    return totalSeconds;
}

void clockSpriteCalculator(int secondsBroughtIn) {
    int firstDigit;
    int secondDigit;
    int thirdDigit;
    int fourthDigit;

    int totalSeconds = secondsBroughtIn + pausedSeconds; //includes current time and the paused time
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds - (minutes * 60);

    std::string randoString1 = std::to_string(minutes);
    if (minutes > 9) {
        firstDigit = randoString1[0] - '0';
        secondDigit = randoString1[1] - '0';
    }
    else {
        firstDigit = 0;
        secondDigit = randoString1[0] - '0';
    }

    std::string randoString2 = std::to_string(seconds);
    if (seconds <= 9) {
        thirdDigit = 0;
        fourthDigit = randoString2[0] - '0';
    }
    else {
        thirdDigit = randoString2[0] - '0';
        fourthDigit = randoString2[1] - '0';
    }

    //begin applying now:
    applyClockDigit(firstDigit, 1);
    applyClockDigit(secondDigit, 2);
    applyClockDigit(thirdDigit, 3);
    applyClockDigit(fourthDigit, 4);
}

//pause/play functionality
void pauseTimer() {
    if (pause == true) {
        pause = false;
        beginGame();
        return;
    }
    pause = true;
    pausedSeconds += totalSeconds;
}

bool checkIfButtonPressed(int xClick, int yClick) {
        sf::IntRect pauseRect = pauseButton.getTextureRect();
        sf::Vector2f pausePos = pauseButton.getPosition(); // Top-left corner of sprite

        int width = pauseRect.width;
        int height = pauseRect.height;

        if ((xClick >= pausePos.x) && (xClick <= pausePos.x + width)) {
            if ((yClick >= pausePos.y) && (yClick <= pausePos.y + height)) {
                return true;
            }
        }

        return false;
}

//open and play game window
void playGame(sf::Font font, int mines, int widthOri, int heightOri){
    //begins timer
    beginGame();

    //Setup clock code
    if (!digitsTexture.loadFromFile("files/images/digits.png")) {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }

    //set clock positioning
    clockDigit1.setPosition(sf::Vector2f((widthOri * 32) - 97, 32 * (heightOri + 0.5) + 16));
    clockDigit2.setPosition(sf::Vector2f((widthOri * 32) - 97 + 21, 32 * (heightOri + 0.5) + 16));
    clockDigit3.setPosition(sf::Vector2f((widthOri * 32) - 54, 32 * (heightOri + 0.5) + 16));
    clockDigit4.setPosition(sf::Vector2f((widthOri * 32) - 54 + 21, 32 * (heightOri + 0.5) + 16));

    //sets textures
    clockDigit1.setTexture(digitsTexture);
    clockDigit2.setTexture(digitsTexture);
    clockDigit3.setTexture(digitsTexture);
    clockDigit4.setTexture(digitsTexture);

    //assign buttons
    assignDebugButton(widthOri, heightOri);
    assignHappyFaceButton(widthOri, heightOri);
    assignPauseButton(widthOri, heightOri);
    assignLeaderBoardButton(widthOri, heightOri);

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
            bool skip = false;
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
                    if (secondaryCounter == 1) {
                        skip = true;
                    }
                    secondaryCounter++;
                }
                if (thirdLine) {
                    yCoordinate = y + 1;
                    xCoordinate = x - 1 + secondaryCounter;
                    secondaryCounter++;
                }

                if (yCoordinate >= 0 && yCoordinate < heightOri) {
                    if (xCoordinate >= 0 && xCoordinate < widthOri) {
                        if (skip == true) {
                            skip = false;
                        }
                        else {
                            pointerToTile= &fullTileList[yCoordinate][xCoordinate];
                            fullTileList[y][x].addPointer(pointerToTile);
                        }
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
        compressedVector[i]->applyDebugMineTexture();
    }

    //counts mines and assigns to the tile variable
    for (int y = 0; y < heightOri; y++) {
        for (int x = 0; x < widthOri; x++) {
            fullTileList[y][x].mineCounter();
        }
    }

    //Game Loop
    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            //mouse button (left or right click)
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

                    if (pressedDebugButton(event.mouseButton.x, event.mouseButton.y)) {
                        if (debug == true) {
                            debug = false;
                        }
                        else {
                            debug = true;
                        }
                    }
                    //use to pause the timer
                    else if (checkIfButtonPressed(event.mouseButton.x, event.mouseButton.y)){
                        pauseTimer();
                    }
                    else {
                        if (pixelY <= heightOri) {
                            tile& element = fullTileList[pixelY][pixelX];
                            std::cout << fullTileList[pixelY][pixelX].adjacentTiles.size() << std::endl;
                            jumpClear(&fullTileList[pixelY][pixelX]);
                            element.hidden = false;
                        }
                    }
                }
            }
        }

        //refresh time
        if (!pause) {
            clockSpriteCalculator(timeElapsed());
        }

        window.clear(sf::Color::White);

        //drawing to the screen
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

                //draw buttons
                window.draw(debugButton);
                window.draw(happyFaceButton);
                window.draw(pauseButton);
                window.draw(leaderBoardButton);

                //draw clock
                window.draw(clockDigit1);
                window.draw(clockDigit2);
                window.draw(clockDigit3);
                window.draw(clockDigit4);

                if (debug == true) {

                    if (fullTileList[y][x].mine == true) {
                        fullTileList[y][x].applyDebugMineTexture();
                        window.draw(fullTileList[y][x].debugMineSprite);
                    }
                }
            }
        }

        window.display();
    }

}