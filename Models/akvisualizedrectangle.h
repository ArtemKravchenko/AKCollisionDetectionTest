//
//  akvisualizedrectangle.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 11/2/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AKCollisionDetectionTest_akvisualizedrectangle_h
#define AKCollisionDetectionTest_akvisualizedrectangle_h

#include <iostream>
#include "AKVisualizedModel.h"
#include "AKrectangle.h"

class AKVisualizedRectangle : public AKVisualizedModel {
    
    AKRectangle _rectangle;
public:
    AKVisualizedRectangle(AKRectangle &rectangle) : _rectangle(rectangle) { }
    virtual void draw() {
        glColor3f(1.0, 1.0, 1.0);
        glLoadIdentity();
        glBegin(GL_LINES);
        // Left
        glVertex2f(_rectangle.center[0] - _rectangle.radius[0], _rectangle.center[1] - _rectangle.radius[1]);
        glVertex2f(_rectangle.center[0] - _rectangle.radius[0], _rectangle.center[1] + _rectangle.radius[1]);
        // Top
        glVertex2f(_rectangle.center[0] - _rectangle.radius[0], _rectangle.center[1] + _rectangle.radius[1]);
        glVertex2f(_rectangle.center[0] + _rectangle.radius[0], _rectangle.center[1] + _rectangle.radius[1]);
        // Right
        glVertex2f(_rectangle.center[0] + _rectangle.radius[0], _rectangle.center[1] - _rectangle.radius[1]);
        glVertex2f(_rectangle.center[0] + _rectangle.radius[0], _rectangle.center[1] + _rectangle.radius[1]);
        // Bottom
        glVertex2f(_rectangle.center[0] - _rectangle.radius[0], _rectangle.center[1] - _rectangle.radius[1]);
        glVertex2f(_rectangle.center[0] + _rectangle.radius[0], _rectangle.center[1] - _rectangle.radius[1]);
        glEnd();
    }
};

#endif
