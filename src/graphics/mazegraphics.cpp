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
 * mazegraphics.cpp
 *
 *  Created on: Aug 1, 2010
 *      Author: benjamin
 */

#ifndef MAZEGRAPHICS_CPP_
#define MAZEGRAPHICS_CPP_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "mazegraphics.h"

namespace MazeGraphics {

const int width = 800;
const int height = 800;

int window;
Maze2d *maze;

void DrawMaze() {
    glTranslatef(-1.99f,1.99f,0.0f);
    // DrawMazeNode(false, false, false, false);

    const float node_size = 3.9f / maze->numRows();

    for (int r=0; r < maze->numRows(); r++)  {
        for(int c=0; c < maze->numCols(); c++) {
            bool right = maze->connected(r, c, RIGHT);
            bool up = maze->connected(r, c, UP);
            bool left = maze->connected(r, c, LEFT);
            bool down = maze->connected(r, c, DOWN);

            DrawMazeNode(right, up, left, down, node_size);

            // translate to the right
            glTranslatef(node_size, 0.0f, 0.0f);
        }
        // translate down and back to the beginning of the row
        glTranslatef(-node_size * maze->numRows(), -node_size, 0.0f);
    }
}
void DrawMazeNode(bool right, bool up, bool left, bool down, const float node_size) {
    glLineWidth (1.0);
    glColor3f (0.0f, 0.0f, 0.0f);
    glBegin (GL_LINES);

    if (!right) {
        glVertex2f(node_size, 0.0f);
        glVertex2f(node_size, -node_size);
    }
    if (!up) {
        glVertex2f(0.0f,0.0f);
        glVertex2f(node_size, 0.0f);
    }
    if (!left) {
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, -node_size);
    }if (!down) {
        glVertex2f(0.0f, -node_size);
        glVertex2f(node_size, -node_size);
    }

    glEnd();
}

void InitGL(int width, int height) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);// This Will Clear The Background Color To Black
    glClearDepth(1.0);// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);// Enables Depth Testing
    glShadeModel(GL_SMOOTH);// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}
void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
    glLoadIdentity();// Reset The View

    glTranslatef( 0.0f, 0.0f, -5.0f);

    DrawMaze();

    glutSwapBuffers();
}
void ReSizeGLScene(int width, int height) {

    if (height==0)// Prevent A Divide By Zero If The Window Is Too Small
        height=1;

    glViewport(0, 0, width, height);// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(10);

    /* If escape is pressed, kill everything. */
    switch(key) {
    case ESCAPE:
    case 'q':
        /* shut down our window */
        glutDestroyWindow(window);
        /* exit the program...normal termination. */
        exit(0);
        break;
    case 'd':
        maze->draw();
        break;
    case 's':
        maze->drawBig(true);
        break;
    case 'f':
        glutFullScreen();
        break;
    default:
        std::cout << "regular key: " << key << "\n";
        break;
    }

}
void InitMaze(Maze2d *_maze) {

    maze = _maze;

    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    glutInitWindowSize(width, height);

    glutInitWindowPosition(150, 150);

    window = glutCreateWindow("Maze Viewer");

    // glutFullScreen();

    glutDisplayFunc(&DrawGLScene);

    glutIdleFunc(&DrawGLScene);

    glutReshapeFunc(&ReSizeGLScene);

    glutKeyboardFunc(&keyPressed);

    // glutSpecialFunc(&specialKeyPressed);

    InitGL(width, height);

    glutMainLoop();

}

}

#endif

