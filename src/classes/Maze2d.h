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
 * @brief Contains the declaration of the Maze class.
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

#ifndef MAZE2D_H_
#define MAZE2D_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "constants.h"
#include "MazeNode.h"

/**
 * @brief Maze data structure.
 *
 * Contains a (dynamically allocated) multidimensional array of MazeNodes, as
 * well as a number of methods to generate and solve itself.
 */
class Maze2d {

public:
    /**
     * @brief Initialize the node with r rows and c columns. Solve it using
     * a given algorithm.
     *
     * A current list of the implemented algorithms can be viewed with
     * ./mazecut -h.
     *
     * @param r number of rows
     * @param c number of columns
     * @param algorithm the algorithm to be used to generate the maze
     */
    Maze2d(int r, int c, std::string algorithm);
    ~Maze2d();

    /**
     * @brief Draw this maze
     *
     * Prints the maze to standard out.
     */
    void draw();

    /**
     * @brief Draw this maze bigger
     *
     * Prints the maze to standard out (twice as large as <code>draw()</code>)
     *
     * @param solve show the maze's solution if true
     */
    void drawBig(bool solve);

    /**
     * @brief Inquire whether the r, c node is connected to a neighboring node.
     *
     * @param r
     * @param c
     * @param dir
     * @return true if connected
     */
    bool connected(int r, int c, int dir);

    /**
     * @brief get the number of rows in the maze
     *
     * @return number of rows
     */
    int numRows() { return rows; }

    /**
     * @brief get the number of columns in the maze
     *
     * @return number of columns
     */
    int numCols() { return cols; }

    MazeNode& getCopyOfNode(int r, int c);

private:

    /**
     * @brief the array of MazeNodes (dynamically allocated in constructor)
     */
    MazeNode **nodes;

    /**
     * @brief Number of rows
     */
    int rows;

    /**
     * @brief Number of columns
     */
    int cols;

    /**
     * @brief Sets the locations of every node in the maze.
     *
     * Should only be called in the constructor.
     */
    void setNodeLocations();

    /**
     * @brief Reset the flags for each node
     */
    void resetFlags();

    /**
     * @brief Join a maze node to an adjacent node, return true if success
     *
     * @param r row of the node we are joining
     * @param c column of the node we are joining
     * @param dir direction in which we are joining (RIGHT, UP, LEFT, DOWN)
     * @return true if success, false if failure
     */
    bool join(int r, int c, int dir);

    /**
     * @brief Get an array of every neighbor of a node
     *
     * Don't forget to delete[] the pointer
     *
     * @param r row of the node
     * @param c column of the node
     * @param ptr_to_array the address of an array to copy into
     * @return the size of the resulting array
     */
    int getAllNeighbors(int r, int c, int **ptr_to_array);

    /**
     * @brief Get an array of every neighbor of a node
     *
     * Assumes the given array is already allocated
     *
     * @param r row of the node
     * @param c column of the node
     * @param array size 4 pre-allocated array of directions (ints)
     * @return the size of the resulting array
     */
    int reGetAllNeighbors(int r, int c, int *array);

    /**
     * @brief Get an array of the unvisited neighbors of a node
     *
     * Don't forget to delete[] the pointer.
     *
     * @param r row of the node
     * @param c column of the node
     * @param ptr_to_array the address of an array to copy into
     * @return the size of the resulting array
     */
    int getUnvisitedNeighbors(int r, int c, int **ptr_to_array);

    /**
     * @brief Get an array of every unvisited neighbor of a node
     *
     * Assumes the given array is already allocated
     *
     * @param r row of the node
     * @param c column of the node
     * @param array size 4 pre-allocated array of directions (ints)
     * @return the size of the resulting array
     */
    int reGetUnvisitedNeighbors(int r, int c, int *array);

    /**
     * @brief Get an array of the visited neighbors of a node
     *
     * Don't forget to delete[] the pointer.
     *
     * @param r row of the node
     * @param c column of the node
     * @param ptr_to_array the addres of an array to copy into
     * @return the size of the resulting array
     */
    int getVisitedNeighbors(int r, int c, int **ptr_to_array);

    /**
     * @brief Get an array of every visited neighbor of a node
     *
     * Assumes the given array is already allocated
     *
     * @param r row of the node
     * @param c column of the node
     * @param array size 4 pre-allocated array of directions (ints)
     * @return the size of the resulting array
     */
    int reGetVisitedNeighbors(int r, int c, int *array);

    /**
     * @brief Get a vector of every neighbor of a node
     *
     * @param r row of the node
     * @param c column of the node
     * @return the vector
     */
    std::vector<MazeNode *> getAllNeighbors(int r, int c);

    /**
     * @brief Get a vector of every unvisited neighbor of a node
     *
     * @param r row of the node
     * @param c column of the node
     * @return the vector
     */
    std::vector<MazeNode *> getUnvisitedNeighbors(int r, int c);

    /**
     * @brief Get a vector of every unvisited neighbor of a node
     *
     * @param r row of the node
     * @param c column of the node
     * @return the vector
     */
    std::vector<MazeNode *> getVisitedNeighbors(int r, int c);

    /**
     * @brief Join two MazeNodes, assuming they are adjacent
     *
     * returns false if they are unjoinable
     * @param one the first node
     * @param two the second node
     * @return true if success
     */
    bool join(MazeNode *one, MazeNode *two);

    /**
     * @brief Get a pointer to the node in a particular direction from a given
     * starting node.
     *
     * Disregards whether the nodes are actually connected.
     *
     * @param r row of starting node
     * @param c column of starting node
     * @param dir direction in which to look for the node
     * @return the node to direction <code>dir</code> of node (r,c)
     */
    MazeNode *nodeTo(int r, int c, int dir);

    /**
     * @brief dumps the node's "visited" boolean to stdout
     */
    void dumpVisited();

    /**
     * @brief tell whether a node has been visited
     *
     * @param rowAndColumn a row, column pair
     * @return whether that node has been visited
     */
    bool getVisited(std::pair<int, int>& rowAndColumn);

    /**
     * @brief Find a path from a starting to ending position, and get the
     * length of the solution
     *
     * @param start_r the row of the starting node
     * @param start_c the column of the starting node
     * @param end_r the row of the ending node
     * @param end_c the column of the ending node
     * @param path_array the address of an array to which we copy the path
     * @return length of the path
     */
    int findPath(int start_r, int start_c, int end_r, int end_c,
            int **path_array);

    /**
     * @brief Generate the maze using recursive backtracking
     *
     * Takes the row and column of the node from which to start.
     *
     * @param r row to start
     * @param c column to start
     */
    void rb(int r, int c);

    /**
     * @brief Generate the maze using recursive backtracking
     *
     * Starts in a random point in the maze.
     */
    void rb();

    /**
     * @brief Generate the maze using Eller's algorithm
     */
    void ellers();

    /**
     * @brief Generate the maze using Prim's algorithm
     *
     * Takes the row and column of the node from which to start.
     *
     * @param r row to start
     * @param c column to start
     */
    void prims(int r, int c);

    /**
     * @brief Generate the maze using Prim's algorithm
     *
     * Starts in a random point in the maze.
     */
    void prims();

    /**
     * @brief Generate the maze using a simple binary tree algorithm
     *
     * Produces a maze that is a literal binary tree with node 0,0 at the root
     * of the tree.
     */
    void binaryTree();

    /**
     * @brief Generate the maze with the Aldous-Broder algorithm
     *
     * @param r row of starting node
     * @param c column of starting node
     */
    void aldousBroder(int r, int c);

    /**
     * @brief Generate the maze with the Aldous-Broder algorithm
     *
     * Starts in a random point in the maze.
     */
    void aldousBroder();

    /**
     * @brief Generate the maze with Wilson's algorithm
     */
    void wilsons();

    /**
     * @brief Generate the maze with Kruskal's algorithm
     *
     * TODO: Implement Kruskal's
     */
    void kruskals();

    /**
     * @brief Generate the maze with the sidewinder algorithm
     *
     * TODO: Implement sidewinder
     */
    void sidewinder();

    /**
     * @brief Generate the maze with the hunt-and-kill algorithm
     *
     * TODO Implement hunt-and-kill
     */
    void huntAndKill();
};

#endif /* MAZE_H_ */
