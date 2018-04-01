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
 * @file constants.h
 * @brief Contains the declarations of the constants and global functions and
 * variables used in mazecut.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

/*
 * TODO: Replace explicit pointer parameters with reference parameters.
 * TODO: General error handling (exceptions)
 * TODO: Replace all the if(debug)s with a new function debugPrint(string)
 * TODO: Instead of capping the rows and columns, cap the total # of nodes
 * TODO: Algorithms too slow: Prim's
 * TODO: Turn all global variables, defs, and constants into static class members
 * TODO: Make MazeNode an inner class of Maze2d (private)
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "MazeNode.h"

/**
 * @brief Maximum maze height
 */
const int MAXMAZEHEIGHT = 500;

/**
 * @brief Maximum maze width
 */
const int MAXMAZEWIDTH  = 500;

/**
 * @brief Maximum nodes in a node set
 */
const int MAXNODESPERSET = MAXMAZEWIDTH;

/**
 * @brief Enumeration of directions
 */
enum DIRECTIONS {RIGHT, UP, LEFT, DOWN};

/**
 * @brief strong debug mode - prints every step of every algorithm
 */
extern bool debug;

/**
 * @brief weak debug mode - only prints general info about the execution of
 * an algorithm
 */
extern bool small_debug;

/**
 * @brief Flips up/down, left/right. Useful for backtracking.
 *
 * @param direction the direction (RIGHT, UP, LEFT, or DOWN)
 * @return opposite direction
 */
int flip(int direction);

/**
 * @brief converts a direction to a string.
 *
 * @param direction the direction
 * @return "right", "up", "left", or "down"
 */
std::string directionToString(int direction);

/**
 * @brief Given an array of directions, outputs a string with each direction
 * interspersed with commas.
 *
 * @param directions a pointer to the array of directions
 * @param len the length of that array
 * @return string of directions
 */
std::string directionsToString(int *directions, int len);

/**
 * @brief Get a string description of several nodes.
 *
 * @param nodes a vector of nodes to convert to a string
 * @return string of nodes
 */
std::string nodesToString(std::vector<MazeNode *> nodes);

/**
 * @brief Prints
 */

#endif
