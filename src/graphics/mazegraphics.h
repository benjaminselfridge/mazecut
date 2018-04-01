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

/*
 * mazegraphics.h
 *
 *  Created on: Aug 1, 2010
 *      Author: benjamin
 */

#ifndef MAZEGRAPHICS_H_
#define MAZEGRAPHICS_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Maze2d.h"

#define ESCAPE 27
#define UP_ARROW 101
#define DOWN_ARROW 103
#define LEFT_ARROW 100
#define RIGHT_ARROW 102

namespace MazeGraphics {

void DrawMaze();
void DrawMazeNode(bool right, bool up, bool left, bool down, float node_size);

void InitGL(int width, int height);
void DrawGLScene();
void ReSizeGLScene(int width, int height);
void InitMaze(Maze2d *maze);

}

#endif /* MAZEGRAPHICS_H_ */
