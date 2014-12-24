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
    if (!particle->sphere.is2dDimension) {
        gluLookAt(-07.0, 02.0, 10.0, 0.0, 0.0, -01.0, 0, 1, 0);
    }
    float x = particle->sphere.center[0] + originX;
    float y = particle->sphere.center[1] + originY;
    float z = 0;
    if (!particle->sphere.is2dDimension) {
        z = 0 - abs(particle->sphere.center[2]) - (originZ);
    }
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glTranslatef(x, y, z);
    glutSolidSphere(particle->sphere.radius, 20, 20);
    glLoadIdentity();
#endif
}
void AKShapeVisualizer::visualizeCell(AKCell* cell, float originX, float originY, float originZ)
{
#ifdef __glut_h__
    if (!cell->bounds.is2dDimension) {
        gluLookAt(-07.0, 02.0, 10.0, 0.0, 0.0, -01.0, 0, 1, 0);
    }
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glTranslatef(cell->bounds.center[0] + originX, cell->bounds.center[1] + originY, 0 - (cell->bounds.center[2]));
    glutWireCube(2 * cell->bounds.radius[0]);
    glLoadIdentity();
#endif
}