//
//  AKCollisionDetectionLogic.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKCollisionDetectionLogic.h"
#include "AKPhysicsUtils.h"
#include "AKException.h"
#include "AKDefines.h"
#include <assert.h>

// Singleton
using namespace std;

#pragma mark /* -------------------  PUBLIC FUNCTIONS ------------------- */
void AKCollisionDetectionLogic::fillEventsInQueue(unsigned int capacity)
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    /* -------------- PROCESS OF FILLING CELL BY PARTICLES -------------- */
    unsigned int                    particlesCount = _particleList->size() & INT_MAX;
    unsigned int                    cellIndex;
    AKCell                          *cell;
    _eventsQueue = new AKPriorityQueue(capacity);
    _timeTotal = 0;
    for (int i = 0; i < particlesCount; i++) {
        AKParticle *particle = _particleList->at(i);
        cellIndex = indexOfCellForParticle(particle);
        particle->cellIndex = cellIndex;
        cell = &(_cellList->at(cellIndex));
        cell->addParticle(particle);
    }
    /* --------------- PROCESS OF FILLING PRIORITY QUEUE --------------- */
    unsigned int            cellsCount = _cellList->size() & INT_MAX;
    AKEvent                 *event;
    AKParticlesList const   *particlesList;
    AKParticle              *firstParticle;
    const int               *neighborsIndexes;
    const AKBox             *box;
    for (int i = 0; i < cellsCount; i++) {
        cell = &(_cellList->at(i));
        particlesList = &cell->insideParticles;
        particlesCount = particlesList->size() & INT_MAX;
        for (int j1 = 0; j1 < particlesCount; j1++) {
            // 1. Compute events with particles inside current cell
            firstParticle = particlesList->at(j1);
            addEventsForParticleAndParticlesInCurrentCell(firstParticle, cell, j1 + 1);
            // 2. Compute events with particles inside current cell and bound of cell
            box = &cell->bounds;
            event = insertEventInQueue(firstParticle, box);
            // 3. Compute events with particles inside current cell and neighbor cell
            neighborsIndexes = &cell->neighbors[0];
            addEventsForParticleAndParticlesInNeighborCells(firstParticle, neighborsIndexes);
        }
    }
    nextEventFromListEvents();
    _timeTotal = 0;
}
void AKCollisionDetectionLogic::updateEventQueueInTime(double time)
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    _timeTotal += time;
    while (IS_EQUAL_WITH_ERROR(_timeTotal, _timeToEvent)) {
        AKEventType eventType = _nextEvent->eventType;
        _nextEvent->firstParticle->localTime = _timeTotal;
        if (_nextEvent->secondParticle != NULL) {
            _nextEvent->secondParticle->localTime = _timeTotal;
        }
        switch (eventType) {
            case AKEventParticleToBoundType:
            {
                handleParticleToBoundCollisionEvent();
                break;
            }
            case AKEventParticleToCellType:
                handleParticleLeftCellCollisionEvent();
                break;
            case AKEventParticleToParticleType:
                handleParticleToParticleCollisionEvent();
                break;
            default:
                throw new wrongEventTypeException();
                break;
        }
        nextEventFromListEvents();
    }
    updateParticlesLocation(time);
}
void AKCollisionDetectionLogic::setParticlesList(AKParticlesList *particlsList)
{
    _particleList = new AKParticlesList(*particlsList);
    _isParticlesAlreadySet = true;
}
#pragma mark /* -------------------  PRIVATE FUNCTIONS ------------------- */
inline void AKCollisionDetectionLogic::nextEventFromListEvents()
{
    if (_nextEvent != NULL) {
        delete _nextEvent;
        _nextEvent = nullptr;
    }
    _nextEvent = _eventsQueue->delMin();
    _timeToEvent = _nextEvent->timeToEvent;
}
inline AKEvent* AKCollisionDetectionLogic::insertEventInQueue(AKParticle const * firstParticle, AKParticle const * secondParticle)
{
    double timeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenTwoParticles(firstParticle, secondParticle);
    if (isnan(timeToEvent) || timeToEvent - firstParticle->localTime < 0) {
        return nullptr;
    }
    AKEvent *event = new AKEvent((firstParticle->sphere.is2dDimension) ? 2 : 3);
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = const_cast<AKParticle*>(secondParticle);
    event->eventType = AKEventParticleToParticleType;
    event->nextCellIndex = -1;
    _eventsQueue->insert(event);
    return event;
}
inline AKEvent* AKCollisionDetectionLogic::insertEventInQueue(AKParticle const * particle, AKBox const * box)
{
    double finalTimeToEvent = std::numeric_limits<double>::max(), tmpTimeToEvent, tmpMeasure;
    int dim = dimension();
    int measure[dim];
    double systemMeasure;
    bool tmpIsSystemBound, isSystemBound = false;
    int nextCellIndex = -1;
    int sign = 0;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < dim; i++) { // loop through coordinates
        for (int j = 0; j < 2; j++) { // loop through sign for special coordinates
            sign = (j == 0) ? 1 : -1;
            tmpMeasure = box->center[i] + (box->radius[i] * sign);
            systemMeasure = _bounds->center[i] + (_bounds->radius[i] * sign);
            tmpIsSystemBound = (tmpMeasure == systemMeasure);
            tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(particle, tmpMeasure, array[i], tmpIsSystemBound, (j == 0));
            if (tmpTimeToEvent - particle->localTime > 0 && tmpTimeToEvent < finalTimeToEvent) {
                finalTimeToEvent = tmpTimeToEvent;
                isSystemBound = tmpIsSystemBound;
                setMeasure(measure, &nextCellIndex, sign, array[i], particle);
            }
        }
    }
    AKEvent *event = new AKEvent(dim);
    event->timeToEvent = finalTimeToEvent;
    event->firstParticle = const_cast<AKParticle*>(particle);
    event->secondParticle = nullptr;
    if (isSystemBound) {
        event->eventType = AKEventParticleToBoundType;
        for (int i = 0; i < dim; i++) {
            event->measure[i] = measure[i];
        }
        event->nextCellIndex = -1;
    } else {
        event->eventType = AKEventParticleToCellType;
        event->nextCellIndex = nextCellIndex;
    }
    _eventsQueue->insert(event);
    return event;
}
inline void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex)
{
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList = &cell->insideParticles;
    AKEvent                 *event;
    int                     particlesCount = particlesList->size() & INT_MAX;
    for (int i = startIndex; i < particlesCount; i++) {
        secondParticle = particlesList->at(i);
        if (particle != secondParticle) {
            event = insertEventInQueue(particle, secondParticle);
        }
    }
}
inline void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes)
{
    int                     neighborIndex;
    const AKCell            *neigbor;
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList;
    int                     particlesCount;
    AKEvent                 *event;
    for (int i = 0; i < 8; i++) {
        neighborIndex = neighborsIndexes[i];
        if (neighborIndex == -1) {
            continue;
        }
        neigbor = &(_cellList->at(neighborsIndexes[i]));
        particlesList = &neigbor->insideParticles;
        particlesCount = particlesList->size() & INT_MAX;
        for (int j = 0; j < particlesCount; j++) {
            secondParticle = particlesList->at(j);
            event = insertEventInQueue(particle, secondParticle);
        }
    }
}
inline void AKCollisionDetectionLogic::updateParticlesLocation(double time)
{
    unsigned int particlesCount = _particleList->size() & INT_MAX;
    AKParticle *currentParticle;
    for (unsigned int i = 0; i < particlesCount; i++) {
        currentParticle = _particleList->at(i);
        AKPhysicsUtils::getInstance().changeParticlePlaceInTime(currentParticle, time);
    }
}
inline void AKCollisionDetectionLogic::handleParticleToParticleCollisionEvent()
{
    /*
     1. Change velocity of each of two  particles
     2. Remove all events related to each of two particles
     3. Add new events related to each of two particles
     */
    // 1
    AKParticle *p1 = _nextEvent->firstParticle;
    AKParticle *p2 = _nextEvent->secondParticle;
    AKPhysicsUtils::getInstance().changeParticleVelocityAfterCollisionWithAnotherParticle(p1, p2);
    // 2
    removeEventsForParticle(p1);
    removeEventsForParticle(p2);
    // 3
    addEventsRelatedToParticle(p1);
    addEventsRelatedToParticle(p2);
}
inline void AKCollisionDetectionLogic::handleParticleToBoundCollisionEvent()
{
    /*
     1. Change velocity of particle
     2. Remove all events related to particle
     3. Add new events related to particles
     */
    // 1
    AKParticle *particle = _nextEvent->firstParticle;
    AKPhysicsUtils::getInstance().changeParticleVelocityAfterCollisionWithBound(particle, _nextEvent->measure);
    // 2
    removeEventsForParticle(particle);
    // 3
    addEventsRelatedToParticle(particle);
}
inline void AKCollisionDetectionLogic::handleParticleLeftCellCollisionEvent()
{
    /*
     1. Remove particle from cell and add particle to new cell
     2. Remove all events related to particle
     3. Add new events related to particles
     */
    // 1
    AKParticle *particle = _nextEvent->firstParticle;
    unsigned int cellIndex = particle->cellIndex;
    AKCell *cell = &(_cellList->at(cellIndex));
    AKParticlesList *insideParticles = &cell->insideParticles;
    insideParticles->erase(std::remove(insideParticles->begin(), insideParticles->end(), particle), insideParticles->end());
    cellIndex = indexOfCellForParticle(particle);
    particle->cellIndex = cellIndex;
    cell = &(_cellList->at(cellIndex));
    cell->addParticle(particle);
    // 2
    removeEventsForParticle(particle);
    // 3
    addEventsRelatedToParticle(particle);
}
inline void AKCollisionDetectionLogic::addEventsRelatedToParticle(AKParticle* particle)
{
    // 3.1 Compute events with particles and other particles inside current cell
    // 3.2 Compute events with particles inside current cell and bound of cell
    // 3.3 Compute events with particles inside current cell and neighbor cell
    unsigned int cellIndex;
    AKCell *cell;
    AKBox *box;
    AKEvent *event;
    const int *neighborsIndexes;
    // 3.1
    cellIndex = particle->cellIndex;
    cell = &(_cellList->at(cellIndex));
    addEventsForParticleAndParticlesInCurrentCell(particle, cell, 0);
    // 3.2
    box = &cell->bounds;
    event = insertEventInQueue(particle, box);
    // 3.3
    neighborsIndexes = &cell->neighbors[0];
    addEventsForParticleAndParticlesInNeighborCells(particle, neighborsIndexes);
}
inline void AKCollisionDetectionLogic::removeEventsForParticle(AKParticle* particle) // O(n)
{
    _eventsQueue->deleteElementForParticle(particle);
}
