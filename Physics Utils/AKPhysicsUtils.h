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
  
private:
    AKPhysicsUtils() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKPhysicsUtils(AKPhysicsUtils const&);              // Don't Implement
    void operator=(AKPhysicsUtils const&); // Don't implement
    
public:
    // Newtown laws
    void changeParticlePlaceInTime(AKParticle *particle, double time);
    void changeParticleVelocityAfterCollisionWithAnotherParticle(AKParticle *particle1, AKParticle *particle2);
    void changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, double bound);
    // SINGLETON
    static AKPhysicsUtils& getInstance()
    {
        static AKPhysicsUtils    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif