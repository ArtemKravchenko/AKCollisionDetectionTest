//
//  openglhandheld.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "openglhandheld.h"
#include "AKSphere.h"

void AKOpenGLHandheld::setUpModels()
{
    AKParticle *particle1;
    AKParticle *particle2;
    AKParticle *particle3;
    AKParticle *particle4;
    
    _collisionDetectionLogic = new AKCollisionDetectionLogic();
    AKBox *bounds = new AKBox();
    bounds->rectangle = AKRectangle();
    bounds->rectangle.center = new double[2];
    bounds->rectangle.radius = new double[2];
    bounds->rectangle.center[0] = 300; bounds->rectangle.center[1] = 300;
    bounds->rectangle.radius[0] = 300; bounds->rectangle.radius[1] = 300;
    
    GLOBAL_MIN_RADIUS = 300;
    GLOBAL_BOUND_X = 600;
    GLOBAL_BOUND_Y = 600;
    
    _collisionDetectionLogic->setBound(bounds, 6, 6);
    particle1 = new AKParticle();
    particle2 = new AKParticle();
    particle3 = new AKParticle();
    particle4 = new AKParticle();
    particle1->is2Ddimension = true; particle2->is2Ddimension = true;
    particle3->is2Ddimension = true; particle4->is2Ddimension = true;
    
    AKSphere sphere1, sphere2, sphere3, sphere4;
    
    sphere1.is2Ddimension = true; sphere2.is2Ddimension = true; sphere3.is2Ddimension = true; sphere4.is2Ddimension = true;
    
    sphere1.center = new double[2]; sphere1.center[0] = 370; sphere1.center[1] = 156; sphere1.radius = 15;
    particle1->velocity = new double[2]; particle1->velocity[0] = 1; particle1->velocity[1] = 1;
    particle1->sphere = sphere1;
    particle1->mass = 2;
    particle1->localTime = 0;
    
    sphere2.center = new double[2]; sphere2.center[0] = 270; sphere2.center[1] = 412; sphere2.radius = 15;
    particle2->velocity = new double[2]; particle2->velocity[0] = 5; particle2->velocity[1] = 5;
    particle2->sphere = sphere2;
    particle2->mass = 2;
    particle2->localTime = 0;
    
    sphere3.center = new double[2]; sphere3.center[0] = 472; sphere3.center[1] = 34; sphere3.radius = 15;
    particle3->velocity = new double[2]; particle3->velocity[0] = 1; particle3->velocity[1] = 1;
    particle3->sphere = sphere3;
    particle3->mass = 2;
    particle3->localTime = 0;
    
    sphere4.center = new double[2]; sphere4.center[0] = 61; sphere4.center[1] = 10; sphere4.radius = 15;
    particle4->velocity = new double[2]; particle4->velocity[0] = 1; particle4->velocity[1] = 1;
    particle4->sphere = sphere4;
    particle4->mass = 2;
    particle4->localTime = 0;
    
    _particle1 = new AKVisualizedParticle(*particle1);
    _particle2 = new AKVisualizedParticle(*particle2);
    _particle3 = new AKVisualizedParticle(*particle3);
    _particle4 = new AKVisualizedParticle(*particle4);
    AKParticlesList *particleList = new AKParticlesList();
    //particleList->push_back(particle1);
    particleList->push_back(particle2);
    //particleList->push_back(particle3);
    //particleList->push_back(particle4);
    _collisionDetectionLogic->setParticlesList(particleList);
    _collisionDetectionLogic->fillEventsInQueue();
}

#pragma mark - OpenGLInit delegate

void AKOpenGLHandheld::redrawObjects()
{
    _particle1->draw();
    _particle2->draw();
    _particle3->draw();
    _particle4->draw();
}

void AKOpenGLHandheld::handleTimerChanges(double time)
{
    _collisionDetectionLogic->updateEventQueueInTime(time);
}