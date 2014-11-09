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
        float x = _particle.sphere.center[0];
        float y = _particle.sphere.center[1];
        float z = 0;
        if (!_particle.is2Ddimension) {
            z = _particle.sphere.center[2];
        }
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(x, y, z);
        glutSolidSphere(_particle.sphere.radius, 20, 2);
    }
};

#endif /* defined(__AKCollisionDetectionTest__akvisualizedsphere__) */
