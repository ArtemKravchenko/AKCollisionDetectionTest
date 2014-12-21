//
//  openglinit.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "openglinit.h"

void display();
void timer_change(int time);

#pragma mark OPENGL INITIALIZATIONS

void AKOpenGLInit::initOpenGl(int *argc, char ** argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
    glutInitWindowPosition(200, 0);
    glutInitWindowSize(DISPLAY_WIDTH + 100, DISPLAY_HEIGHT + 100);
    glutCreateWindow("Collision Detection Test");
    glutDisplayFunc(display);
    glutTimerFunc(1, timer_change, 0);
    init();
}

void AKOpenGLInit::startModellingProcess()
{
    glutMainLoop();
}

void AKOpenGLInit::init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, DISPLAY_WIDTH + 100, 0, DISPLAY_HEIGHT + 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
}

void timer_change(int time)
{
    AKOpenGLInit::getInstance().delegate->handleTimerChanges(0.1);
    glutPostRedisplay();
    glutTimerFunc(1, timer_change, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    AKOpenGLInit::getInstance().delegate->redrawObjects();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);
    glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}