//
//  AKCollisionDetectionLogic.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_COLLISION_DETECTION_LOGIC
#define AK_COLLISION_DETECTION_LOGIC

#include "AKPhysicsUtils.h"
#include "AKGeometricUtils.h"
#include "AKParticle.h"
#include <vector>
#include <queue>

class AKEvent {
    
    
public:
    
};

typedef std::vector<AKParticle*> AKParticlesList;
typedef std::priority_queue<AKEvent> AKEventsQueue;

class AKCollisionDetectionLogic {
    
private:
    AKParticlesList  *_particleList;
    AKEventsQueue    *_eventsQueue;
    
public:
    void fillEventsInQueue();
    void updateEventQueueInTime(int time);
    
};

#endif