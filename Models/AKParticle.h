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
    double      velocity[3];
    int         cellIndex;
    
    AKParticle(unsigned int dimension) : sphere(dimension)
    {
        is2Ddimension = (dimension == 2);
        wasInit = true;
    }
    ~AKParticle()
    {
        if (!wasInit) {
            throw std::exception();
        }
        mass = NULL;
        localTime = NULL;
        cellIndex = NULL;
    }
    
    bool operator==(const AKParticle &other) const {
        for (int i = 0; i < 3; i++) {
            if (!IS_EQUAL_WITH_ERROR(this->velocity[i], other.velocity[i])) {
                return false;
            }
        }
        return (this->sphere == other.sphere) && (this->mass == other.mass) && (IS_EQUAL_WITH_ERROR(this->localTime, other.localTime)) && this->cellIndex == other.cellIndex;
    }
    
    bool operator!=(const AKParticle &other) const {
        return !(*this == other);
    }
private:
    bool wasInit;
};

#endif