//
//  AKParticle.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/24/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKParticle.h"

// Constructors and destructors

// Public methods
AKSphere const * AKParticle::getSphere() const
{
    return _sphere;
}
int AKParticle::getMass() const
{
    return _mass;
}
int AKParticle::getLocalTime() const
{
    return _localTime;
}
VectorXd const * AKParticle::getVelocity() const
{
    return _velocity;
}
