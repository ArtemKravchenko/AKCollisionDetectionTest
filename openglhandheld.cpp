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
    AKParticle *particle1;
    AKParticle *particle2;
    AKParticle *particle3;
    AKParticle *particle4;
    AKParticle *particle5;
    AKParticle *particle6;
    AKParticle *particle7;
    AKParticle *particle8;
    AKParticle *particle9;
    AKParticle *particle10;
    AKParticle *particle11;
    AKParticle *particle12;
    AKParticle *particle13;
    AKParticle *particle14;
    AKParticle *particle15;
    AKParticle *particle16;
    
    _collisionDetectionLogic = new AKCollisionDetectionLogic();
    AKBox *bounds = new AKBox(2);
    bounds->rectangle.center[0] = DISPLAY_WIDTH / 2; bounds->rectangle.center[1] = DISPLAY_WIDTH / 2;
    bounds->rectangle.radius[0] = DISPLAY_HEIGHT / 2; bounds->rectangle.radius[1] = DISPLAY_HEIGHT / 2;
    
    _collisionDetectionLogic->setBound(bounds, 4, 4);
    particle1 = new AKParticle(2);
    particle2 = new AKParticle(2);
    particle3 = new AKParticle(2);
    particle4 = new AKParticle(2);
    particle5 = new AKParticle(2);
    particle6 = new AKParticle(2);
    particle7 = new AKParticle(2);
    particle8 = new AKParticle(2);
    particle9 = new AKParticle(2);
    particle10 = new AKParticle(2);
    particle11 = new AKParticle(2);
    particle12 = new AKParticle(2);
    particle13 = new AKParticle(2);
    particle14 = new AKParticle(2);
    particle15 = new AKParticle(2);
    particle16 = new AKParticle(2);
    
    particle1->sphere.center[0] = 210; particle1->sphere.center[1] = 400; particle1->sphere.radius = 15;
    particle1->velocity[0] = 5; particle1->velocity[1] = -5;
    particle1->mass = 2;
    particle1->localTime = 0;
    
    particle2->sphere.center[0] = 584.5; particle2->sphere.center[1] = 15.5; particle2->sphere.radius = 15;
    particle2->velocity[0] = -5; particle2->velocity[1] = 5;
    particle2->mass = 2;
    
    particle3->sphere.center[0] = 400; particle3->sphere.center[1] = 200; particle3->sphere.radius = 15;
    particle3->velocity[0] = -5; particle3->velocity[1] = 5;
    particle3->mass = 2;
    particle3->localTime = 0;
    
    particle4->sphere.center[0] = 15.5; particle4->sphere.center[1] = 584.5; particle4->sphere.radius = 15;
    particle4->velocity[0] = 5; particle4->velocity[1] = -5;
    particle4->mass = 2;
    particle4->localTime = 0;
    
    particle5->sphere.center[0] = 101.182; particle5->sphere.center[1] = 200; particle5->sphere.radius = 15;
    particle5->velocity[0] = 2.34774; particle5->velocity[1] = -6.66994;
    particle5->mass = 2;
    particle5->localTime = 0;
    
    particle6->sphere.center[0] = 400; particle6->sphere.center[1] = 500; particle6->sphere.radius = 15;
    particle6->velocity[0] = -5; particle6->velocity[1] = 5;
    particle6->mass = 2;
    particle6->localTime = 0;
    
    particle7->sphere.center[0] = 20; particle7->sphere.center[1] = 200; particle7->sphere.radius = 15;
    particle7->velocity[0] = -5; particle7->velocity[1] = 5;
    particle7->mass = 2;
    particle7->localTime = 0;
    
    particle8->sphere.center[0] = 45.5; particle8->sphere.center[1] = 504.5; particle8->sphere.radius = 15;
    particle8->velocity[0] = 5; particle8->velocity[1] = -5;
    particle8->mass = 2;
    particle8->localTime = 0;
    
    particle9->sphere.center[0] = 50; particle9->sphere.center[1] = 50; particle9->sphere.radius = 15;
    particle9->velocity[0] = 5; particle9->velocity[1] = -5;
    particle9->mass = 2;
    particle9->localTime = 0;
    
    particle10->sphere.center[0] = 80; particle10->sphere.center[1] = 80; particle10->sphere.radius = 15;
    particle10->velocity[0] = -5; particle10->velocity[1] = 5;
    particle10->mass = 2;
    
    particle11->sphere.center[0] = 110; particle11->sphere.center[1] = 150; particle11->sphere.radius = 15;
    particle11->velocity[0] = -5; particle11->velocity[1] = 5;
    particle11->mass = 2;
    particle11->localTime = 0;
    
    particle12->sphere.center[0] = 300; particle12->sphere.center[1] = 300; particle12->sphere.radius = 15;
    particle12->velocity[0] = 5; particle12->velocity[1] = -5;
    particle12->mass = 2;
    particle12->localTime = 0;
    
    particle13->sphere.center[0] = 350; particle13->sphere.center[1] = 350; particle13->sphere.radius = 15;
    particle13->velocity[0] = 10; particle13->velocity[1] = 6.66994;
    particle13->mass = 2;
    particle13->localTime = 0;
    
    particle14->sphere.center[0] = 400; particle14->sphere.center[1] = 400; particle14->sphere.radius = 15;
    particle14->velocity[0] = -5; particle14->velocity[1] = 5;
    particle14->mass = 2;
    particle14->localTime = 0;
    
    particle15->sphere.center[0] = 500; particle15->sphere.center[1] = 100; particle15->sphere.radius = 15;
    particle15->velocity[0] = -5; particle15->velocity[1] = 5;
    particle15->mass = 2;
    particle15->localTime = 0;
    
    particle16->sphere.center[0] = 550; particle16->sphere.center[1] = 550; particle16->sphere.radius = 15;
    particle16->velocity[0] = 5; particle16->velocity[1] = -5;
    particle16->mass = 2;
    particle16->localTime = 0;
    
    _particle1 = new AKVisualizedParticle(*particle1);
    _particle2 = new AKVisualizedParticle(*particle2);
    _particle3 = new AKVisualizedParticle(*particle3);
    _particle4 = new AKVisualizedParticle(*particle4);
    _particle5 = new AKVisualizedParticle(*particle5);
    _particle6 = new AKVisualizedParticle(*particle6);
    _particle7 = new AKVisualizedParticle(*particle7);
    _particle8 = new AKVisualizedParticle(*particle8);
    _particle9 = new AKVisualizedParticle(*particle9);
    _particle10 = new AKVisualizedParticle(*particle10);
    _particle11 = new AKVisualizedParticle(*particle11);
    _particle12 = new AKVisualizedParticle(*particle12);
    _particle13 = new AKVisualizedParticle(*particle13);
    _particle14 = new AKVisualizedParticle(*particle14);
    _particle15 = new AKVisualizedParticle(*particle15);
    _particle16 = new AKVisualizedParticle(*particle16);
    /*
     */
    AKParticlesList *particleList = new AKParticlesList();

    particleList->push_back(particle1);
    particleList->push_back(particle2);
    particleList->push_back(particle3);
    particleList->push_back(particle4);
    particleList->push_back(particle5);
    particleList->push_back(particle6);
    particleList->push_back(particle7);
    particleList->push_back(particle8);
    particleList->push_back(particle9);
    particleList->push_back(particle10);
    particleList->push_back(particle11);
    particleList->push_back(particle12);
    particleList->push_back(particle13);
    particleList->push_back(particle14);
    particleList->push_back(particle15);
    particleList->push_back(particle16);
    /*
     */
    _collisionDetectionLogic->setParticlesList(particleList);
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
    /*
     */
}

void AKOpenGLHandheld::handleTimerChanges(double time)
{
    _collisionDetectionLogic->updateEventQueueInTime(time);
}