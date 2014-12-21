//
//  main.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <iostream>
#include "AKVisualizedModel.h"
#include "openglhandheld.h"

using namespace std;

int main(int argc, char ** argv) {
    // Initialize GLUT and OpenGL
    AKOpenGLHandheld::getInstance().originX = 200;
    AKOpenGLHandheld::getInstance().originY = 200;
    AKOpenGLHandheld::getInstance().originZ = 0;
    AKOpenGLInit::getInstance().delegate = &AKOpenGLHandheld::getInstance();
    AKOpenGLHandheld::getInstance().is2DDimension = false;
    AKOpenGLHandheld::getInstance().setUpModels();
    AKOpenGLInit::getInstance().initOpenGl(&argc, argv);
    AKOpenGLInit::getInstance().startModellingProcess();
    return 0;
}

