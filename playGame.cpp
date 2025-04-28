#include <iostream>
#include <SFML/Graphics.hpp>
#include "playGame.hpp"
#include "leaderboard.hpp"
#include <vector>
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
namespace {
//Buttons
sf::Sprite debugButton;
sf::Sprite happyFaceButton;
sf::Sprite pauseButton;
sf::Sprite leaderBoardButton;
sf::Texture leaderBoardTexture;
sf::Texture pauseButtonTexture;
sf::Texture playButtonTexture;
sf::Texture happyFaceTexture;
sf::Texture lossFaceTexture;
sf::Texture winFaceTexture;

sf::Texture debugButtonTexture;

sf::Clock gameClock;
sf::Texture digitsTexture;
sf::Sprite clockDigit1;
sf::Sprite clockDigit2;
sf::Sprite clockDigit3;
sf::Sprite clockDigit4;
int totalSeconds = 0;
int pausedSeconds = 0;
int tilesRevealed = 0;

//Flag counter
int flagCounter = 0;
sf::Sprite counterDigit1;
sf::Sprite counterDigit2;
sf::Sprite counterDigit3;
sf::Sprite negativeDigit;

bool debug = false;
bool pause = false;
bool gameOver = false; //used to disable the debug method when the game is over
bool gameWon = false;
} // anonymous namespace

//each tile
struct tile {
public:
    bool hidden;
    bool mine;
    bool flag;
    sf::Texture hiddenTexture; sf::Texture mineTexture; sf::Texture flagTexture; sf::Texture revealedTexture; sf::Texture adjacentMineTexture; sf::Texture debugTexture;
    sf::Sprite hiddenSprite; sf::Sprite mineSprite; sf::Sprite flagSprite; sf::Sprite revealedSprite; sf::Sprite adjacentMineSprite; sf::Sprite debugMineSprite; sf::Sprite whilePausedTile;
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

        //while paused all tiles need to be in revealed format
        whilePausedTile.setTexture(revealedTexture);

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
        whilePausedTile.setPosition(sf::Vector2f(xPos * 32, yPos * 32));
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
        tilesRevealed++;

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
            flagCounter--;
        }
        else {
            flag = false;
            flagCounter++;
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

//tileList
std::vector<std::vector<tile>> fullTileList;

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
void assignLoseFaceButton() {
    if (!lossFaceTexture.loadFromFile("files/images/face_lose.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    happyFaceButton.setTexture(lossFaceTexture);
}
void assignWinFaceButton() {
    if (!winFaceTexture.loadFromFile("files/images/face_win.png"))
    {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    happyFaceButton.setTexture(winFaceTexture);
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

//check if debug button is pressed
bool pressedDebugButton(int xClick, int yClick) {
    sf::IntRect rect = debugButton.getTextureRect();
    sf::Vector2f pos = debugButton.getPosition(); // Top-left corner of sprite

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
    if (!gameOver) {
        if (pause == true) {
            pauseButton.setTexture(pauseButtonTexture);
            pause = false;
            beginGame();
            return;
        }
        pause = true;
        pauseButton.setTexture(playButtonTexture);
        pausedSeconds += totalSeconds;
    }
}

//For the play/pause button
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

//For the play/pause button
bool checkIfButtonPressedHappyFace(int xClick, int yClick) {
    sf::IntRect happyFaceRect = happyFaceButton.getTextureRect();
    sf::Vector2f happyFacePos = happyFaceButton.getPosition(); // Top-left corner of sprite

    int width = happyFaceRect.width;
    int height = happyFaceRect.height;

    if ((xClick >= happyFacePos.x) && (xClick <= happyFacePos.x + width)) {
        if ((yClick >= happyFacePos.y) && (yClick <= happyFacePos.y + height)) {
            return true;
        }
    }

    return false;
}

//Check if leaderboard button is pressed
bool checkIfLeaderBoardButtonPressed(int xClick, int yClick) {
    sf::IntRect leaderboardrect = leaderBoardButton.getTextureRect();
    sf::Vector2f leaderboardpos = leaderBoardButton.getPosition();

    int width = leaderboardrect.width;
    int height = leaderboardrect.height;

    if ((xClick >= leaderboardpos.x) && (xClick <= leaderboardpos.x + width)) {
        if ((yClick >= leaderboardpos.y) && (yClick <= leaderboardpos.y + height)) {
            return true;
        }
    }

    return false;
}


//End Of Game
void gameWonMethod() {
    if (gameWon == true) {
        pauseTimer(); //pauses the timer
        assignWinFaceButton(); //change face icon
        gameOver = true; //disables play/pause
        openLeaderboard(gameWon, pausedSeconds);

        return;
        std::cout << "game has been lost";
    }

    pauseTimer(); //pauses the timer
    debug = true; //shows all mines
    assignLoseFaceButton(); //change face icon
    gameOver = true;//disable debug and disable play/pause
    gameWon = false;

    std::cout << "game has been lost!";

    //open leaderboard
}

//Recursive call reveals all that don't have any mine relations
//if a tile has a zero mine count, it calls this method which checks if its fellows have zero mines
//if a fellow has zero it goes in depth into that otherwise continues searching
void jumpClear(tile* currentTile) {

    if (currentTile->hidden == false) {
        return;
    }

    if (currentTile->getIfMine()) {
        gameWonMethod();
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

//count number of flags and print
void countFlags() {
    int firstDigit;
    int secondDigit;
    int thirdDigit;

    std::string randoString1 = std::to_string(flagCounter);
    if (flagCounter < -99) {
        firstDigit = randoString1[1] - '0';
        secondDigit = randoString1[2] - '0';
        thirdDigit = randoString1[3] - '0';
    }
    else if (flagCounter < -9) {
        firstDigit = 0;
        secondDigit = randoString1[1] - '0';
        thirdDigit = randoString1[2] - '0';
    }
    else if (flagCounter <= - 1) {
        firstDigit = 0;
        secondDigit = 0;
        thirdDigit = randoString1[1] - '0';
    }
    else if (flagCounter <= 9) {
        firstDigit = 0;
        secondDigit = 0;
        thirdDigit = randoString1[0] - '0';
    }
    else if (flagCounter <= 99) {
        firstDigit = 0;
        secondDigit = randoString1[0] - '0';
        thirdDigit = randoString1[1] - '0';
    }
    else {
        firstDigit = randoString1[0] - '0';
        secondDigit = randoString1[1] - '0';
        thirdDigit = randoString1[2] - '0';
    }

    //begin applying now:
    sf::IntRect rect(firstDigit * 21, 0, 21, 32);
    sf::IntRect rect1(secondDigit * 21, 0, 21, 32);
    sf::IntRect rect2(thirdDigit * 21, 0, 21, 32);
    sf::IntRect rect3(10 * 21, 0, 21, 32);
    counterDigit1.setTextureRect(rect);
    counterDigit2.setTextureRect(rect1);
    counterDigit3.setTextureRect(rect2);
    negativeDigit.setTextureRect(rect3);
}

//sets everything in the vector
void gameReset(int widthOri, int heightOri, int mines) {
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
}

//restart game
void restartGame(int widthOri, int heightOri, int mines) {

    pause = false;
    gameOver = false;
    debug = false;
    gameWon = false;

    //assignHappyFaceButton(widthOri, heightOri);

    //rest timer
    totalSeconds = 0;
    pausedSeconds = 0;
    beginGame(); //restart clock

    tilesRevealed = 0;
    flagCounter = mines;

    //rebuild the grid
    fullTileList.clear();

    //set the list of the tiles
    for (int y = 0; y < heightOri; y++) {
        std::vector<tile> listOfTiles;
        for (int x = 0; x < widthOri; x++) {
            tile first(x, y);
            listOfTiles.push_back(first);
        }

        fullTileList.push_back(listOfTiles);
    }

    // hide all tiles and clear flags
    for (auto &row : fullTileList) {
        for (auto &t : row) {
            t.hidden = true;
            t.flag   = false;
            t.applyHiddenTexture();
            t.applyFlagTexture();
            t.applyRevealedTexture();
        }
    }

    gameReset(widthOri, heightOri, mines);
}


//open and play game window
void playGame(sf::Font font, int mines, int widthOri, int heightOri){
    //begins timer
    beginGame();

    //inital amounts of flags
    flagCounter = 5;

    //Setup clock code
    if (!digitsTexture.loadFromFile("files/images/digits.png")) {
        std::cout << "Failed to load debug button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }
    //set up playbutton texture
    if (!playButtonTexture.loadFromFile("files/images/play.png")) {
        std::cout << "Failed to load play button texture" << std::endl;
        exit(EXIT_FAILURE); //closes the program
    }

    //set clock + counter positioning
    clockDigit1.setPosition(sf::Vector2f((widthOri * 32) - 97, 32 * (heightOri + 0.5) + 16));
    clockDigit2.setPosition(sf::Vector2f((widthOri * 32) - 97 + 21, 32 * (heightOri + 0.5) + 16));
    clockDigit3.setPosition(sf::Vector2f((widthOri * 32) - 54, 32 * (heightOri + 0.5) + 16));
    clockDigit4.setPosition(sf::Vector2f((widthOri * 32) - 54 + 21, 32 * (heightOri + 0.5) + 16));
    counterDigit1.setPosition(sf::Vector2f(33, 32 * (heightOri + 0.5) + 16));
    counterDigit2.setPosition(sf::Vector2f(54, 32 * (heightOri + 0.5) + 16));
    counterDigit3.setPosition(sf::Vector2f(75, 32 * (heightOri + 0.5) + 16));
    negativeDigit.setPosition(sf::Vector2f(12, 32 * (heightOri + 0.5) + 16));

    //sets textures
    clockDigit1.setTexture(digitsTexture);
    clockDigit2.setTexture(digitsTexture);
    clockDigit3.setTexture(digitsTexture);
    clockDigit4.setTexture(digitsTexture);
    counterDigit1.setTexture(digitsTexture);
    counterDigit2.setTexture(digitsTexture);
    counterDigit3.setTexture(digitsTexture);
    negativeDigit.setTexture(digitsTexture);

    //assign buttons
    assignDebugButton(widthOri, heightOri);
    assignHappyFaceButton(widthOri, heightOri);
    assignPauseButton(widthOri, heightOri);
    assignLeaderBoardButton(widthOri, heightOri);

    sf::RenderWindow window(sf::VideoMode(widthOri * 32, (heightOri * 32) + 100), "Minesweeper");

    //set the list of the tiles
    for (int y = 0; y < heightOri; y++) {

        std::vector<tile> listOfTiles;
        for (int x = 0; x < widthOri; x++) {
            tile first(x, y);
            listOfTiles.push_back(first);
        }

        fullTileList.push_back(listOfTiles);
    }

    //sets all other details for the vector
    gameReset(widthOri, heightOri, mines);

    //Game Loop
    while(window.isOpen()) {

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            if (!gameOver) {
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

                        //when the game is over, debug is turned off
                        if (gameOver && gameWon) {
                            debug = false;
                        }
                        if (gameOver && !gameWon) {
                            debug = true;
                        }
                    }
                    else if (checkIfButtonPressedHappyFace(event.mouseButton.x, event.mouseButton.y)) {
                        restartGame(widthOri, heightOri, mines);
                    }

                    else if (checkIfLeaderBoardButtonPressed(event.mouseButton.x, event.mouseButton.y)) {

                        // remember whether the game was already paused
                        bool wasPausedBefore = pause;

                        if (!pause) {
                            pauseTimer();
                        }

                        // Draw one frame to reveal all tiles
                        window.clear(sf::Color::White);
                        for (int y = 0; y < fullTileList.size(); y++) {
                            for (int x = 0; x < fullTileList[y].size(); x++) {
                                window.draw(fullTileList[y][x].whilePausedTile);
                            }
                        }

                        // draw buttons
                        window.draw(debugButton);
                        window.draw(happyFaceButton);
                        window.draw(pauseButton);
                        window.draw(leaderBoardButton);
                        // draw clock digits
                        window.draw(clockDigit1);
                        window.draw(clockDigit2);
                        window.draw(clockDigit3);
                        window.draw(clockDigit4);
                        // draw flag counter
                        window.draw(counterDigit1);
                        window.draw(counterDigit2);
                        window.draw(counterDigit3);
                        if (flagCounter < 0) {
                            window.draw(negativeDigit);
                        }

                        window.display();

                        openLeaderboard(gameWon, pausedSeconds);

                        // if the game was running before opening leaderboard, resume
                        if (!wasPausedBefore) {
                            pauseTimer();
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

                        if (tilesRevealed == (widthOri * heightOri - mines)) {
                            gameWon = true;
                            gameWonMethod();
                        }
                    }
                }
            }
            }

        }

        //refresh time
        if (!pause) {
            clockSpriteCalculator(timeElapsed());
        }

        //flagCounter
        countFlags();

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

                //draw flag counter
                window.draw(counterDigit1);
                window.draw(counterDigit2);
                window.draw(counterDigit3);
                if (flagCounter < 0) {
                    window.draw(negativeDigit);
                }

                if (debug == true) {

                    if (fullTileList[y][x].mine == true) {
                        fullTileList[y][x].applyDebugMineTexture();
                        window.draw(fullTileList[y][x].debugMineSprite);
                    }
                }

                //if pause button is pressed, make all tiles to revealed.
                if (pause == true) {
                    if (gameOver == false) {
                        window.draw(fullTileList[y][x].whilePausedTile);
                    }
                }
            }
        }

        window.display();
    }

}