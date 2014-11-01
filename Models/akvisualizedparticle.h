//
//  akvisualizedsphere.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 11/1/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__akvisualizedsphere__
#define __AKCollisionDetectionTest__akvisualizedsphere__

#include <stdio.h>
#include "AKVisualizedModel.h"
#include "AKParticle.h"

class AKVisualizedParticle : public AKVisualizedModel {
    
private:
    AKParticle  _particle;
    
public:
    AKVisualizedParticle (AKParticle& particle) :  _particle(particle) {}
    
    virtual void draw() {
        glLoadIdentity();
        float x = (2 * _particle.sphere.center[0]) / GLOBAL_BOUND_X;
        float y = (2 * _particle.sphere.center[1]) / GLOBAL_BOUND_Y;
        float z = 0;
        if (!_particle.is2Ddimension) {
            z = (2 * _particle.sphere.center[2]) / GLOBAL_BOUND_Z;
        }
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(-1.0 + x, -1.0 + y, -1.0 + z);
        glutSolidSphere(_particle.sphere.radius / (2 * GLOBAL_MIN_RADIUS), 20, 2);
    }
};

#endif /* defined(__AKCollisionDetectionTest__akvisualizedsphere__) */
