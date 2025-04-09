#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

//sets text to the center
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    int width = 1000;
    int height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "my window");

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
    setText(text, width/2, (height/2)-150);

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
                    nameTypedIn.pop_back();
                    nameTypedIn += "|";
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