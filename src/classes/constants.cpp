/*
 *  Copyright 2010 Ben Selfridge
 *
 *  This file is part of mazecut.
 *
 *  mazecut is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mazecut is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with mazecut.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file constants.cpp
 * @brief Contains the implementations of the functions declared in constants.h
 * in mazecut.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

#ifndef CONSTANTS_CPP_
#define CONSTANTS_CPP_

#include "constants.h"
#include "MazeNode.h"

bool debug = false, small_debug = false;

int flip(int direction) {
    switch (direction) {
    case RIGHT:
        return LEFT;
        break;
    case UP:
        return DOWN;
        break;
    case LEFT:
        return RIGHT;
        break;
    case DOWN:
        return UP;
        break;
    default:
        return -1;
        break;
    }
}

std::string directionToString(int direction) {
    switch(direction) {
    case RIGHT:
        return "right";
        break;
    case UP:
        return "up";
        break;
    case LEFT:
        return "left";
        break;
    case DOWN:
        return "down";
        break;
    default:
        return "nodir";
        break;
    }
}

std::string directionsToString(int *directions, int len) {
    std::string theString = "<";
    int i;

    for(i=0; i < len-1; i++) theString += directionToString(directions[i]) + ", ";
    theString += directionToString(directions[len-1]) + ">";

    return theString;
}

std::string nodesToString(std::vector<MazeNode *> nodes) {
    std::stringstream theString;
    theString << "<";
    unsigned int i;

    for(i=0; i < nodes.size(); i++) {
        theString << *(nodes.at(i));
    }

    theString << ">";
    return theString.str();
}

#endif
