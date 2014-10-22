//
//  AKParticle.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_PARTICLE
#define AK_PARTICLE

#include "AKShape.h"
#include "AKSphere.h"

struct AKParticle : public AKShape  {
    AKSphere    sphere;
    double      mass;
    double      localTime;
    double      *velocity;
};

#endif