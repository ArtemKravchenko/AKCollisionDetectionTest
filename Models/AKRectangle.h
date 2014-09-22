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

class AKRectangle : public AKShape  {
    
private:
    Eigen::VectorXd     *_center;
    Eigen::VectorXd     *_radius;
    
public:
    AKRectangle();
    AKRectangle(float xCenter, float yCenter, float xRadius, float yRadius);
    AKRectangle(float xCenter, float yCenter, float zCenter, float xRadius, float yRadius, float zRadius);
    ~AKRectangle();
    VectorXd const* getCenter() const;
    VectorXd const* getRadius() const;
};

#endif