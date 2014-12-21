//
//  AKShapeVisualizer.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__AKShapeVisualizer__
#define __AKCollisionDetectionTest__AKShapeVisualizer__

#include <stdio.h>
#include "AKCell.h"
#include "AKParticle.h"

#ifndef AKOpenGLInclude
#define AKOpenGLInclude

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

#endif

class AKShapeVisualizer {
    
private:
    AKShapeVisualizer() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKShapeVisualizer(AKShapeVisualizer const&);              // Don't Implement
    void operator=(AKShapeVisualizer const&); // Don't implement
    
public:
    void visualizeParticle(AKParticle* particle, float originX = 0, float originY = 0, float orginZ = 0);
    void visualizeCell(AKCell* cell, float originX = 0, float originY = 0, float originZ = 0);
    
    static AKShapeVisualizer& getInstance()
    {
        static AKShapeVisualizer    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif /* defined(__AKCollisionDetectionTest__AKShapeVisualizer__) */
