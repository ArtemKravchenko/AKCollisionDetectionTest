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
#include "AKCollisionDetectionDiscreteTimeLogic3D.h"

class AKOpenGLHandheld : public AKOpenGLInitDelegate {

private:
    AKCollisionDetectionDiscreteTimeLogic   *_collisionDetectionLogic;
    vector<AKParticle*>                     *_particlesArray;
    float                                   _displayWidth, _displayHeight, _displayDepth;
    bool                                    _displayWidthWasSet, _displayHeightWasSet, _displayDepthWasSet;
    
    void initSystemData();
    // SINGLTONE
    AKOpenGLHandheld() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKOpenGLHandheld(AKOpenGLHandheld const&);              // Don't Implement
    void operator=(AKOpenGLHandheld const&); // Don't implement
    
public:
    float                                   originX, originY, originZ;
    bool                                    is2DDimension;
    void                                    setUpModels();
    /* ----------------------------- DELEGATE IMPLEMENTATION ----------------------------- */
    virtual void                            handleTimerChanges(double time);
    virtual void                            redrawObjects();
    virtual bool                            getIs2Dimension();
    virtual float                           getDisplayWidth();
    virtual float                           getDisplayHeight();
    virtual float                           getDisplayDepth();
    /*------------------------------------------------------------------------------------*/
    void                                    setDisplayWidth(float);
    void                                    setDisplayHeight(float);
    void                                    setDisplayDepth(float);
    // SINGLTONE
    static AKOpenGLHandheld& getInstance()
    {
        static AKOpenGLHandheld    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif /* defined(__AKCollisionDetectionTest__openglhandheld__) */
