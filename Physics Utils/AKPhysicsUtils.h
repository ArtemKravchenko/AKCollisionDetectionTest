//
//  AKPhysicsUtils.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_PHYSICS_UTILS
#define AK_PHYSICS_UTILS

#include "AKParticle.h"

class AKPhysicsUtils {
    
    
public:
    // Newtown laws
    void changeParticlePlaceInTime(AKParticle *particle);
    void changeVelocityAfterCollisionTwoParticles(AKParticle *particle1, AKParticle2 *particle2);
    void changeVelocityAfterCollisionParticleAndBound(AKParticle *particle, double bound);
};

#endif
