//
//  AKVisualizedModel.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_VISUALIZED_MODEL
#define AK_VISUALIZED_MODEL

#include "AKDefines.h"
#include "AKRectangle.h"

struct AKVisualizedModel {
    static bool isVisible;
    
    virtual void draw() { /* Need to implement in inheritance classes */ };
};

#endif