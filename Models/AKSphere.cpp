//
//  AKSphere.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKSphere.h"

// Constructors and Destructors
AKSphere::AKSphere()
{
    set2Ddimension(true);
    _center = new VectorXd(2);
    _radius = 0;
}
AKSphere::AKSphere(double x, double y, int radius)
{
    set2Ddimension(true);
    _center = new VectorXd(2);
    (*_center)[0] = x;
    (*_center)[1] = y;
    _radius = radius;
}
AKSphere::AKSphere(double x, double y, double z, int radius)
{
    set2Ddimension(true);
    _center = new VectorXd(3);
    (*_center)[0] = x;
    (*_center)[1] = y;
    (*_center)[2] = z;
    _radius = radius;
}
AKSphere::~AKSphere()
{
    delete _center;
}
// Public functions
VectorXd const* AKSphere::getCenter() const
{
    return _center;
}
const int AKSphere::getRadius() const
{
    return _radius;
}
