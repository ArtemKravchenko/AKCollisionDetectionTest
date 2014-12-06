//
//  openglhandheld.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "openglhandheld.h"
#include "AKSphere.h"
#include "AKSystemGenerator.h"

void AKOpenGLHandheld::setUpModels()
{
    _collisionDetectionLogic = new AKCollisionDetectionLogic();
    AKBox *bounds = new AKBox(2);
    bounds->center[0] = DISPLAY_WIDTH / 2; bounds->center[1] = DISPLAY_WIDTH / 2;
    bounds->radius[0] = DISPLAY_HEIGHT / 2; bounds->radius[1] = DISPLAY_HEIGHT / 2;
    
    _collisionDetectionLogic->setBound(bounds, 4, 4);
    _particle1 = new AKParticle(2);
    _particle2 = new AKParticle(2);
    _particle3 = new AKParticle(2);
    _particle4 = new AKParticle(2);
    _particle5 = new AKParticle(2);
    _particle6 = new AKParticle(2);
    _particle7 = new AKParticle(2);
    _particle8 = new AKParticle(2);
    _particle9 = new AKParticle(2);
    _particle10 = new AKParticle(2);
    _particle11 = new AKParticle(2);
    _particle12 = new AKParticle(2);
    _particle13 = new AKParticle(2);
    _particle14 = new AKParticle(2);
    _particle15 = new AKParticle(2);
    _particle16 = new AKParticle(2);
    
    _particle1->sphere.center[0] = 210; _particle1->sphere.center[1] = 400; _particle1->sphere.radius = 15;
    _particle1->velocity[0] = 5; _particle1->velocity[1] = -5;
    _particle1->mass = 2;
    _particle1->localTime = 0;
    
    _particle2->sphere.center[0] = 584.5; _particle2->sphere.center[1] = 15.5; _particle2->sphere.radius = 15;
    _particle2->velocity[0] = -5; _particle2->velocity[1] = 5;
    _particle2->mass = 2;
    
    _particle3->sphere.center[0] = 400; _particle3->sphere.center[1] = 200; _particle3->sphere.radius = 15;
    _particle3->velocity[0] = -5; _particle3->velocity[1] = 5;
    _particle3->mass = 2;
    _particle3->localTime = 0;
    
    _particle4->sphere.center[0] = 15.5; _particle4->sphere.center[1] = 584.5; _particle4->sphere.radius = 15;
    _particle4->velocity[0] = 5; _particle4->velocity[1] = -5;
    _particle4->mass = 2;
    _particle4->localTime = 0;
    
    _particle5->sphere.center[0] = 101.182; _particle5->sphere.center[1] = 200; _particle5->sphere.radius = 15;
    _particle5->velocity[0] = 2.34774; _particle5->velocity[1] = -6.66994;
    _particle5->mass = 2;
    _particle5->localTime = 0;
    
    _particle6->sphere.center[0] = 400; _particle6->sphere.center[1] = 500; _particle6->sphere.radius = 15;
    _particle6->velocity[0] = -5; _particle6->velocity[1] = 5;
    _particle6->mass = 2;
    _particle6->localTime = 0;
    
    _particle7->sphere.center[0] = 20; _particle7->sphere.center[1] = 200; _particle7->sphere.radius = 15;
    _particle7->velocity[0] = -5; _particle7->velocity[1] = 5;
    _particle7->mass = 2;
    _particle7->localTime = 0;
    
    _particle8->sphere.center[0] = 45.5; _particle8->sphere.center[1] = 504.5; _particle8->sphere.radius = 15;
    _particle8->velocity[0] = 5; _particle8->velocity[1] = -5;
    _particle8->mass = 2;
    _particle8->localTime = 0;
    
    _particle9->sphere.center[0] = 50; _particle9->sphere.center[1] = 50; _particle9->sphere.radius = 15;
    _particle9->velocity[0] = 5; _particle9->velocity[1] = -5;
    _particle9->mass = 2;
    _particle9->localTime = 0;
    
    _particle10->sphere.center[0] = 80; _particle10->sphere.center[1] = 80; _particle10->sphere.radius = 15;
    _particle10->velocity[0] = -5; _particle10->velocity[1] = 5;
    _particle10->mass = 2;
    
    _particle11->sphere.center[0] = 110; _particle11->sphere.center[1] = 150; _particle11->sphere.radius = 15;
    _particle11->velocity[0] = -5; _particle11->velocity[1] = 5;
    _particle11->mass = 2;
    _particle11->localTime = 0;
    
    _particle12->sphere.center[0] = 300; _particle12->sphere.center[1] = 300; _particle12->sphere.radius = 15;
    _particle12->velocity[0] = 5; _particle12->velocity[1] = -5;
    _particle12->mass = 2;
    _particle12->localTime = 0;
    
    _particle13->sphere.center[0] = 350; _particle13->sphere.center[1] = 350; _particle13->sphere.radius = 15;
    _particle13->velocity[0] = 10; _particle13->velocity[1] = 6.66994;
    _particle13->mass = 2;
    _particle13->localTime = 0;
    
    _particle14->sphere.center[0] = 400; _particle14->sphere.center[1] = 400; _particle14->sphere.radius = 15;
    _particle14->velocity[0] = -5; _particle14->velocity[1] = 5;
    _particle14->mass = 2;
    _particle14->localTime = 0;
    
    _particle15->sphere.center[0] = 500; _particle15->sphere.center[1] = 100; _particle15->sphere.radius = 15;
    _particle15->velocity[0] = -5; _particle15->velocity[1] = 5;
    _particle15->mass = 2;
    _particle15->localTime = 0;
    
    _particle16->sphere.center[0] = 550; _particle16->sphere.center[1] = 550; _particle16->sphere.radius = 15;
    _particle16->velocity[0] = 5; _particle16->velocity[1] = -5;
    _particle16->mass = 2;
    _particle16->localTime = 0;
    
    AKParticlesList *_particleList = new AKParticlesList();

    _particleList->push_back(_particle1);
    _particleList->push_back(_particle2);
    _particleList->push_back(_particle3);
    _particleList->push_back(_particle4);
    _particleList->push_back(_particle5);
    _particleList->push_back(_particle6);
    _particleList->push_back(_particle7);
    _particleList->push_back(_particle8);
    _particleList->push_back(_particle9);
    _particleList->push_back(_particle10);
    _particleList->push_back(_particle11);
    _particleList->push_back(_particle12);
    _particleList->push_back(_particle13);
    _particleList->push_back(_particle14);
    _particleList->push_back(_particle15);
    _particleList->push_back(_particle16);
    _collisionDetectionLogic->setParticlesList(_particleList);
     _collisionDetectionLogic->fillEventsInQueue();
}

#pragma mark - OpenGLInit delegate

void AKOpenGLHandheld::redrawObjects()
{
    _collisionDetectionLogic->drawCells();
    _particle1->draw();
    _particle2->draw();
    _particle3->draw();
    _particle4->draw();
    _particle5->draw();
    _particle6->draw();
    _particle7->draw();
    _particle8->draw();
    _particle9->draw();
    _particle10->draw();
    _particle11->draw();
    _particle12->draw();
    _particle13->draw();
    _particle14->draw();
    _particle15->draw();
    _particle16->draw();
}

void AKOpenGLHandheld::handleTimerChanges(double time)
{
    _collisionDetectionLogic->updateEventQueueInTime(time);
}