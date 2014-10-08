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
    double      m1              = particle1->mass,
                m2              = particle2->mass;
    VectorXd    v1              = *(particle1->velocity),
                v2              = *(particle2->velocity);
    VectorXd    r1              = *(particle1->sphere->center),
                r2              = *(particle2->sphere->center);
    VectorXd    deltaV          = v2 - v1,
                deltaR          = r2 - r1;
    double      deltaProduct    = deltaV.dot(deltaR);
    double      squareR         = deltaR.dot(deltaR);
    double      lambda1         = 2 * m2 * deltaProduct / (m1 + m2) * squareR,
                lambda2         = 2 * m1 * deltaProduct / (m1 + m2) * squareR;
    VectorXd    v1n = v1 + lambda1 * deltaR,
                v2n = v2 - lambda2 * deltaR;
    
    *(particle1->velocity) = v1n;
    *(particle2->velocity) = v2n;
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, double bound)
{
    VectorXd    n,
                v           = *(particle->velocity);
    double      dotProduct  = v.dot(n);
    VectorXd    v1n = v - 2 * n * dotProduct;
    
    *(particle->velocity) = v1n;
}