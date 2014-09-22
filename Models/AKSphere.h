//
//  AKShpere.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_SPHERE
#define AK_SPHERE

#include <Eigen/Dense>
#include "AKShape.h"

using namespace Eigen;

class AKSphere : public AKShape {
    
private:
    Eigen::VectorXd     *_center;
    int                 _radius;
    
public:
    AKSphere();
    AKSphere(double x, double y, int radius);
    AKSphere(double x, double y, double z, int radius);
    ~AKSphere();
    
    VectorXd const * getCenter() const;
    const int getRadius() const;
};

#endif