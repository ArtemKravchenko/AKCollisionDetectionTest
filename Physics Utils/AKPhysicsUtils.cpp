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
    int dimension = (particle->sphere.is2dDimension) ? 2 : 3;
    double velocityShift [dimension];
    PRODUCT_VECTOR_BY_NUMBER(particle->velocity, time, velocityShift, dimension);
    VECTOR_SUM_FOR_ARRAYS(particle->sphere.center, velocityShift, particle->sphere.center, dimension)
    assert(particle->sphere.center[0] + particle->sphere.radius <= DISPLAY_WIDTH + 1);
    assert(particle->sphere.center[0] - particle->sphere.radius >= -1);
    assert(particle->sphere.center[1] + particle->sphere.radius <= DISPLAY_HEIGHT + 1);
    assert(particle->sphere.center[1] - particle->sphere.radius >= -1);
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithAnotherParticle(AKParticle *particle1, AKParticle *particle2)
{
    int dimension = (particle1->sphere.is2dDimension) ? 2 : 3;
    double      m1              = particle1->mass,
                m2              = particle2->mass;
    double deltaVa[dimension], deltaRa[dimension];
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, particle1->velocity, deltaVa, dimension)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->sphere.center, particle1->sphere.center, deltaRa, dimension)
    double deltaProduct = 0, squareR      = 0;
    DOT_PRODUCT_FOR_ARRAYS(deltaVa, deltaRa, deltaProduct, dimension)
    DOT_PRODUCT_FOR_ARRAYS(deltaRa, deltaRa, squareR, dimension)
    double      mainPart        = deltaProduct / ((m1 + m2) * squareR);
    double      lambda1         = 2 * m2 * mainPart,
                lambda2         = 2 * m1 * mainPart;
    double lambdaVector1[dimension], lambdaVector2[dimension];
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda1, lambdaVector1, dimension);
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda2, lambdaVector2, dimension);
    double v1na[dimension], v2na[dimension];
    VECTOR_SUM_FOR_ARRAYS(particle1->velocity, lambdaVector1, v1na, dimension)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, lambdaVector2, v2na, dimension)
    
    ARRAY_FROM_VECTOR_XD(v1na, particle1->velocity, dimension)
    ARRAY_FROM_VECTOR_XD(v2na, particle2->velocity, dimension)
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, int *n)
{
    int dimension = (particle->sphere.is2dDimension) ? 2 : 3;
    double      dotProduct  = 0;
    DOT_PRODUCT_FOR_ARRAYS(particle->velocity, n, dotProduct, dimension)
    double      dotProductByN[dimension];
    PRODUCT_VECTOR_BY_NUMBER(n, 2 * dotProduct, dotProductByN, dimension)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle->velocity, dotProductByN, particle->velocity, dimension)
}
