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
 * @file MazeNode.cpp
 * @brief Contains the methods of the MazeNode class.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */
#ifndef MAZENODE_CPP_
#define MAZENODE_CPP_

#include "constants.h"
#include "MazeNode.h"

/*
 * Simple constructor
 */
MazeNode::MazeNode() {
    // A node has no neighbors at first.
    right = NULL;
    up    = NULL;
    left  = NULL;
    down  = NULL;

    // This is set by various algorithms
    visited = false;
    frontier_visited = false;
    part_of_solution = false;

    // Set by Eller's algorithm
    setNum = -1;

    // This is set by an algorithm
    from = -1;
    to = -1;

    // The node has to be told what row and col it is in; defaults to
    // nonsensical values
    row = -1;
    col = -1;
}

MazeNode::MazeNode(const MazeNode& node) {
	right = node.right;
	up = node.up;
	left = node.left;
	down = node.down;

	visited = node.visited;
	frontier_visited = node.frontier_visited;
	part_of_solution = node.part_of_solution;

	setNum = node.setNum;

	from = node.from;
	to = node.to;

	row = node.row;
	col = node.col;
}

MazeNode::MazeNode(int r, int c) {
    // A node has no neighbors at first.
    right = NULL;
    up    = NULL;
    left  = NULL;
    down  = NULL;

    // This is set by various algorithms
    visited = false;
    frontier_visited = false;
    part_of_solution = false;

    // Set by Eller's algorithm
    setNum = -1;

    // This is set by an algorithm
    from = -1;
    to = -1;

    row = r;
    col = c;
}

MazeNode::~MazeNode() {
}

void MazeNode::resetFlags() {

    // This is set by an algorithm
    visited = false;
    frontier_visited = false;
    part_of_solution = false;

    // Set by Eller's algorithm
    setNum = -1;

    // This is set by an algorithm
    from = -1;
    to = -1;
}

void MazeNode::draw() {
    if (down) {
        std::cout << " ";
    } else {
        std::cout << "_";
    }

    if (right) {
        std::cout << " ";
    } else {
        std::cout << "|";
    }
}


MazeNode *MazeNode::nodeTo(int dir) {
    switch (dir) {
    case RIGHT:
        return right;
        break;
    case UP:
        return up;
        break;
    case LEFT:
        return left;
        break;
    case DOWN:
        return down;
        break;
    default:
        return NULL;
        break;
    }
}

int MazeNode::pathOptions(int **options_array) {
    int len = 0;

    *options_array = new int[4];

    if (right && !right->visited) {
        (*options_array)[len++] = RIGHT;
    }
    if (up && !up->visited) {
        (*options_array)[len++] = UP;
    }
    if (left && !left->visited) {
        (*options_array)[len++] = LEFT;
    }
    if (down && !down->visited) {
        (*options_array)[len++] = DOWN;
    }

    return len;
}

std::string MazeNode::toString() {
    std::stringstream stream;
    stream << "(" << row << "," << col <<  ")";
    return stream.str();
}

std::ostream & operator <<(std::ostream & out, const MazeNode & maze) {
    out << "(" << maze.row << "," << maze.col <<  ")";
    return out;
}

#endif
