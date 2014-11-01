//
//  AKRectangle.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_RECTANGLE
#define AK_RECTANGLE

#include "AKShape.h"

struct AKRectangle : public AKShape  {
    double*     center;
    double*     radius;
    
    bool operator==(const AKRectangle &other) const {
        DIMENSION_FROM_BOOL(this->is2Ddimension, count);
        for (int i = 0; i < count; i++) if (!IS_EQUAL_WITH_ERROR(this->center[i], other.center[i]) || !IS_EQUAL_WITH_ERROR(this->radius[i], other.radius[i])) return false; 
        return true;
    }
    
    bool operator!=(const AKRectangle &other) const {
        return !(*this == other);
    }
};

#endif