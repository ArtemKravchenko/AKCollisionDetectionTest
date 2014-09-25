//
//  AKGeometricUtils.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/20/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_GEOMETRIC_UTILS
#define AK_GEOMETRIC_UTILS

#include "AKParticle.h"

typedef enum {
    AKCollisionCompareXType,
    AKCollisionCompareYType,
    AKCollisionCompareZType
} AKCollisionCompareType;

class AKGeometricUtils {
    
private:
    AKGeometricUtils() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKGeometricUtils(AKGeometricUtils const&);              // Don't Implement
    void operator=(AKGeometricUtils const&); // Don't implement
    
public:
    int getTimeToCollisionBetweenTwoParticles(AKParticle const *p1, AKParticle const *p2);
    int getTimeToCollisionBetweenParticleAndBound(AKParticle const *particle, double bound, AKCollisionCompareType type, bool isSystemBound, bool isGreaterMeasure);
    // SINGLETON
    static AKGeometricUtils& getInstance()
    {
        static AKGeometricUtils    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif