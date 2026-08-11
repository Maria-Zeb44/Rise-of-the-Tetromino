#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include"GameConstants.h"
#include"Shape.h"
using namespace std;
class OShape : public Shape {
public:
    OShape(int starting_X_coordinate, int starting_Y_coordinate) : Shape(starting_X_coordinate, starting_Y_coordinate, sf::Color::Yellow) //constructor
    {
        coordinates = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
    }

    void rotate() override {} // O-Shape doesn't rotate as its all the 4 rottaions will be the same

    vector<sf::Vector2i> getRotatedCoords() const override {
        return coordinates; // Returns same coordinates as its all the 4 rotations will be the same
    }
};