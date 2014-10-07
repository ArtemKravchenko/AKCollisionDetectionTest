//
//  AKBox.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_BOX
#define AK_BOX

#include "AKVisualizedModel.h"
#include "AKRectangle.h"
#include "akpriorityqueue.h"

struct AKBox : public AKVisualizedModel {
    AKRectangle     *rectangle;
};

#endif