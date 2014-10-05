//
//  AKPhysicsUtils.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/1/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKPhysicsUtils.h"

void AKPhysicsUtils::changeParticlePlaceInTime(AKParticle *particle, double time)
{
    VectorXd oldPosition = (*particle->sphere->center);
    VectorXd newPosition = oldPosition + ((*particle->velocity) *  (time - particle->localTime));
    (*particle->sphere->center) = newPosition;
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithAnotherParticle(AKParticle *particle1, AKParticle *particle2)
{
    // TODO: Need to implement
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, double bound)
{
    // TODO: Need to implement
}