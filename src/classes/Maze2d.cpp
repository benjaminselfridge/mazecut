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
 * @file Maze2d.cpp
 * @brief Contains the methods of the Maze class.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

#ifndef MAZE2D_CPP_
#define MAZE2D_CPP_

#include "Maze2d.h"
#include "constants.h"
#include "MazeNode.h"

/*
 * Constructor initializes the number of rows and cols, and also
 * allocates memory for the array of nodes.
 */
Maze2d::Maze2d(int r, int c, std::string algorithm) {
    rows = r < MAXMAZEWIDTH ? r : MAXMAZEWIDTH;
    cols = c < MAXMAZEHEIGHT ? c : MAXMAZEHEIGHT;

    // allocation and initialization
    nodes = new MazeNode *[rows];
    for(int i = 0; i < rows; i++)
        nodes[i] = new MazeNode[cols]; // initializes each node also

    setNodeLocations();

    if (small_debug) {
        std::cout << "rows=" << rows << " cols=" << cols << "\n";
    }

    if (algorithm == "rb" || algorithm == "recursivebacktracking") {
        if (small_debug) std::cout << "Using recursive backtracking\n";
        rb();
    } else if (algorithm == "e" || algorithm == "eller" || algorithm == "ellers") {
        if (small_debug) std::cout << "Using Eller's algorithm\n";
        ellers();
    } else if (algorithm == "ab" || algorithm == "aldousbroder" || algorithm == "aldousbroders") {
        if (small_debug) std::cout << "Using the Aldous-Broder algorithm\n";
        aldousBroder();
    } else if (algorithm == "w" || algorithm == "wilson" || algorithm == "wilsons") {
        if (small_debug) std::cout << "Using Wilson's algorithm\n";
        wilsons();
    } else if (algorithm == "bt" || algorithm == "binarytree") {
        if (small_debug) std::cout << "Using a binary tree algorithm\n";
        binaryTree();
    } else if (algorithm == "p" || algorithm == "prim" || algorithm == "prims") {
        if (small_debug) std::cout << "Using Prim's algorithm\n";
        prims(rand()%rows, rand()%cols);
    } else if (algorithm == "s" || algorithm == "sidewinder") {
    	if (small_debug) std::cout << "Using a sidewinder algorithm\n";
    	sidewinder();
    } else {
        std::cerr << "Algorithm \"" << algorithm << "\" not recognized.\n";
    }
}

/*
 * Delete the array row by row
 */
Maze2d::~Maze2d() {
    for(int i = 0; i < rows; i++) {
        delete[] nodes[i];
    }
    delete[] nodes;
}

MazeNode& Maze2d::getCopyOfNode(int r, int c) {
	MazeNode& copy(nodes[r][c]);
	return copy;
}

void Maze2d::setNodeLocations() {
    int r, c;
    for(r=0; r < rows; r++) {
        for(c=0; c < cols; c++) {
            nodes[r][c].row = r;
            nodes[r][c].col = c;
        }
    }
}

void Maze2d::resetFlags() {
    int r, c;
    for (r=0; r < rows; r++) {
        for (c=0; c < cols; c++) {
            nodes[r][c].resetFlags();
        }
    }
}

void Maze2d::draw() {

    int r, c;

    // Top of the maze
    std::cout << "  "; // leave entrance open
    for (c=1; c < cols; c++) {
        std::cout << " _";
    }
    std::cout << "\n";

    // Cells
    for (r=0; r < rows; r++) {
        // Left side of the maze
        std::cout << "|";
        for (c=0; c < cols; c++) {
            if (r == rows-1 && c == cols-1) {
                std::cout << " |"; // leave exit open
            }
            else {
                nodes[r][c].draw();
            }
        }
        std::cout << "\n";
    }
}

void Maze2d::drawBig(bool solve) {
    int r, c;
    int *solution_path, solution_length;
    if (solve) {
        solution_length = findPath(0,0,rows-1,cols-1,&solution_path);
    }

    // Top of the maze
    std::cout << "    ";
    for (c=1; c < cols; c++) {
        std::cout << " ___";
    }
    std::cout << "\n";

    // Cells
    for (r=0; r < rows; r++) {

        // Top of row
        std::cout << "|";
        for (c=0; c < cols; c++) {
            if (solve && nodes[r][c].part_of_solution) {
                std::cout << " * ";
            } else {
                std::cout << "   ";
            }
            if (nodes[r][c].right) {
                std::cout << " ";
            }
            else {
                std::cout << "|";
            }
        }
        std::cout << "\n";

        // Bottom of row
        std::cout << "|";
        for (c=0; c < cols; c++) {
            for (c=0; c < cols; c++) {
                if (nodes[r][c].down || (r == rows-1 && c == cols-1)) {
                    std::cout << "   ";
                } else {
                    std::cout << "___";
                }

                if (nodes[r][c].right) {
                    std::cout << " ";
                } else {
                    std::cout << "|";
                }

            }
        }
        std::cout << "\n";
    }
}

bool Maze2d::join(int r, int c, int dir) {
    // Fail if we're out of bounds
    if (r < 0 or c < 0) return false;
    if (r >= rows or c >= cols) return false;

    switch (dir) {
    case RIGHT:
        // Fail if we're in the last column
        if (c == cols-1) return false;
        // Otherwise, we're good
        nodes[r][c].right  = &nodes[r][c+1];
        nodes[r][c+1].left = &nodes[r][c];
        break;
    case UP:
        // Fail if we're in the first row
        if (r == 0) return false;
        // Otherwise, we're good
        nodes[r][c].up  = &nodes[r-1][c];
        nodes[r-1][c].down = &nodes[r][c];
        break;
    case LEFT:
        // Fail if we're in the first column
        if (c == 0) return false;
        // Otherwise, we're good
        nodes[r][c].left  = &nodes[r][c-1];
        nodes[r][c-1].right = &nodes[r][c];
        break;
    case DOWN:
        // Fail if we're in the last row
        if (r == rows-1) return false;
        // Otherwise, we're good
        nodes[r][c].down  = &nodes[r+1][c];
        nodes[r+1][c].up = &nodes[r][c];
        break;
    default:
        // Something's wrong!
        return false;
        break;
    }

    return true;
}

bool Maze2d::join(MazeNode *one, MazeNode *two) {
    int one_r = one->row, one_c = one->col;
    int two_r = two->row, two_c = two->col;

    if (one_r == two_r && one_c + 1 == two_c) {
        one->right = two;
        two->left = one;
    }
    else if (one_r - 1 == two_r && one_c == two_c) {
        one->up = two;
        two->down = one;
    }
    else if (one_r == two_r && one_c - 1 == two_c) {
        one->left = two;
        two->right = one;
    }
    else if (one_r + 1 == two_r && one_c == two_c) {
        one->down = two;
        two->up = one;
    }

    return true;
}

int Maze2d::getUnvisitedNeighbors(int r, int c, int **options_array) {
    int len = 0;

    *options_array = new int[4];

    if (c < cols-1 && !nodes[r][c+1].visited) {
        (*options_array)[len++] = RIGHT;
    }
    if (r > 0 && !nodes[r-1][c].visited) {
        (*options_array)[len++] = UP;
    }
    if (c > 0 && !nodes[r][c-1].visited) {
        (*options_array)[len++] = LEFT;
    }
    if (r < rows-1 && !nodes[r+1][c].visited) {
        (*options_array)[len++] = DOWN;
    }

    return len;
}

int Maze2d::reGetUnvisitedNeighbors(int r, int c, int *options_array) {
    int len = 0;

    if (c < cols-1 && !nodes[r][c+1].visited) {
        options_array[len++] = RIGHT;
    }
    if (r > 0 && !nodes[r-1][c].visited) {
        options_array[len++] = UP;
    }
    if (c > 0 && !nodes[r][c-1].visited) {
        options_array[len++] = LEFT;
    }
    if (r < rows-1 && !nodes[r+1][c].visited) {
        options_array[len++] = DOWN;
    }

    return len;
}

int Maze2d::getVisitedNeighbors(int r, int c, int **options_array) {
    int len = 0;

    *options_array = new int[4];

    if (c < cols-1 && nodes[r][c+1].visited) {
        (*options_array)[len++] = RIGHT;
    }
    if (r > 0 && nodes[r-1][c].visited) {
        (*options_array)[len++] = UP;
    }
    if (c > 0 && nodes[r][c-1].visited) {
        (*options_array)[len++] = LEFT;
    }
    if (r < rows-1 && nodes[r+1][c].visited) {
        (*options_array)[len++] = DOWN;
    }

    return len;
}

int Maze2d::reGetVisitedNeighbors(int r, int c, int *options_array) {
    int len = 0;

    if (c < cols-1 && nodes[r][c+1].visited) {
        options_array[len++] = RIGHT;
    }
    if (r > 0 && nodes[r-1][c].visited) {
        options_array[len++] = UP;
    }
    if (c > 0 && nodes[r][c-1].visited) {
        options_array[len++] = LEFT;
    }
    if (r < rows-1 && nodes[r+1][c].visited) {
        options_array[len++] = DOWN;
    }

    return len;
}

int Maze2d::getAllNeighbors(int r, int c, int **options_array) {
    int len = 0;

    *options_array = new int[4];
    if (c < cols-1) {
        (*options_array)[len++] = RIGHT;
    }
    if (r > 0) {
        (*options_array)[len++] = UP;
    }
    if (c > 0) {
        (*options_array)[len++] = LEFT;
    }
    if (r < rows-1) {
        (*options_array)[len++] = DOWN;
    }

    return len;
}

int Maze2d::reGetAllNeighbors(int r, int c, int *options_array) {
    int len = 0;

    if (c < cols-1) {
        options_array[len++] = RIGHT;
    }
    if (r > 0) {
        options_array[len++] = UP;
    }
    if (c > 0) {
        options_array[len++] = LEFT;
    }
    if (r < rows-1) {
        options_array[len++] = DOWN;
    }

    return len;
}

std::vector<MazeNode *> Maze2d::getAllNeighbors(int r, int c) {
    std::vector<MazeNode *> allNeighbors;
    allNeighbors.reserve(4);

    if (c < cols-1) {
        allNeighbors.push_back(nodeTo(r, c, RIGHT));
    }
    if (r > 0) {
        allNeighbors.push_back(nodeTo(r, c, UP));
    }
    if (c > 0) {
        allNeighbors.push_back(nodeTo(r, c, LEFT));
    }
    if (r < rows-1) {
        allNeighbors.push_back(nodeTo(r, c, DOWN));
    }

    return allNeighbors;
}

std::vector<MazeNode *> Maze2d::getUnvisitedNeighbors(int r, int c) {
    std::vector<MazeNode *> unvisitedNeighbors;
    unvisitedNeighbors.reserve(4);

    if (c < cols-1 && !nodeTo(r, c, RIGHT)->visited) {
        unvisitedNeighbors.push_back(nodeTo(r, c, RIGHT));
    }
    if (r > 0 && !nodeTo(r, c, UP)->visited) {
        unvisitedNeighbors.push_back(nodeTo(r, c, UP));
    }
    if (c > 0 && !nodeTo(r, c, LEFT)->visited) {
        unvisitedNeighbors.push_back(nodeTo(r, c, LEFT));
    }
    if (r < rows-1 && !nodeTo(r, c, DOWN)->visited) {
        unvisitedNeighbors.push_back(nodeTo(r, c, DOWN));
    }

    return unvisitedNeighbors;
}

std::vector<MazeNode *> Maze2d::getVisitedNeighbors(int r, int c) {
    std::vector<MazeNode *> visitedNeighbors;
    visitedNeighbors.reserve(4);

    if (c < cols-1 && nodeTo(r, c, RIGHT)->visited) {
        visitedNeighbors.push_back(nodeTo(r, c, RIGHT));
    }
    if (r > 0 && nodeTo(r, c, UP)->visited) {
        visitedNeighbors.push_back(nodeTo(r, c, UP));
    }
    if (c > 0 && nodeTo(r, c, LEFT)->visited) {
        visitedNeighbors.push_back(nodeTo(r, c, LEFT));
    }
    if (r < rows-1 && nodeTo(r, c, DOWN)->visited) {
        visitedNeighbors.push_back(nodeTo(r, c, DOWN));
    }

    return visitedNeighbors;
}

MazeNode *Maze2d::nodeTo(int r, int c, int dir) {
    MazeNode node = nodes[r][c];
    switch (dir) {
    case RIGHT:
        return &nodes[r][c+1];
        break;
    case UP:
        return &nodes[r-1][c];
        break;
    case LEFT:
        return &nodes[r][c-1];
        break;
    case DOWN:
        return &nodes[r+1][c];
        break;
    default:
        return NULL;
        break;
    }

    return NULL;
}

bool Maze2d::connected(int r, int c, int dir) {
    MazeNode *node = &nodes[r][c];
    if (node->nodeTo(dir) == NULL) return false;
    else return true;
}

bool Maze2d::getVisited(std::pair<int, int>& rowAndColumn) {
    return nodes[rowAndColumn.first][rowAndColumn.second].visited;
}

void Maze2d::dumpVisited() {
    for(int r=0; r < rows; r++) {
        for(int c=0; c < cols; c++) {
            std::cout << nodes[r][c].visited << " ";
        }
        std::cout << "\n";
    }
}

void Maze2d::rb(int r, int c) {
    MazeNode *current_node = &nodes[r][c];
    int total_steps = 1;

    while (current_node != NULL) {
        r = current_node->row;
        c = current_node->col;

        if (debug) std::cout << "At node " << r << ", " << c << "\n";

        int *option_array;
        int num_options = getUnvisitedNeighbors(r, c, &option_array);

        current_node->visited = true; // mark this node as visited

        if (num_options == 0) {
            // Backtrack
            if (debug) std::cout << "\tNo options, backtracking...\n";
            int from = current_node->from;
            current_node = current_node->nodeTo(from);
        }
        else {
            // Pick a random place to go, and go!
            int x = rand() % num_options;
            int dir = option_array[x];
            if (debug) {
                std::cout << "\tOptions are " << directionsToString(option_array, num_options) << "\n";
                std::cout << "\tGoing " << directionToString(dir) << "\n";
            }

            // Join the current node to the next node
            join(r, c, dir);

            // Get the next node
            MazeNode *next_node = nodeTo(r, c, dir);

            current_node = next_node;
            current_node->from = flip(dir);
        }
        delete[] option_array;
        ++total_steps;
    }
    if (debug) {
        std::cout << "Reached null pointer; terminating.\n";
    }
    if (small_debug) {
        std::cout << "The algorithm took " << total_steps << " steps to complete.\n";
    }
}

/* Perform recursive backtracking starting in a random place */
void Maze2d::rb() {
    rb(rand() % rows, rand() % cols);
}

void Maze2d::ellers() {
    /*
     * Cheap, array-based implementation of a circular linked list with no
     * data, specific to this algorithm.
     */
    int forwards[cols];
    int backwards[cols];

    // Initially, each element is its own list
    for(int c=0; c < cols; c++) {
        forwards[c] = c;
        backwards[c] = c;
    }

    for(int r=0; r < rows-1; r++) {
        if (debug) std::cout << "At row " << r << "\n";
        if (debug) {
            std::cout << "\tCurrent state of linked list:\n";
            for(int c=0; c < cols; c++) {
                std::cout << "\t\t" << c << "->forwards = " << forwards[c] << "\n";
                std::cout << "\t\t" << c << "->backwards = " << backwards[c] << "\n";
            }
        }
        // Join horizontally
        if (debug) std::cout << "\tJoining horizontally...\n";
        for(int c=0; c < cols-1; c++) {
            // If the nodes are not already in the same set, 50/50 chance of
            // joining them
            if(forwards[c] != c+1 && (rand() % 2) == 1) {
                if (debug) {
                    std::cout << "\t\tJoining node " << nodes[r][c];
                    std::cout << " with node " << nodes[r][c+1] << "\n";
                }
                // Physically join the nodes
                join(r, c, RIGHT);
                // Update the linked list
                forwards[backwards[c+1]] = forwards[c];
                backwards[forwards[c]] = backwards[c+1];
                forwards[c] = c+1;
                backwards[c+1] = c;
            }
        }
        if (debug) {
            std::cout << "\tCurrent state of linked list:\n";
            for(int c=0; c < cols; c++) {
                std::cout << "\t\t" << c << "->forwards = " << forwards[c] << "\n";
                std::cout << "\t\t" << c << "->backwards = " << backwards[c] << "\n";
            }
        }

        // Join vertically
        if (debug) std::cout << "\tJoining vertically...\n";
        for(int c=0; c < cols; c++) {
            // Might be able to get less of a rightward bias by making the
            // random number reflect how many nodes are in the set?

            // If it's a singleton set or 50/50 chance
            if (forwards[c] == c || rand() % 2) {
                if (debug) {
                    std::cout << "\t\tJoining node " << nodes[r][c];
                    std::cout << " with node " << nodes[r+1][c] << "\n";
                }
                // Physically join the nodes
                join(r, c, DOWN);
                // Leave the linked list alone
            } else {
                // Update the linked list to put this node in isolation
                forwards[backwards[c]] = forwards[c];
                backwards[forwards[c]] = backwards[c];
                forwards[c] = c;
                backwards[c] = c;
            }
        }
    }

    // For the last row, just make sure to join all the sets
    for(int c=0; c < cols-1; c++) {
        if (forwards[c] != c+1) {
            // Physically join the nodes
            join(rows-1, c, RIGHT);
            // Update the linked list
            forwards[backwards[c+1]] = forwards[c];
            backwards[forwards[c]] = backwards[c+1];
            forwards[c] = c+1;
            backwards[c+1] = c;
        }
    }
}
/*
void Maze2d::prims(int r, int c) {
    int total_steps = 1;

    std::vector<MazeNode *> in;
    std::vector<MazeNode *> frontier;

    if(debug) {
        std::cout << "At " << nodes[r][c] << "\n";
        std::cout << "\tMarking " << nodes[r][c] << " as visited\n";
    }
    nodes[r][c].visited = true;
    in.push_back(&nodes[r][c]);

    if (debug) {
        std::cout << "\tAdding " << &nodes[r][c] << " to in\n";
    }

    std::vector<MazeNode *> unvisitedNeighbors = getUnvisitedNeighbors(r, c);
    if (debug) {
        std::cout << "\tUnvisited neighbors are: ";
        std::cout << nodesToString(unvisitedNeighbors);
        std::cout << "\n";
    }
    for(unsigned int i=0; i < unvisitedNeighbors.size(); i++) {
        MazeNode *new_frontier = unvisitedNeighbors[i];
        if (debug) {
            std::cout << "\tAdding " << *unvisitedNeighbors[i] << " to frontier\n";
        }
        frontier.push_back(new_frontier);
        new_frontier->frontier_visited = true;
    }

    while(!frontier.empty()) {
        if (debug) {
            std::cout << "\tChoosing from frontier: ";
            std::cout << nodesToString(frontier) << "\n";
        }

        int new_in_index = rand() % frontier.size();
        MazeNode *new_in = frontier[new_in_index];
        int new_r = new_in->row, new_c = new_in->col;

        new_in->visited = true;
        in.push_back(new_in);
        frontier.erase(frontier.begin() + new_in_index);
        if (debug) {
            std::cout << "At " << *new_in << "\n";
            std::cout << "\tMarking " << *new_in << " as visited\n";
            std::cout << "\tAdding " << *new_in << " to in\n";
            std::cout << "\tDeleting " << *new_in << " from frontier\n";
            std::cout << "\tin: " << nodesToString(in) << "\n";
            std::cout << "\tfrontier: " << nodesToString(frontier) << "\n";
        }

        std::vector<MazeNode *> unvisitedNeighbors = getUnvisitedNeighbors(new_r, new_c);
        if (debug) {
            std::cout << "\tUnvisited neighbors are: ";
            std::cout << nodesToString(unvisitedNeighbors);
            std::cout << "\n";
        }

        std::vector<MazeNode *> visitedNeighbors = getVisitedNeighbors(new_r, new_c);
        if (debug) {
            std::cout << "\tVisited neighbors are: ";
            std::cout << nodesToString(visitedNeighbors);
            std::cout << "\n";
        }

        int rand_visited_neighbor_index = rand() % visitedNeighbors.size();
        MazeNode *rand_visited_neighbor = visitedNeighbors[rand_visited_neighbor_index];
        if (debug) {
            std::cout << "\tConnecting " << *new_in << " to ";
            std::cout << *rand_visited_neighbor << "\n";
        }
        // join(new_r, new_c, rand_visited_neighbor_dir);
        join(new_in, rand_visited_neighbor);

        for(unsigned int i=0; i < unvisitedNeighbors.size(); i++) {
            MazeNode *new_frontier = unvisitedNeighbors[i];
            if (!new_frontier->frontier_visited) {
                frontier.push_back(new_frontier);
                if (debug) {
                    std::cout << "\tAdding " << *new_frontier << " to frontier\n";
                }
            }
            else {
                if (debug) {
                    std::cout << "\tSkipping " << *new_frontier << ", already in frontier\n";
                }
            }
            new_frontier->frontier_visited = true;
        }

        ++total_steps;
    }

    if (small_debug) {
        std::cout << "The algorithm took " << total_steps << " steps to complete.\n";
    }
}
*/
/*
void Maze2d::prims(int r, int c) {
    int total_steps = 1;

    std::vector<MazeNode *> in;
    std::vector<MazeNode *> frontier;

    in.reserve(rows * cols);
    frontier.reserve(rows * cols);

    nodes[r][c].visited = true;
    in.push_back(&nodes[r][c]);

    std::vector<MazeNode *> unvisitedNeighbors = getUnvisitedNeighbors(r, c);
    for(unsigned int i=0; i < unvisitedNeighbors.size(); i++) {
        MazeNode *new_frontier = unvisitedNeighbors[i];
        frontier.push_back(new_frontier);
        new_frontier->frontier_visited = true;
    }

    while(!frontier.empty()) {

        int new_in_index = rand() % frontier.size();
        MazeNode *new_in = frontier[new_in_index];
        int new_r = new_in->row, new_c = new_in->col;

        new_in->visited = true;
        in.push_back(new_in);
        frontier.erase(frontier.begin() + new_in_index);

        std::vector<MazeNode *> unvisitedNeighbors = getUnvisitedNeighbors(new_r, new_c);
        std::vector<MazeNode *> visitedNeighbors = getVisitedNeighbors(new_r, new_c);

        int rand_visited_neighbor_index = rand() % visitedNeighbors.size();
        MazeNode *rand_visited_neighbor = visitedNeighbors[rand_visited_neighbor_index];

        join(new_in, rand_visited_neighbor);

        for(unsigned int i=0; i < unvisitedNeighbors.size(); i++) {
            MazeNode *new_frontier = unvisitedNeighbors[i];
            if (!new_frontier->frontier_visited) {
                frontier.push_back(new_frontier);
            }
            new_frontier->frontier_visited = true;
        }

        ++total_steps;
    }
}
*/
void Maze2d::prims(int r, int c) {
    int total_steps = 1;

    std::vector<MazeNode *> in;
    std::vector<MazeNode *> frontier;

    int unvisitedNeighbors[4];
    int visitedNeighbors[4];
    int numUnvisitedNeighbors = 0, numVisitedNeighbors = 0;
    int new_in_index = 0, new_r = 0, new_c = 0;
    int rand_visited_neighbor_index = 0;
    int i=0;
    MazeNode *new_in, *new_frontier, *rand_visited_neighbor;

    in.reserve(rows * cols);
    frontier.reserve(rows * cols);

    if(debug) {
        std::cout << "At " << nodes[r][c] << "\n";
        std::cout << "\tMarking " << nodes[r][c] << " as visited\n";
        std::cout << "\tAdding " << nodes[r][c] << " to in\n";
    }

    nodes[r][c].visited = true;
    in.push_back(&nodes[r][c]);

    numUnvisitedNeighbors = reGetUnvisitedNeighbors(r, c, unvisitedNeighbors);
    if (debug) {
        std::cout << "\tUnvisited neighbors are: ";
        std::cout << directionsToString(unvisitedNeighbors, numUnvisitedNeighbors);
        std::cout << "\n";
    }
    for(int i=0; i < numUnvisitedNeighbors; i++) {
        int dir = unvisitedNeighbors[i];
        MazeNode *new_frontier = nodeTo(r, c, dir);
        frontier.push_back(new_frontier);
        new_frontier->frontier_visited = true;
    }

    while(!frontier.empty()) {

        if (debug) {
            std::cout << "\tChoosing from frontier: ";
            std::cout << nodesToString(frontier) << "\n";
        }

        new_in_index = rand() % frontier.size();
        new_in = frontier[new_in_index];
        new_r = new_in->row;
        new_c = new_in->col;

        new_in->visited = true;
        in.push_back(new_in);
        frontier.erase(frontier.begin() + new_in_index);
        if (debug) {
            std::cout << "At " << *new_in << "\n";
            std::cout << "\tMarking " << *new_in << " as visited\n";
            std::cout << "\tAdding " << *new_in << " to in\n";
            std::cout << "\tDeleting " << *new_in << " from frontier\n";
            std::cout << "\tin: " << nodesToString(in) << "\n";
            std::cout << "\tfrontier: " << nodesToString(frontier) << "\n";
        }

        numUnvisitedNeighbors = reGetUnvisitedNeighbors(new_r, new_c, unvisitedNeighbors);
        numVisitedNeighbors = reGetVisitedNeighbors(new_r, new_c, visitedNeighbors);
        if (debug) {
            std::cout << "\tUnvisited neighbors are: ";
            std::cout << directionsToString(unvisitedNeighbors, numUnvisitedNeighbors);
            std::cout << "\n";
            std::cout << "\tVisited neighbors are: ";
            std::cout << directionsToString(visitedNeighbors, numVisitedNeighbors);
            std::cout << "\n";
        }

        rand_visited_neighbor_index = rand() % numVisitedNeighbors;
        rand_visited_neighbor = nodeTo(new_r, new_c, visitedNeighbors[rand_visited_neighbor_index]);
        if (debug) {
            std::cout << "\tConnecting " << *new_in << " to ";
            std::cout << *rand_visited_neighbor << "\n";
        }

        join(new_in, rand_visited_neighbor);

        for(i=0; i < numUnvisitedNeighbors; i++) {
            new_frontier = nodeTo(new_r, new_c, unvisitedNeighbors[i]);
            if (!new_frontier->frontier_visited) {
                frontier.push_back(new_frontier);
                if (debug) {
                    std::cout << "\tAdding " << *new_frontier << " to frontier\n";
                }
            }
            else {
                if (debug) {
                    std::cout << "\tSkipping " << *new_frontier << ", already in frontier\n";
                }
            }
            new_frontier->frontier_visited = true;
        }
        ++total_steps;
    }
    if (small_debug) {
        std::cout << "The algorithm took " << total_steps << " steps to complete.\n";
    }
}

void Maze2d::prims() {
    prims(rand() % rows, rand() % cols);
}

void Maze2d::binaryTree() {
    for(int r=0; r < rows; r++) {
        for(int c=0; c < cols; c++) {
            if (r == 0 && c == 0) {
            } else if (r == 0) {
                join(r, c, LEFT);
            } else if (c == 0) {
                join(r, c, UP);
            } else {
                int which = rand() % 2;
                if (which == 0) {
                    join(r, c, UP);
                } else {
                    join(r, c, LEFT);
                }
            }
        }
    }
}

void Maze2d::aldousBroder(int r, int c) {
    MazeNode *last_node = NULL, *current_node = &nodes[r][c];
    int total_nodes = rows * cols;
    int nodes_carved = 1;
    int total_steps = 0;

    while (nodes_carved < total_nodes) {
        current_node->visited = true;
        int row = current_node->row;
        int col = current_node->col;
        if (debug) {
            std::cout << "At " << *current_node << "\n";
        }

        int *direction_array;
        int num_directions = getAllNeighbors(row, col, &direction_array);
        int direction = direction_array[rand() % num_directions];

        last_node = current_node;
        current_node = nodeTo(row, col, direction);
        if (debug) {
            std::cout << "\tMoving " << directionToString(direction) << " to " << *current_node << "\n";
        }
        if (!current_node->visited) {
            if (debug) {
                std::cout << "\tNode not visited, joining " << *last_node;
                std::cout << " to " << *current_node << "\n";
            }
            join(row, col, direction);
            nodes_carved++;
        }
        if (debug) {
            std::cout << "\tCarved " << nodes_carved << " nodes of " << total_nodes << "\n";
        }
        delete[] direction_array;
        ++total_steps;
    }

    if (small_debug) {
        std::cout << "The algorithm took " << total_steps << " steps to complete.\n";
    }
}

void Maze2d::aldousBroder() {
    aldousBroder(rand() % rows, rand() % cols);
}

void Maze2d::wilsons() {
    int total_steps = 0;

    // Vector representation of our nodes
    std::vector<std::pair<int, int> > pairs;
    for(int r=0; r < rows; r++) {
        for(int c=0; c < cols; c++) {
            pairs.push_back(std::pair<int,int>(r,c));
        }
    }

    // Put the vector in a random order
    std::random_shuffle(pairs.begin(), pairs.end());
    unsigned int pairs_index = 0; // refers to the place we're at in pairs

    if (debug) {
        std::cout << "Adding " << pairs[pairs_index].first << ", " << pairs[pairs_index].second << "\n";
    }

    // Mark the first element of pairs as "in" using the node's visited attribute
    nodes[pairs[pairs_index].first][pairs[pairs_index].second].visited = true;
    pairs_index++;
    ++total_steps;

    while(pairs_index < pairs.size()) {
        int r = pairs[pairs_index].first, c = pairs[pairs_index].second;

        MazeNode *start_node = &nodes[r][c];
        MazeNode *current_node = start_node;

        if (debug) std::cout << "Starting at " << *start_node;

        while (!current_node->visited) {
            ++total_steps;

            int *all_neighbors;
            int num_all_neighbors;
            num_all_neighbors = getAllNeighbors(current_node->row, current_node->col, &all_neighbors);

            int rand_neighbor_index = rand() % num_all_neighbors;
            int random_direction = all_neighbors[rand_neighbor_index];

            current_node->to = random_direction;
            current_node = nodeTo(current_node->row, current_node->col, random_direction);

            if (debug) std::cout << "\n\tAt " << *current_node;

            delete[] all_neighbors;

        }

        if (debug) std::cout << ", an in node\n";

        // Now we've marked a path, so go along each node in the path and add it to the maze
        current_node = start_node;
        while (!current_node->visited) {
            if (debug) std::cout << "\tAdding " << *current_node << "\n";
            int next_dir = current_node->to;
            join(current_node->row, current_node->col, next_dir);
            current_node->visited = true;
            current_node = nodeTo(current_node->row, current_node->col, next_dir);
        }

        // Advance until we get to a node that hasn't been visited
        while(pairs_index < pairs.size() && getVisited(pairs[pairs_index])) pairs_index++;
    }
    if (small_debug) {
        std::cout << "The algorithm took " << total_steps << " steps to complete.\n";
    }
}

int Maze2d::findPath(int start_r, int start_c, int end_r, int end_c,
        int **path_array) {
    resetFlags();
    // The path could potentially be as long as the maze
    *path_array = new int[MAXMAZEWIDTH*MAXMAZEHEIGHT];

    int len=0;

    MazeNode *current_node = &nodes[start_r][start_c];
    while (current_node != NULL &&
            (current_node->row != end_r || current_node->col != end_c)) {

        int *option_array;
        int num_options = current_node->pathOptions(&option_array);

        current_node->visited = true;
        if (num_options == 0) {
            // Set the part_of_solution variable to false
            current_node->part_of_solution = false;

            int from = (*path_array)[--len];

            // std::cout << "backtracking " << from << "\n";
            current_node = current_node->nodeTo(flip(from));
        }
        else {
            // Set the part_of_solution variable to true
            current_node->part_of_solution = true;

            // go in the first direction
            int dir = option_array[0];
            (*path_array)[len++] = dir;
            // std::cout << "moving " << dir << "\n";

            // Get the next node
            MazeNode *next_node = current_node->nodeTo(dir);

            current_node = next_node;
            current_node->from = flip(dir);
        }
        delete[] option_array;
    }
    if (len > 0) {
        nodes[end_r][end_c].part_of_solution = true;
    }

    return len;
}

void Maze2d::huntAndKill() {
}

void Maze2d::kruskals() {
}

void Maze2d::sidewinder() {
	// First, join all the nodes in the top row
	for (int c=0; c < cols-1; c++) {
		join(0, c, RIGHT);
	}

	for (int r=1; r < rows; r++) {
		std::vector<MazeNode *> bunch;
		for (int c=0; c < cols; c++) {
			bool keepGoing = rand() % 2;
			bunch.push_back(&nodes[r][c]);
			if (keepGoing && c < cols-1) {
				join(r, c, RIGHT);
			}
			else {
				MazeNode *randomFromBunch = bunch[rand() % bunch.size()];
				int rand_r = randomFromBunch->row;
				int rand_c = randomFromBunch->col;
				join(rand_r, rand_c, UP);
				bunch.clear();
			}
		}
	}
}

#endif /* MAZE_CPP_ */
