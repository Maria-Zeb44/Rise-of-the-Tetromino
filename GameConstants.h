#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

const int WIDTH_OF_WELL = 10;
const int HEIGHT_OF_WELL = 20;
const int BLOCK_SIZE = 30;//each block will be of 30x30 pixels
const int SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW = 55;
const int UPWARD_DISTANCE_OF_WELL_FROM_WINDOW = 55;
const int SIDE_PANEL_WIDTH = SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE+ 55 ;
const float INITIAL_FALL_SPEED = 1.0f;
