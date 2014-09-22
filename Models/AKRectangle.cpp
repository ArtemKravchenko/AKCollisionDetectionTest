//
//  AKRectangle.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKRectangle.h"

// Constructors and Destructors
AKRectangle::AKRectangle()
{
    set2Ddimension(true);
    _center = new VectorXd(2);
    _radius = new VectorXd(2);
}
AKRectangle::AKRectangle(float xCenter, float yCenter, float xRadius, float yRadius)
{
    set2Ddimension(true);
    _center = new VectorXd(2);
    (*_center)[0] = xCenter;
    (*_center)[1] = yCenter;
    _radius = new VectorXd(2);
    (*_radius)[0] = xRadius;
    (*_radius)[1] = yRadius;
}
AKRectangle::AKRectangle(float xCenter, float yCenter, float zCenter, float xRadius, float yRadius, float zRadius)
{
    set2Ddimension(false);
    _center = new VectorXd(3);
    (*_center)[0] = xCenter;
    (*_center)[1] = yCenter;
    (*_center)[2] = zCenter;
    _radius = new VectorXd(3);
    (*_radius)[0] = xRadius;
    (*_radius)[1] = yRadius;
    (*_radius)[2] = zRadius;
}
AKRectangle::~AKRectangle()
{
    delete _center;
    delete _radius;
}
// Public functions
VectorXd const* AKRectangle::getCenter() const
{
    return _center;
}
VectorXd const* AKRectangle::getRadius() const
{
    return _radius;
}