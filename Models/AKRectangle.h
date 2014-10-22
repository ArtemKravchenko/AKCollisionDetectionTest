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
};

#endif