//
//  AKShapeVisualizer.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKShapeVisualizer.h"
#include <iostream>
#include <cmath>

using namespace std;

void AKShapeVisualizer::visualizeParticle(AKParticle* particle, float originX, float originY, float originZ)
{
#ifdef __glut_h__
    glLoadIdentity();
    float x = particle->sphere.center[0] + originX;
    float y = particle->sphere.center[1] + originY;
    float z = 0;
    if (!particle->sphere.is2dDimension) {
        z = 0 - abs(particle->sphere.center[2]) - (originZ);
    }
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(x, y, z);
    glutSolidSphere(particle->sphere.radius, 20, 2);
#endif
}
void AKShapeVisualizer::visualizeCell(AKCell* cell, float originX, float originY, float originZ)
{
#ifdef __glut_h__
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity();
    glBegin(GL_LINES);
    float leftX = cell->bounds.center[0] - cell->bounds.radius[0] + originX;
    float rightX = cell->bounds.center[0] + cell->bounds.radius[0] + originX;
    float bottomY = cell->bounds.center[1] - cell->bounds.radius[1] + originY;
    float topY = cell->bounds.center[1] + cell->bounds.radius[1] + originY;
    //cout << "leftX = " << leftX << ";" << "rightX = " << rightX << ";" << "bottomY = " << bottomY << ";" << "topY = " << topY << ";";
    if (cell->bounds.is2dDimension) {
        // Left
        glVertex2f(leftX, bottomY);
        glVertex2f(leftX, topY);
        // Top
        glVertex2f(leftX, topY);
        glVertex2f(rightX, topY);
        // Right
        glVertex2f(rightX, bottomY);
        glVertex2f(rightX, topY);
        // Bottom
        glVertex2f(leftX, bottomY);
        glVertex2f(rightX, bottomY);
    } else {
        float nearZ = cell->bounds.center[2] - cell->bounds.radius[2] - originZ;
        float furtherZ = 0 - (cell->bounds.center[2] + cell->bounds.radius[2]) - originZ;
        //cout << "nearZ = " << nearZ << ";" << "furtherZ = " << furtherZ << ";";
        //glutWireCube(cell->bounds.radius[0] + cell->bounds.center[0]);
        // NEAR
        glColor3f(1.0, 1.0, 1.0);
        // ------------------------------------------------------------------------------------------------------------ //
        // LEFT
        glVertex3f(leftX, bottomY, nearZ);
        glVertex3f(leftX, topY, nearZ);
        // TOP
        glVertex3f(leftX, topY, nearZ);
        glVertex3f(rightX, topY, nearZ);
        // RIGHT
        glVertex3f(rightX, bottomY, nearZ);
        glVertex3f(rightX, topY, nearZ);
        // BOTTOM
        glVertex3f(leftX, bottomY, nearZ);
        glVertex3f(rightX, bottomY, nearZ);
        
        // FURTHER
        glColor3f(0.7, 0.7, 0.7);
        // ------------------------------------------------------------------------------------------------------------ //
        // LEFT
        glVertex3f(leftX, bottomY, furtherZ);
        glVertex3f(leftX, topY, furtherZ);
        // TOP
        glVertex3f(leftX, topY, furtherZ);
        glVertex3f(rightX, topY, furtherZ);
        // RIGHT
        glVertex3f(rightX, bottomY, furtherZ);
        glVertex3f(rightX, topY, furtherZ);
        // BOTTOM
        glVertex3f(leftX, bottomY, furtherZ);
        glVertex3f(rightX, bottomY, furtherZ);
        
        // SIDE
        glColor3f(0.6, 0.5, 0.4);
        // ------------------------------------------------------------------------------------------------------------ //
        // LEFT, BOTTOM
        glVertex3f(leftX, bottomY, nearZ);
        glVertex3f(leftX, bottomY, furtherZ);
        // LEFT, TOP
        glVertex3f(leftX, topY, nearZ);
        glVertex3f(leftX, topY, furtherZ);
        // RIGHT BOTTOM
        glVertex3f(rightX, bottomY, nearZ);
        glVertex3f(rightX, bottomY, furtherZ);
        // RIGHT TOP
        glVertex3f(rightX, topY, nearZ);
        glVertex3f(rightX, topY, furtherZ);
    }
    //cout << endl;
    glEnd();
#endif
}