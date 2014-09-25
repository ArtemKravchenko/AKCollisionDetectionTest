//
//  AKGeometricUtils.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/24/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include "AKGeometricUtils.h"
#include "AKSphere.h"

int AKGeometricUtils::getTimeToCollisionBetweenTwoParticles(AKParticle const *p1, AKParticle const *p2)
{
    double time;
    VectorXd v1 = *p1->getVelocity(),
            v2 = *p2->getVelocity(),
            r10 = *p1->getSphere()->getCenter(),
            r20shift = *p2->getSphere()->getCenter() + (p1->getLocalTime() - p2->getLocalTime())*(*p2->getVelocity()),
            deltaV, deltaR;
    double L = p1->getSphere()->getRadius() + p2->getSphere()->getRadius(),
            A, B, D;
    deltaV = v1 - v2;
    deltaR = r10 - r20shift;
    A = deltaV.dot(deltaV);
    B = -(deltaR.dot(deltaV));
    D = B*B - A*((deltaR.dot(deltaR)) - L*L);
    time = (B - sqrtl(D)) / A;
    return time;
}
int AKGeometricUtils::getTimeToCollisionBetweenParticleAndBound(AKParticle const *particle, double bound, AKCollisionCompareType type, bool isSystemBound, bool isGreaterMeasure)
{
    double r1 = bound, r0, v;
    double radius = (particle->getSphere())->getRadius();
    VectorXd *vectorC = (VectorXd*)particle->getSphere()->getCenter();
    VectorXd *vectorV = (VectorXd*)particle->getVelocity();
    switch (type) {
        case AKCollisionCompareXType:
        {
            r0 = (*vectorC)[0];
            v =  (*vectorV)[0];
        }
            break;
        case AKCollisionCompareYType:
        {
            r0 = (*vectorC)[1];
            v =  (*vectorV)[1];
        }
            break;
        case AKCollisionCompareZType:
        {
            r0 = (*vectorC)[2];
            v =  (*vectorV)[2];
        }
            break;
        default:
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
            r0 -= radius;
        } else {
            r0 += radius;
        }
    }
    double time = (r1 - r0) / v - particle->getLocalTime();
    
    return time;
}