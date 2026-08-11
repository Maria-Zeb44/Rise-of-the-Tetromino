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

class IShape : public Shape {
public:
    IShape(int starting_X_coordinate, int starting_Y_coordinate) : Shape(starting_X_coordinate, starting_Y_coordinate, sf::Color::Cyan) //constructor
    {
        coordinates = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
    }

    void rotate() override //overrode rotate function(INHERITANCE)(POLYMORPHISM)
    {
        vector<sf::Vector2i> rotated_coordinates = getRotatedCoords();
        coordinates = rotated_coordinates;
    }

    vector<sf::Vector2i> getRotatedCoords() const override// overrode function to get coordinates after rotation(INHERITANCE)(POLYMORPHISM)
    {
        vector<sf::Vector2i> rotated_coordinates;
        sf::Vector2i center = coordinates[1]; // Use 2nd block as center

        for (int i = 0; i < coordinates.size(); ++i) {
            int tempX = coordinates[i].x - center.x;//rotating x coordinate w.r.t center
            int tempY = coordinates[i].y - center.y;//rotating y coordinate w.r.t center
            sf::Vector2i newCoord(center.x - tempY, center.y + tempX);//storing the rotated coordinates
            rotated_coordinates.push_back(newCoord);
        }

        return rotated_coordinates;
    }
};
