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
    int             measure[3];
    double          timeToEvent;
    unsigned        int index;
    int             nextCellIndex;
    
    AKEvent(unsigned int dimension)
    {
        firstParticle = new AKParticle(dimension);
        secondParticle = new AKParticle(dimension);
    }
    ~AKEvent()
    {
        index = NULL;
        nextCellIndex = NULL;
    }
    
    bool operator==(const AKEvent &other) const {
        return false;
    }
    
    bool operator!=(const AKEvent &other) const {
        return !(*this == other);
    }
};

#endif /* defined(__AKCollisionDetectionTest__akevent__) */
