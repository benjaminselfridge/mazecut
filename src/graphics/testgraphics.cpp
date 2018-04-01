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
 * test.cpp
 *
 *  Created on: Aug 1, 2010
 *      Author: benjamin
 */

#ifndef TEST_CPP_
#define TEST_CPP_

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <iostream>

#define ESCAPE 27
#define UP_ARROW 101
#define DOWN_ARROW 103
#define LEFT_ARROW 100
#define RIGHT_ARROW 102

int window;


GLfloat translate_x=0.0f;
GLfloat translate_y=0.0f;
GLfloat translate_z=-10.0f;

GLfloat rotate_x=0.0f;
GLfloat rotate_y=0.0f;
GLfloat rotate_z=0.0f;

GLfloat rotate_x_speed=0.0f;
GLfloat rotate_y_speed=0.0f;
GLfloat rotate_z_speed=0.0f;

GLfloat translate_incr = 0.05f;
GLfloat rotate_incr = 1.0f;
GLfloat speed_incr = 0.05f;

bool moving = true;

GLfloat randomFloat(GLfloat lowest, GLfloat highest) {
    // Get a random number between 0 and 1
    GLfloat r = (float)rand()/(float)RAND_MAX;

    // Scale it
    r *= (highest - lowest);

    // Put it in the correct range
    r += lowest;

    return r;
}

struct point {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct cube {
    struct point *ful;
    struct point *fur;
    struct point *fdl;
    struct point *fdr;
    struct point *bul;
    struct point *bur;
    struct point *bdl;
    struct point *bdr;
};

struct point *newPoint(GLfloat x, GLfloat y, GLfloat z) {
    struct point *new_pt = new struct point;

    new_pt->x = x;
    new_pt->y = y;
    new_pt->z = z;

    return new_pt;
}

struct point *randomPoint(GLfloat start_x, GLfloat start_y, GLfloat start_z, float max_err) {
    float x = randomFloat(start_x-max_err, start_x+max_err);
    float y = randomFloat(start_y-max_err, start_y+max_err);
    float z = randomFloat(start_z-max_err, start_z+max_err);

    struct point *rand_pt = newPoint(x, y, z);

    return rand_pt;
}

struct cube *newCube(struct point *ful, struct point *fur, struct point *fdl, struct point *fdr,
        struct point *bul, struct point *bur, struct point *bdl, struct point *bdr) {
    struct cube *new_cube = new struct cube;

    new_cube->ful = ful;
    new_cube->fur = fur;
    new_cube->fdl = fdl;
    new_cube->fdr = fdr;
    new_cube->bul = bul;
    new_cube->bur = bur;
    new_cube->bdl = bdl;
    new_cube->bdr = bdr;

    return new_cube;
}

struct cube *newUniformCube(GLfloat radius) {
    struct point *ful = newPoint(-radius, radius, radius);
    struct point *fur = newPoint( radius, radius, radius);
    struct point *fdl = newPoint(-radius,-radius, radius);
    struct point *fdr = newPoint( radius,-radius, radius);
    struct point *bul = newPoint(-radius, radius,-radius);
    struct point *bur = newPoint( radius, radius,-radius);
    struct point *bdl = newPoint(-radius,-radius,-radius);
    struct point *bdr = newPoint( radius,-radius,-radius);

    struct cube *new_unif_cube = newCube(ful, fur, fdl, fdr, bul, bur, bdl, bdr);

    return new_unif_cube;
}

void deleteCube(struct cube *cube_ptr) {
    delete cube_ptr->ful;
    delete cube_ptr->fur;
    delete cube_ptr->fdl;
    delete cube_ptr->fdr;
    delete cube_ptr->bul;
    delete cube_ptr->bur;
    delete cube_ptr->bdl;
    delete cube_ptr->bdr;

    delete cube_ptr;
}

void drawVertex(struct point *pt) {
    glVertex3f(pt->x, pt->y, pt->z);
}

void drawCube(struct cube *cube) {
    // front of cube
    glColor3f(1.0f,0.0f,0.0f);
    drawVertex(cube->ful);
    drawVertex(cube->fur);
    drawVertex(cube->fdr);
    drawVertex(cube->fdl);

    // back of cube
    glColor3f(0.0f,1.0f,0.0f);
    drawVertex(cube->bul);
    drawVertex(cube->bur);
    drawVertex(cube->bdr);
    drawVertex(cube->bdl);

    // right of cube
    glColor3f(0.0f,0.0f,1.0f);
    drawVertex(cube->fur);
    drawVertex(cube->bur);
    drawVertex(cube->bdr);
    drawVertex(cube->fdr);

    // left of cube
    glColor3f(1.0f,1.0f,0.0f);
    drawVertex(cube->ful);
    drawVertex(cube->bul);
    drawVertex(cube->bdl);
    drawVertex(cube->fdl);

    // top of cube
    glColor3f(1.0f,0.0f,1.0f);
    drawVertex(cube->ful);
    drawVertex(cube->fur);
    drawVertex(cube->bur);
    drawVertex(cube->bul);

    // bottom of cube
    glColor3f(0.0f,1.0f,1.0f);
    drawVertex(cube->fdl);
    drawVertex(cube->fdr);
    drawVertex(cube->bdr);
    drawVertex(cube->bdl);
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)        // We call this right after our OpenGL window is created.
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);// This Will Clear The Background Color To Black
    glClearDepth(1.0);// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);// Enables Depth Testing
    glShadeModel(GL_SMOOTH);// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
    glLoadIdentity();// Reset The View

    glTranslatef( translate_x, translate_y, translate_z);
    glRotatef(rotate_x,1.0f,0.0f,0.0f);
    glRotatef(rotate_y,0.0f,1.0f,0.0f);
    glRotatef(rotate_z,0.0f,0.0f,1.0f);

    glBegin(GL_QUADS);

    struct cube *cube = newUniformCube(1.0f);
    drawCube(cube);
    glEnd();

    if (moving) {
        rotate_x += rotate_x_speed;
        rotate_y += rotate_y_speed;
        rotate_z += rotate_z_speed;
    }

    deleteCube(cube);
    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)// Prevent A Divide By Zero If The Window Is Too Small
        Height=1;

    glViewport(0, 0, Width, Height);// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
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
    case 's':
        if (moving) moving = false;
        else moving = true;
        break;
    case 'r':
        translate_x=0.0f;
        translate_y=0.0f;
        translate_z=-10.0f;

        rotate_x=0.0f;
        rotate_y=0.0f;
        rotate_z=0.0f;

        rotate_x_speed=0.0f;
        rotate_y_speed=0.0f;
        rotate_z_speed=0.0f;
        break;
    default:
        std::cout << "regular key: " << key << "\n";
        break;
    }

}

void specialKeyPressed(int key, int x, int y) {
    usleep(10);

    int mod = glutGetModifiers();

    switch(key) {
    case UP_ARROW:
        if (mod == GLUT_ACTIVE_CTRL) {
            rotate_x += rotate_incr;
        }
        else if (mod == GLUT_ACTIVE_ALT) {
            rotate_x_speed += speed_incr;
        }
        else if (mod == GLUT_ACTIVE_SHIFT) {
            translate_z += translate_incr;
        }
        else {
            translate_y += translate_incr;
        }
        break;
    case DOWN_ARROW:
        if (mod == GLUT_ACTIVE_CTRL) {
            rotate_x -= rotate_incr;
        }
        else if (mod == GLUT_ACTIVE_ALT) {
            rotate_x_speed -= speed_incr;
        }
        else if (mod == GLUT_ACTIVE_SHIFT) {
            translate_z -= translate_incr;
        }
        else {
            translate_y -= translate_incr;
        }
        break;
    case LEFT_ARROW:
        if (mod == GLUT_ACTIVE_CTRL) {
            rotate_y += rotate_incr;
        }
        else if (mod == GLUT_ACTIVE_ALT) {
            rotate_y_speed += speed_incr;
        }
        else {
            translate_x -= translate_incr;
        }
        break;
    case RIGHT_ARROW:
        if (mod == GLUT_ACTIVE_CTRL) {
            rotate_y -= rotate_incr;
        }
        else if (mod == GLUT_ACTIVE_ALT) {
            rotate_y_speed -= speed_incr;
        }
        else {
            translate_x += translate_incr;
        }
        break;
    default:
        std::cout << "special key: " << key << "\n";
        break;
    }
}

int main(int argc, char **argv)
{
    srand(time(0));

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    glutInitWindowSize(600, 600);

    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("Ben Selfridge rocks");

    // glutFullScreen();

    glutDisplayFunc(&DrawGLScene);

    glutIdleFunc(&DrawGLScene);

    glutReshapeFunc(&ReSizeGLScene);

    glutKeyboardFunc(&keyPressed);

    glutSpecialFunc(&specialKeyPressed);

    InitGL(600, 600);

    glutMainLoop();

    return 0;

}

#endif /* TEST_CPP_ */
