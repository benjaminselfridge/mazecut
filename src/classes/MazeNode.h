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
 * @file MazeNode.h
 * @brief Contains the declaration of the MazeNode class.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

#ifndef MAZENODE_H_
#define MAZENODE_H_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "constants.h"

/**
 * @brief A class encapsulating a single spot, or node, in the maze.
 *
 * Nodes know "where" they are in the maze, but they do not necessarily know
 * who they are next to unless the two nodes are joined in the maze
 */
class MazeNode {

private:

public:

    /**
     * @brief The node, if any, that this node is connected to on the right.
     */
    MazeNode *right;
    /**
     * @brief The node, if any, that this node is connected to upwards.
     */
    MazeNode *up;
    /**
     * @brief The node, if any, that this node is connected to on the left.
     */
    MazeNode *left;
    /**
     * @brief The node, if any, that this node is connected to downwards.
     */
    MazeNode *down;

    /**
     * @brief The row of the node (set manually, not in constructor)
     */
    int row;

    /**
     * @brief The column of the node (set manually, not in constructor)
     */
    int col;

    /**
     * @brief Used for Eller's algorithm
     */
    int setNum;

    /* Flags for algorithms */
    /* These are potentially used differently in each algorithm */

    /**
     * @brief A flag to tell if this node has been visited.
     *
     * Most algorithms use this flag.
     */
    bool visited;

    /**
     * @brief A flag to tell if this node has been put in the frontier.
     *
     * Used in Prim's algorithm for generation.
     */
    bool frontier_visited;

    /**
     * @brief A flag to tell if this node is in the solution
     *
     * Used by all solution-finding algorithms.
     */
    bool part_of_solution;

    /**
     * @brief The direction from which this node was visited.
     *
     * Used for backtracking algorithms (recursive backtracker)
     */
    int from;

    /**
     * @brief The direction to go from this node for a path.
     *
     * Potentially general flag, but only used in Wilson's algorithm.
     */
    int to;

    /**
     * @brief Create a new, blank MazeNode.
     */
    MazeNode();

    /**
     * @brief Create a new MazeNode and initialize its row and column
     *
     * @param r row
     * @param c column
     */
    MazeNode(int r, int c);

    /**
     * @brief Create a new MazeNode from an old one
     *
     * @param node old MazeNode
     */
    MazeNode(const MazeNode& node);
    ~MazeNode();

    /**
     * @brief Reset the node's flags to what they were at construction
     *
     * This method does not touch the row/column variables, nor does it touch
     * the right/left/up/down variables. It resets everything else.
     */
    void resetFlags();

    /**
     * @brief Draw this node.
     *
     * Prints to standard output.
     */
    void draw();

    /**
     * @brief Get a (connected) neighboring node
     *
     * If this node is not connected to another node in the direction
     * <code>dir</code>, then <code>NULL</code> is returned.
     *
     * @param dir the direction we are looking for the node
     * @return a pointer to the node (NULL if there isn't one)
     */
    MazeNode *nodeTo(int dir);

    /**
     * @brief Get an array of all the directions that have an attaching node
     *
     * Don't forget to delete[] the pointer.
     *
     * @param options_array The address of an array in which to copy the directions into.
     * @return The length of the array.
     */
    int pathOptions(int **options_array);

    /**
     * @brief Get a string description of this node of the form "row,col".
     *
     * @return A string description of the node
     */
    std::string toString();
};

/**
 * @brief overloading the output stream operator
 *
 * Uses the toString() method
 */
std::ostream& operator << (std::ostream& out, const MazeNode& maze);


#endif /* MAZENODE_H_ */
