//
//  AKPhysicsUtils.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/1/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKPhysicsUtils.h"
#include "AKDefines.h"

void AKPhysicsUtils::changeParticlePlaceInTime(AKParticle *particle, double time)
{
    DIMENSION_FROM_BOOL(particle->is2Ddimension, count)
    double *newPosition = new double[count], *velocityShift = new double[count];
    PRODUCT_VECTOR_BY_NUMBER(particle->velocity, time, velocityShift, count);
    VECTOR_SUM_FOR_ARRAYS(particle->sphere.center, velocityShift, particle->sphere.center, count)
    delete [] newPosition; delete [] velocityShift;
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithAnotherParticle(AKParticle *particle1, AKParticle *particle2)
{
    double      m1              = particle1->mass,
                m2              = particle2->mass;
    DIMENSION_FROM_BOOL(particle1->is2Ddimension, count)
    double *deltaVa = new double[count], *deltaRa = new double[count];
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, particle1->velocity, deltaVa, count)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->sphere.center, particle1->sphere.center, deltaRa, count)
    double deltaProduct = 0, squareR      = 0;
    DOT_PRODUCT_FOR_ARRAYS(deltaVa, deltaRa, deltaProduct, count)
    DOT_PRODUCT_FOR_ARRAYS(deltaRa, deltaRa, squareR, count)
    double      mainPart        = deltaProduct / ((m1 + m2) * squareR);
    double      lambda1         = 2 * m2 * mainPart,
                lambda2         = 2 * m1 * mainPart;
    double *lambdaVector1 = new double[count], *lambdaVector2 = new double[count];
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda1, lambdaVector1, count);
    PRODUCT_VECTOR_BY_NUMBER(deltaRa, lambda2, lambdaVector2, count);
    double *v1na = new double[count], *v2na = new double[count];
    VECTOR_SUM_FOR_ARRAYS(particle1->velocity, lambdaVector1, v1na, count)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle2->velocity, lambdaVector2, v2na, count)
    
    ARRAY_FROM_VECTOR_XD(v1na, particle1->velocity, count)
    ARRAY_FROM_VECTOR_XD(v2na, particle2->velocity, count)
    delete [] deltaVa; delete [] deltaRa; delete [] lambdaVector1; delete [] lambdaVector2; delete [] v1na; delete [] v2na;
}
void AKPhysicsUtils::changeParticleVelocityAfterCollisionWithBound(AKParticle* particle, int *bound)
{
    DIMENSION_FROM_BOOL(particle->is2Ddimension, count)
    double *n = new double[count];
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
    DOT_PRODUCT_FOR_ARRAYS(particle->velocity, n, dotProduct, count)
    double      *dotProductByN = new double[count];
    PRODUCT_VECTOR_BY_NUMBER(n, 2 * dotProduct, dotProductByN, count)
    VECTOR_DIFERRENCE_FOR_ARRAYS(particle->velocity, dotProductByN, particle->velocity, count)
    delete [] n;
}
