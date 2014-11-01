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
    int         cellIndex;
    
    bool operator==(const AKParticle &other) const {
        DIMENSION_FROM_BOOL(this->is2Ddimension, count);
        for (int i = 0; i < count; i++) {
            if (!IS_EQUAL_WITH_ERROR(this->velocity[i], other.velocity[i])) {
                return false;
            }
        }
        return (this->sphere == other.sphere) && (this->mass == other.mass) && (IS_EQUAL_WITH_ERROR(this->localTime, other.localTime)) && this->cellIndex == other.cellIndex;
    }
    
    bool operator!=(const AKParticle &other) const {
        return !(*this == other);
    }
};

#endif