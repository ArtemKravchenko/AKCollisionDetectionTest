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
    vector<AKParticle*>         *particlesArray;
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
