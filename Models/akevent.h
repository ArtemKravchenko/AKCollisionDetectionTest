//
//  akevent.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/8/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__akevent__
#define __AKCollisionDetectionTest__akevent__

#include <stdio.h>
#include "AKParticle.h"

typedef enum {
    AKEventParticleToParticleType,
    AKEventParticleToBoundType,
    AKEventParticleToCellType,
} AKEventType;

struct AKEvent {
    AKEventType     eventType;
    AKParticle      *firstParticle;
    AKParticle      *secondParticle;
    double          measure;
    double          timeToEvent;
    unsigned        int index;
};

#endif /* defined(__AKCollisionDetectionTest__akevent__) */
