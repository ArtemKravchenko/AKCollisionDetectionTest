//
//  AKRectangle.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_RECTANGLE
#define AK_RECTANGLE

#include <Eigen/Dense>
#include "AKShape.h"

using namespace Eigen;

struct AKRectangle : public AKShape  {
    Eigen::VectorXd     *center;
    Eigen::VectorXd     *radius;
};

#endif