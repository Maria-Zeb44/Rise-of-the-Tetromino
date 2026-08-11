#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "GameConstants.h"
#include "Shape.h"

using namespace std;

class ZShape : public Shape {
public:
    ZShape(int starting_X_coordinate, int starting_Y_coordinate) : Shape(starting_X_coordinate, starting_Y_coordinate, sf::Color::Red)  //constructor
    {
        coordinates = { {0, 0}, {1, 0}, {1, 1}, {2, 1} };
    }

    void rotate() override//overrode rotate function(INHERITANCE)(POLYMORPHISM)
    {
        int centerX = 1, centerY = 1;
        for (int i = 0; i < coordinates.size(); ++i) {
            int tempX = coordinates[i].x - centerX;//rotating x coordinate w.r.t center
            int tempY = coordinates[i].y - centerY;//rotating y coordinate w.r.t center
            coordinates[i].x = -tempY + centerX;
            coordinates[i].y = tempX + centerY;
        }
    }

    vector<sf::Vector2i> getRotatedCoords() const override // overrode function to get coordinates after rotation(INHERITANCE)(POLYMORPHISM)
    {
        vector<sf::Vector2i> rotated_coordinates = coordinates;
        int centerX = 1, centerY = 1;
        for (int i = 0; i < rotated_coordinates.size(); ++i) {
            int relX = rotated_coordinates[i].x - centerX;
            int relY = rotated_coordinates[i].y - centerY;
            rotated_coordinates[i].x = -relY + centerX;
            rotated_coordinates[i].y = relX + centerY;
        }
        return rotated_coordinates;
    }
};
