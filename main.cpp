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
    AKOpenGLHandheld::getInstance().originX = 50;
    AKOpenGLHandheld::getInstance().originY = 50;
    AKOpenGLHandheld::getInstance().originZ = 0;
    AKOpenGLHandheld::getInstance().setDisplayWidth(600);
    AKOpenGLHandheld::getInstance().setDisplayHeight(600);
    AKOpenGLInit::getInstance().delegate = &AKOpenGLHandheld::getInstance();
    AKOpenGLHandheld::getInstance().is2DDimension = true;
    AKOpenGLHandheld::getInstance().setUpModels();
    AKOpenGLInit::getInstance().initOpenGl(&argc, argv);
    AKOpenGLInit::getInstance().startModellingProcess();
    return 0;
}

