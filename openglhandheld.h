//
//  openglhandheld.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__openglhandheld__
#define __AKCollisionDetectionTest__openglhandheld__

#include <stdio.h>
#include "openglinit.h"
#include "AKCollisionDetectionLogic.h"

class AKOpenGLHandheld : public AKOpenGLInitDelegate {

private:
    AKCollisionDetectionLogic   *_collisionDetectionLogic;
    AKParticle        *_particle1, *_particle2, *_particle3, *_particle4, *_particle5, *_particle6, *_particle7, *_particle8,
    *_particle9, *_particle10, *_particle11, *_particle12, *_particle13, *_particle14, *_particle15, *_particle16;
    AKOpenGLHandheld() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKOpenGLHandheld(AKOpenGLHandheld const&);              // Don't Implement
    void operator=(AKOpenGLHandheld const&); // Don't implement
    
public:
    void setUpModels();
    virtual void handleTimerChanges(double time);
    virtual void redrawObjects();
    static AKOpenGLHandheld& getInstance()
    {
        static AKOpenGLHandheld    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif /* defined(__AKCollisionDetectionTest__openglhandheld__) */
