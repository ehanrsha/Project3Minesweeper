#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "playGame.hpp"
#include "leaderboard.hpp"
#include <fstream>


int main() {
    //read config file to gain information for width and height
    std::string width1;
    std::string height1;
    std::string mines1;
    std::ifstream readFile("files/config.cfg");
    std::getline(readFile, width1);
    std::getline(readFile, height1);
    std::getline(readFile, mines1);
    int width = std::stoi(width1) * 32;
    int widthOri = std::stoi(width1);
    int height = (std::stoi(height1)*32) + 100;
    int heightOri = (std::stoi(height1));
    int mines = std::stoi(mines1);

    std::cout<<heightOri<<std::endl;


    sf::RenderWindow window(sf::VideoMode(widthOri * 32, (heightOri * 32) + 100), "Welcome Window");

    //load a font
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        return 10101;
    }

    sf::Text text;
    text.setFont(font);
    text.setString("WELCOME TO MINESWEEPER");
    text.setColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setCharacterSize(24);
    setText(text, (width/2), ((height/2)-150));

    sf::Text text2;
    text2.setFont(font);
    text2.setString("Enter your name:");
    text2.setColor(sf::Color::White);
    text2.setStyle(sf::Text::Bold);
    text2.setCharacterSize(20);
    setText(text2, width/2, (height/2)-75);

    sf::Text text3;
    text3.setFont(font);
    text3.setString("user typed name here");
    text3.setColor(sf::Color::Yellow);
    text3.setStyle(sf::Text::Bold);
    text3.setCharacterSize(18);
    setText(text3, width/2, (height/2)-45);


    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Red);
    std::string nameTypedIn = "";
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (nameTypedIn.empty()) {
                    nameTypedIn += "|";
                }
            }

            //text commands
            if (event.type == sf::Event::TextEntered) {
                sf::Uint32 unicode = event.text.unicode;
                nameTypedIn.pop_back();

                //don't allow more than 10 characters
                if ((nameTypedIn.length() == 10) && (unicode != 8 )) {
                    break;
                }

                //backspace command
                if (unicode == 8) {
                    if (nameTypedIn.length() == 0) {
                        nameTypedIn += "|";
                    }
                    else {
                        nameTypedIn.pop_back();
                        nameTypedIn += "|";
                    }
                    break;
                }

                //Only read letters
                if ((unicode >= 65 && unicode <= 90) || (unicode >= 97 && unicode <= 122)) {
                    //First letter
                    if (nameTypedIn.empty()) {
                        if (unicode <= 90) {
                            char c = static_cast<char>(unicode);
                            nameTypedIn += c;
                        }
                        else {
                            unicode -= 32;
                            char c = static_cast<char>(unicode);
                            nameTypedIn += c;
                        }
                    }
                    else {
                        if (unicode >= 97) {
                            char c = static_cast<char>(unicode);
                            nameTypedIn += c;
                        }
                        else {
                            unicode += 32;
                            char c = static_cast<char>(unicode);
                            nameTypedIn += c;
                        }

                    }
                }

                nameTypedIn += "|";

            }

            //enter key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (nameTypedIn.length() == 1) {
                    break;
                }
                if (nameTypedIn.empty()){
                    break;
                }
                    window.close();
                    //sets width and height for leaderboard
                    nameTypedIn.pop_back();
                    setNumbers(widthOri, heightOri, nameTypedIn);
                    playGame(font, mines, widthOri, heightOri);
            }
        }

        text3.setString(nameTypedIn);

        window.clear(sf::Color::Blue);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.display();
    }

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.