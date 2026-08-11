#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>// for smart pointers
#include <random> // for random number generation
#include <iostream>
#include <sstream>//string stream
#include <iomanip>
#include"GameConstants.h"
#include"Shape.h"
#include"IShape.h"
#include"JShape.h"
#include"LShape.h"
#include"OShape.h"
#include"SShape.h"
#include"TShape.h"
#include"ZShape.h"
using namespace std;
class Game {
private:
    int score;
    int level; 
    int linesCleared; // num of lines cleared
    float fallSpeed;
    bool isGameOver;
    bool shapeLocked = false;//prevent redundance
    bool isPaused;
    bool shapeJustDropped = false;//prevent redundance
    unique_ptr<Shape> currentShape; // used smart pointers to avoid any memory losses
    
    //GRAPHICS AND TIME
    sf::RenderWindow window;
    vector<vector<sf::Color>> well;
    sf::Clock gameClock;
    sf::Clock inputClock;
    sf::Clock moveClock;
    sf::Font font;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> dist;//for selecting range to be selected between 0 and 6

public:
    Game();
    void run();
private:
    void spawnNewShape();
    void clearLines();
    void handleInput();
    void update();
    void resetGame();
    void render();
    void mergeShape();
    bool checkCollision();
    bool checkPosition(const vector<sf::Vector2i>&, int, int);
    
    void drawInfoBox(const std::string& title, const std::string& value, float top) {
        const float width_of_blocks = 200;
        const float height_of_block = 50;
        const float boxSpacing = 1;

        sf::RectangleShape box(sf::Vector2f(width_of_blocks, height_of_block));
        box.setPosition(SIDE_PANEL_WIDTH, top);
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Black);
        window.draw(box);

        sf::Text titleText(title, font, 20);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(SIDE_PANEL_WIDTH + 5, top + 5);
        window.draw(titleText);

        sf::Text valueText(value, font, 20);
        valueText.setFillColor(sf::Color::Black);
        valueText.setPosition(SIDE_PANEL_WIDTH + width_of_blocks - valueText.getLocalBounds().width - 5, top + 5);
        window.draw(valueText);
    }

};