//
//  openglinit.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "openglinit.h"

#pragma mark OPENGL INITIALIZATIONS

void AKOpenGLInit::initOpenGl(int *argc, char ** argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA| GLUT_DEPTH);
    glutInitWindowPosition(200, 0);
    glutInitWindowSize(delegate->getDisplayWidth() + 100, delegate->getDisplayHeight() + 100);
    glutCreateWindow("Collision Detection Test");
    glutDisplayFunc(display);
    glutTimerFunc(500, timer_change, 0);
    init();
}
void AKOpenGLInit::startModellingProcess()
{
    glutMainLoop();
}
void AKOpenGLInit::init()
{
    glClearColor(0,0,0,0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); // GL_SMOOTH
    glEnable(GL_CULL_FACE);
    /* LIGHTING */
    if (!delegate->getIs2Dimension()) {
        GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
        GLfloat mat_shininess[]={50.0};
        GLfloat light_position[]={-05.0, 02.0, 10.0, 0.0};//{1.0,1.0,1.0,0.0};
        GLfloat white_light[]={1.0,1.0,1.0,1.0};
        glClearColor(0.0,0.0,0.0,0.0);
        glShadeModel(GL_SMOOTH);
        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
        glLightfv(GL_LIGHT0,GL_POSITION,light_position);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
        glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
    }
}
void AKOpenGLInit::timer_change(int time)
{
    AKOpenGLInit::getInstance().delegate->handleTimerChanges(0.1);
    glutPostRedisplay();
    glutTimerFunc(1, timer_change, 0);
}
void AKOpenGLInit::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, AKOpenGLInit::getInstance().delegate->getDisplayWidth() + 100, 0, AKOpenGLInit::getInstance().delegate->getDisplayHeight() + 100, AKOpenGLInit::getInstance().delegate->getDisplayDepth(), 0 - AKOpenGLInit::getInstance().delegate->getDisplayDepth());
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    AKOpenGLInit::getInstance().delegate->redrawObjects();
    glutSwapBuffers();
}