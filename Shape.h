#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "GameConstants.h"

using namespace std;

class Shape {
protected:
    vector<sf::Vector2i> coordinates;
    int x, y;
    sf::Color color;

public:
    Shape(int startX, int startY, const sf::Color& col) : x(startX), y(startY), color(col) {}//constructor
    virtual ~Shape() {}//destructor

    virtual void rotate() = 0;//pure virtaul rotate function(POLYMORPHISM)

    //GETTERS
    virtual vector<sf::Vector2i> getRotatedCoords() const = 0;//pure virtaul rotate function(POLYMORPHISM)
    vector<sf::Vector2i> getCoordinates() const { return coordinates; }//return coordinates
    int getX() const { return x; }// return x-coordinate
    int getY() const { return y; }// return y-coordinate
    sf::Color getColor() const { return color; }// return color

    //FUNCTIONS FOR MOVEMENT
    void moveLeft() { x--; }
    void moveRight() { x++; }
    void moveDown() { y++; }
    void moveUp() { y--; }

    void draw(sf::RenderWindow& window)//function to display shaped on the screen
    {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
        block.setFillColor(color);
        block.setOutlineThickness(1.5);
        block.setOutlineColor(sf::Color(200, 200, 200));

        for (int i = 0; i < coordinates.size(); ++i) {
            sf::Vector2i coord = coordinates[i];
            block.setPosition(
                SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + (x + coord.x) * BLOCK_SIZE,
                UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + (y + coord.y) * BLOCK_SIZE
            );
            window.draw(block);
        }
    }
};
