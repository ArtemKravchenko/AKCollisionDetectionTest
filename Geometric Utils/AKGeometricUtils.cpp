//
//  AKGeometricUtils.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/24/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <cmath>
#include "AKGeometricUtils.h"
#include "AKSphere.h"
#include "AKException.h"
#include "AKDefines.h"

double AKGeometricUtils::getTimeToCollisionBetweenTwoParticles(AKParticle const *p1, AKParticle const *p2)
{
    int dimesion = (p1->sphere.is2dDimension) ? 2 : 3;
    // NEED TO RESEARCH CHANGE OF BEHAVIOR OF COMPUTATION COLLISIOIN BETWEEN TWO PARTICLES
    // RELATED TO LOCAL TIME OF EACH PARTILCES
    // p1->sphere.center --> r10
    double tmpR20shift[dimesion];
    for (int i = 0; i < dimesion; i++) {
        tmpR20shift[i] = p2->sphere.center[i] + (abs(p1->localTime - p2->localTime))*((p2->velocity)[i]);
    }
    // tmpR20shift --> r20shift
    // p1->velocity --> v1
    // p2->velocity --> v2
    double deltaV[dimesion], deltaR[dimesion];
    double L = p1->sphere.radius + p2->sphere.radius,
            A = 0, B = 0, D = 0, deltaRSquare = 0;
    VECTOR_DIFERRENCE_FOR_ARRAYS(p1->velocity, p2->velocity, deltaV, dimesion)
    VECTOR_DIFERRENCE_FOR_ARRAYS(p1->sphere.center , p2->sphere.center, deltaR, dimesion)
    //VECTOR_DIFERRENCE_FOR_ARRAYS(p1->sphere.center , tmpR20shift, deltaR, count)
    DOT_PRODUCT_FOR_ARRAYS(deltaV, deltaV, A, dimesion)
    DOT_PRODUCT_FOR_ARRAYS(deltaR, deltaV, B, dimesion) // B = -(deltaR.dot(deltaV));
    B = 0 - B;
    DOT_PRODUCT_FOR_ARRAYS(deltaR, deltaR, deltaRSquare, dimesion)
    D = B*B - A*(deltaRSquare - L*L);
    double time = (B - sqrtl(D)) / A;
    return time + max(p1->localTime, p2->localTime);
}
double AKGeometricUtils::getTimeToCollisionBetweenParticleAndBound(AKParticle const *particle, double bound, AKCollisionCompareType type, bool isSystemBound, bool isGreaterMeasure)
{
    double r0, v;
    double radius = particle->sphere.radius;
    switch (type) {
        case AKCollisionCompareXType:
        {
            r0 = particle->sphere.center[0];
            v =  particle->velocity[0];
        }
            break;
        case AKCollisionCompareYType:
        {
            r0 = particle->sphere.center[1];
            v =  particle->velocity[1];
        }
            break;
        case AKCollisionCompareZType:
        {
            r0 = particle->sphere.center[2];
            v =  particle->velocity[2];
        }
            break;
        default:
            throw new wrongCollisionCompareTypeException();
            break;
    }
    if (isSystemBound) {
        if (isGreaterMeasure) {
            r0 += radius;
        } else {
            r0 -= radius;
        }
    } else {
        if (isGreaterMeasure) {
            r0 -= 0.0000001;
        } else {
            r0 += 0.0000001;
        }
    }
    double time = (bound - r0) / v + particle->localTime;
    return time;
}