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
    double     center[3];
    double     radius[3];
    
    AKRectangle(unsigned int dimension)
    {
        is2Ddimension = (dimension == 2);
    }
    ~AKRectangle()
    {
    }
    
    bool operator==(const AKRectangle &other) const {
        for (int i = 0; i < 3; i++) if (!IS_EQUAL_WITH_ERROR(this->center[i], other.center[i]) || !IS_EQUAL_WITH_ERROR(this->radius[i], other.radius[i])) return false;
        return true;
    }
    
    bool operator!=(const AKRectangle &other) const {
        return !(*this == other);
    }
};

#endif