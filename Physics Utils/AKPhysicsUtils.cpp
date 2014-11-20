//
//  AKPhysicsUtils.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/1/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "assert.h"
#include "AKPhysicsUtils.h"
#include "AKDefines.h"

void AKPhysicsUtils::changeParticlePlaceInTime(AKParticle *particle, double time)
{
    double velocityShift [2];
    PRODUCT_VECTOR_BY_NUMBER(particle->velocity, time, velocityShift, 2);
    VECTOR_SUM_FOR_ARRAYS(particle->sphere.center, velocityShift, particle->sphere.center, 2)
    assert(particle->sphere.center[0] + particle->sphere.radius < DISPLAY_WIDTH);
    assert(particle->sphere.center[0] - particle->sphere.radius >= -1);
    assert(particle->sphere.center[1] + particle->sphere.radius < DISPLAY_HEIGHT);
    assert(particle->sphere.center[1] - particle->sphere.radius >= -1);
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithAnotherParticle(AKParticle *particle1, AKParticle *particle2)
{
    double      m1              = particle1->mass,
                m2              = particle2->mass;
    double deltaVa[2], deltaRa[2];
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, particle1->velocity, deltaVa, 2)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->sphere.center, particle1->sphere.center, deltaRa, 2)
    double deltaProduct = 0, squareR      = 0;
    DOT_PRODUCT_FOR_ARRAYS(deltaVa, deltaRa, deltaProduct, 2)
    DOT_PRODUCT_FOR_ARRAYS(deltaRa, deltaRa, squareR, 2)
    double      mainPart        = deltaProduct / ((m1 + m2) * squareR);
    double      lambda1         = 2 * m2 * mainPart,
                lambda2         = 2 * m1 * mainPart;
    double lambdaVector1[2], lambdaVector2[2];
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda1, lambdaVector1, 2);
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda2, lambdaVector2, 2);
    double v1na[2], v2na[2];
    VECTOR_SUM_FOR_ARRAYS(particle1->velocity, lambdaVector1, v1na, 2)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, lambdaVector2, v2na, 2)
    
    ARRAY_FROM_VECTOR_XD(v1na, particle1->velocity, 2)
    ARRAY_FROM_VECTOR_XD(v2na, particle2->velocity, 2)
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, int *bound)
{
    double n[3];
    if (particle->is2Ddimension) {
        if (bound[0] == 0) {
            if (bound[1] == 0) {
                n[0] = 1; n[1] = 0;
            } else {
                n[0] = 0, n[1] = -1;
            }
        } else {
            if (bound[1] == 0) {
                n[0] = 0; n[1] = 1;
            } else {
                n[0] = -1; n[1] = 0;
            }
        }
    } else {
        // TODO: Need to implement for 3D case
    }
    double      dotProduct  = 0;
    DOT_PRODUCT_FOR_ARRAYS(particle->velocity, n, dotProduct, 2)
    double      dotProductByN[2];
    PRODUCT_VECTOR_BY_NUMBER(n, 2 * dotProduct, dotProductByN, 2)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle->velocity, dotProductByN, particle->velocity, 2)
}
