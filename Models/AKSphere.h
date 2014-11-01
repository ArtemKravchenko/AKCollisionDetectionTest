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
    
    bool operator==(const AKSphere &other) const {
        DIMENSION_FROM_BOOL(this->is2Ddimension, count)
        for (int i = 0; i < count; i++) if (!IS_EQUAL_WITH_ERROR(this->center[i], other.center[i])) return false;
        return IS_EQUAL_WITH_ERROR(this->radius, other.radius);
    }
    
    bool operator!=(const AKSphere &other) const {
        return !(*this == other);
    }
};

#endif