//
//  AKBox.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_BOX
#define AK_BOX

#include "AKRectangle.h"

struct AKBox : public AKShape {
    AKRectangle     rectangle;
    
    bool operator==(const AKBox &other) const {
        return this->rectangle == other.rectangle;
    }
    
    bool operator!=(const AKBox &other) const {
        return !(*this == other);
    }
};

#endif