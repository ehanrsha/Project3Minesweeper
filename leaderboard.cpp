#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "playGame.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>

int widthOri = 0;
int heightOri = 0;
std::string nameTypedIn = "";

void setNumbers(int widthOri1, int heightOri1, std::string nameTypedIn1){
  widthOri = widthOri1;
  heightOri = heightOri1;
  nameTypedIn = nameTypedIn1;
}

void openLeaderboard(bool gameWon, int totalSeconds){
    std::cout << totalSeconds << std::endl;
    sf::RenderWindow window(sf::VideoMode(widthOri * 16, (heightOri * 16) + 50), "Leaderboard");

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        return;
    }

    //Load data in from file
    std::fstream file("files/leaderboard.txt");
    if(!file.is_open()){
      std::cout << "Failed to open leaderboard file" << std::endl;
      return;
    }

    //read data from file
    std::vector<std::string> pastWinnerList;
    std::string line;
    std::string finalLine;
    int counter = 1;
    while(std::getline(file, line)){
      std::size_t commaPos = line.find(',');
      line.replace(commaPos, 1, "\t");
      std::string nextString = std::to_string(counter) + ".\t" + line + "\n\n";
      pastWinnerList.push_back(nextString);
      counter++;
    }

    int replace = -1;
    //if player won, check data and change vector
    if (gameWon){
      for (int x = 0; x < pastWinnerList.size(); x++ ){
        std::string entry = pastWinnerList[x];
        int tabPos = entry.find("\t");
        std::string substring = entry.substr(tabPos + 1, 5);

        int breakPos = substring.find(":");
        int minutes = std::stoi(substring.substr(0, breakPos));
        int seconds = std::stoi(substring.substr(breakPos + 1));
       	int totalSecondsOfEntry = (minutes * 60) + seconds;

        if (totalSeconds < totalSecondsOfEntry){
          replace = x;
          break;
        }
      }

      if (replace != -1){
        // Build zero-padded time string
        int totalMins = totalSeconds / 60;
        int totalSeconds2 = totalSeconds % 60;
        std::string mins = std::to_string(totalMins);
        std::string seconds = std::to_string(totalSeconds2);
        std::string minsStr = (totalMins < 10 ? "0" + mins : mins);
        std::string secsStr = (totalSeconds2 < 10 ? "0" + seconds : seconds);
        std::string timeStr = minsStr + ":" + secsStr;
        // Create new entry and insert it at the correct position, shifting down the rest
        std::string newEntry = std::to_string(replace + 1) + ".\t" + timeStr + "\t" + nameTypedIn + "*" + "\n\n";
        pastWinnerList.insert(pastWinnerList.begin() + replace, newEntry);
        // If the list grows beyond the top 5, remove the last entry
        if (pastWinnerList.size() > 5) {
            pastWinnerList.pop_back();
        }
      }
    }

  file.close();

    // Renumber entries so prefixes match vector positions
    for (int i = 0; i < pastWinnerList.size(); ++i) {
        size_t tabPos = pastWinnerList[i].find('\t');
        if (tabPos != std::string::npos) {
            std::string rest = pastWinnerList[i].substr(tabPos + 1);
            pastWinnerList[i] = std::to_string(i + 1) + ".\t" + rest;
        }
    }

    //the final string
    for (int x = 0; x < pastWinnerList.size(); x++ ){
      finalLine = finalLine + pastWinnerList[x];
    }
  std::cout << finalLine << std::endl;

    sf::Text leaderboardTitle;
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("LEADERBOARD");
    leaderboardTitle.setColor(sf::Color::White);
    leaderboardTitle.setStyle(sf::Text::Bold);
    leaderboardTitle.setCharacterSize(20);
    leaderboardTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    float windowWidth = widthOri * 16;
    float windowHeight = (heightOri * 16) + 50;
    setText(leaderboardTitle, windowWidth / 2.0f, windowHeight/2.0f - 120);

	sf::Text content;
    content.setFont(font);
    content.setString(finalLine);
    content.setColor(sf::Color::White);
    content.setStyle(sf::Text::Bold);
    content.setCharacterSize(18);
    setText(content, windowWidth / 2.0f, windowHeight/2.0f + 20);


  //write data back to file:
  // Write top 5 back to leaderboard.txt
      std::ofstream outFile("files/leaderboard.txt");
      if (!outFile) {
        std::cout << "Failed to open leaderboard file for writing" << std::endl;
      }
      else {
        int limit = std::min(5, static_cast<int>(pastWinnerList.size()));
        for (int i = 0; i < limit; ++i) {
          std::string entry = pastWinnerList[i];
          // Remove ranking prefix "n.\t"
          size_t firstTab = entry.find('\t');
          std::string rest = entry.substr(firstTab + 1);
          // rest is "mm:ss\t Name*\n\n"
          size_t secondTab = rest.find('\t');
          std::string timeStr = rest.substr(0, secondTab);
          std::string nameStr = rest.substr(secondTab + 1);
          // Trim whitespace/newlines from end
          while (!nameStr.empty() && (nameStr.back() == '\n' || isspace(nameStr.back()))) {
              nameStr.pop_back();
          }
          // Remove trailing asterisk
          if (!nameStr.empty() && nameStr.back() == '*') {
              nameStr.pop_back();
          }
          // Trim again in case asterisk was followed by whitespace
          while (!nameStr.empty() && (nameStr.back() == '\n' || isspace(nameStr.back()))) {
              nameStr.pop_back();
          }
          // Remove leading whitespace
          while (!nameStr.empty() && isspace(nameStr.front())) {
              nameStr.erase(0, 1);
          }
          // Ensure minutes and seconds are zero-padded to two digits
          size_t colonPos = timeStr.find(':');
          std::string minsStr = timeStr.substr(0, colonPos);
          std::string secsStr = timeStr.substr(colonPos + 1);
          if (minsStr.length() < 2) minsStr = "0" + minsStr;
          if (secsStr.length() < 2) secsStr = "0" + secsStr;
          outFile << minsStr << ":" << secsStr << "," << nameStr << std::endl;
        }
      }
      // file is closed when outFile goes out of scope

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }


        }

        window.clear(sf::Color::Blue);
        window.draw(leaderboardTitle);
        window.draw(content);
        window.display();
    }
}

