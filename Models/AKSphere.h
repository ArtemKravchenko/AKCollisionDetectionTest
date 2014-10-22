//
//  AKShpere.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_SPHERE
#define AK_SPHERE

#include "AKShape.h"

struct AKSphere : public AKShape {
    double*     center;
    double      radius;
};

#endif