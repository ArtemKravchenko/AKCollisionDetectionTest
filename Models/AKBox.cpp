//
//  AKBox.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/24/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKBox.h"

// Constructors and destructors

// Public methods
AKRectangle const * AKBox::getRectangle() const
{
    return _rectangle;
}