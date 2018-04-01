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
 * @file mazecut.cpp
 * @brief Implements a basic command-line interface for generating and solving
 * mazes.
 *
 * Invoke with ./mazecut -h to see the list of valid command-line options
 *
 * @author Benjamin Selfridge
 * @date 7/25/10
 */

#ifndef MAZECUT_CPP_
#define MAZECUT_CPP_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "Maze2d.h"
#include "constants.h"

extern bool debug, small_debug;

/**
 * @brief Print a list of all the valid command-line options
 */
void printHelp();

/**
 * @brief Show the warranty information
 */
void showWarrantyInfo();

/**
 * @brief Show the copying information
 */
void showCopyingInfo();

/**
 * @brief Implements a basic CLI for generating and solving
 */
int main (int argc, char * const argv[]) {

    srand(time(0));

    int rows=10, cols=10;

    bool rows_set=false, cols_set=false;
    bool help = false, copyright = false, warranty = false;
    bool big = false;
    bool solve = false;
    bool quiet = false;

    int i;
    unsigned int j;
    char opt;
    std::string algorithm = "rb";

    for (i=1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strlen(argv[i]) == 2 && argv[i][1] == 'a') {
                if (i < argc-1) algorithm = argv[++i];
                else std::cerr << "Warning: -a is not a flag, you need to specify an algorithm\n";
            }
            else {
                for (j=1; j<strlen(argv[i]); j++) {
                    opt = argv[i][j];
                    switch (opt) {
                    case 'h':
                        help = true;
                        break;
					case 'c':
						copyright = true;
						break;
					case 'w':
						warranty = true;
						break;
                    case 'l':
                        big = true;
                        break;
                    case 's':
                        big = true;
                        solve = true;
                        break;
                    case 'D':
                        debug = true;
                        small_debug = true;
                        break;
                    case 'd':
                        small_debug = true;
                        break;
                    case 'q':
                        quiet = true;
                        break;
                    default:
                        std::cout << "Warning: unknown flag '-";
                        std::cout << opt << "'\n";
                        break;
                    }
                }
            }
        }
        else { // it has to be a number
            if (!rows_set) {
                sscanf(argv[i], "%d", &rows);
                if (rows > MAXMAZEHEIGHT) {
                    std::cout << "Warning: max height is ";
                    std::cout << MAXMAZEHEIGHT << ". ";
                    std::cout << "Initializing rows to " << MAXMAZEHEIGHT << ".\n";
                }
                rows_set = true;
            }
            else if (!cols_set) {
                sscanf(argv[i], "%d", &cols);
                if (cols > MAXMAZEWIDTH) {
                    std::cout << "Warning: max width is ";
                    std::cout << MAXMAZEWIDTH << ". ";
                    std::cout << "Initializing cols to " << MAXMAZEWIDTH << ".\n";
                }
                cols_set = true;
            }
        }
    }

    if (help || warranty || copyright) {
		if (help) {
			printHelp();
		}
		if (warranty) {
			showWarrantyInfo();
		}
		if (copyright) {
			showCopyingInfo();
		}
        return 0;
    }

    Maze2d m(rows, cols, algorithm);
    if (!quiet) {
        if (big) m.drawBig(solve);
        else m.draw();
    }
    return 0;
}

void printHelp() {
    std::cout << "---------------------------------------------------------------\n";
    std::cout << "| mazecut -- a c++ program that automatically generates mazes |\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << "\n";
    std::cout << "Usage: mazecut [FLAGS] [-a [algorithm]] <rows> <columns>\n";
    std::cout << "\n";
    std::cout << "Available options:\n\n";

	std::cout << "\tCopyright stuff\n\n";
	std::cout << "\t-c\t\t\tShow the copying information for mazecut\n";
	std::cout << "\t-w\t\t\tShow the warranty information for mazecut\n";
	std::cout << "\n";

    std::cout << "\tFlags\n\n";
    std::cout << "\t-h\t\t\tDisplay help menu\n";
    std::cout << "\t-l\t\t\tDraw larger maze\n";
    std::cout << "\t-s\t\t\tShow solution with maze\n";
    std::cout << "\t-d\t\t\tShow a little debugging information\n";
    std::cout << "\t-D\t\t\tShow all debugging information (there's a lot)\n";
    std::cout << "\t-q\t\t\tQuiet mode - no output\n";
    std::cout << "\t-a [algorithm]\t\tUse a particular algorithm to generate the maze\n";
    std::cout << "\n";

    std::cout << "\tAlgorithm options (use -a [algorithm])\n\n";
    std::cout << "\trb, recursivebacktracking\t\tUse recursive backtracking (default)\n";
    std::cout << "\te, eller, ellers\t\t\tUse Eller's algorithm\n";
    std::cout << "\tp, prim, prims\t\t\t\tUse Prim's algorithm\n";
    std::cout << "\tab, aldousbroder, aldousbroders\t\tUse Aldous-Broder algorithm\n";
    std::cout << "\tw, wilson, wilsons\t\t\tUse Wilson's algorithm\n";
    std::cout << "\tbt, binarytree\t\t\t\tUse binary tree\n";
    std::cout << "\n";
}

void showWarrantyInfo() {

std::cout << "15. Disclaimer of Warranty.\n\n";

std::cout << "  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY \n"
			 "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n"
			 "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n"
			 "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n"
			 "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
			 "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n"
			 "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n"
			 "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n\n";
}

void showCopyingInfo() {
	std::cout << "Copyright 2010 Ben Selfridge\n\n";

	std::cout << "mazecut is free software: you can redistribute it and/or modify\n"
				 "it under the terms of the GNU General Public License as published by\n"
				 "the Free Software Foundation, either version 3 of the License, or\n"
				 "(at your option) any later version.\n\n";

	std::cout << "mazecut is distributed in the hope that it will be useful,\n"
				 "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
				 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
				 "GNU General Public License for more details.\n\n";

	std::cout << "You should have received a copy of the GNU General Public License\n"
				 "along with mazecut.  If not, see <http://www.gnu.org/licenses/>.\n\n";
}

#endif
