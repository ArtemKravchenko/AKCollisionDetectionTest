//
//  AKCollisionDetectionEventDrivenLogic.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/24/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionEventDrivenLogic.h"
#include "AKPhysicsUtils.h"
#include "AKDefines.h"
#include <assert.h>
#include <cfloat>

using namespace std;

#pragma mark /* -------------------  PUBLIC FUNCTIONS ------------------- */
void AKCollisionDetectionEventDrivenLogic::fillEventsInQueue(unsigned int capacity)
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    /* -------------- PROCESS OF FILLING CELL BY PARTICLES -------------- */
    unsigned int                    particlesCount = _particleList->size() & INT_MAX;
    unsigned int                    cellIndex;
    AKCell                          *cell;
    _eventsQueue = new AKPriorityQueue(capacity);
    _eventsQueue->delegate = this;
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
            insertEventInQueue(firstParticle, box);
            // 3. Compute events with particles inside current cell and neighbor cell
            neighborsIndexes = &cell->neighbors[0];
            int neighborsCount = countOfHalfNeighbors();
            addEventsForParticleAndParticlesInNeighborCells(firstParticle, neighborsIndexes, neighborsCount);
        }
    }
    nextEventFromListEvents();
    _timeTotal = 0;
}
void AKCollisionDetectionEventDrivenLogic::updateEventQueueInTime(double time)
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    updateParticlesLocation(_timeToEvent);
    _timeTotal = _timeToEvent;
    cout << _timeTotal << endl;
    AKEventType eventType = _nextEvent->eventType;
    _eventsQueue->delMin()->firstParticle->localTime = _timeTotal;
    if (_nextEvent->secondParticle != NULL) {
        _eventsQueue->delMin()->secondParticle->localTime = _timeTotal;
    }
    switch (eventType) {
        case AKEventParticleToParticleType:
            handleParticleToParticleCollisionEvent();
            break;
        case AKEventParticleToBoundType:
            handleParticleToBoundCollisionEvent();
            break;
        case AKEventParticleToCellType:
            handleParticleLeftCellCollisionEvent();
            break;
        default:
            throw new wrongEventTypeException();
            break;
    }
    nextEventFromListEvents();
}
void AKCollisionDetectionEventDrivenLogic::setParticlesList(AKParticlesList *particlsList)
{
    _particleList = new AKParticlesList(*particlsList);
    _isParticlesAlreadySet = true;
}
#pragma mark /* -------------------  PRIVATE FUNCTIONS ------------------- */
void AKCollisionDetectionEventDrivenLogic::setSystemBounds(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);
}
inline void AKCollisionDetectionEventDrivenLogic::nextEventFromListEvents()
{
    if (_nextEvent != NULL) {
        delete _nextEvent;
        _nextEvent = nullptr;
    }
    _nextEvent = new AKEvent(*_eventsQueue->delMin());
    _timeToEvent = _nextEvent->timeToEvent;
}
inline void AKCollisionDetectionEventDrivenLogic::prepeareEventForCollision(AKParticle const *firstParticle, AKParticle const *secondParticle, AKEvent*event)
{
    double timeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenTwoParticles(firstParticle, secondParticle);
    event->timeToEvent = (isnan(timeToEvent) || timeToEvent - max(firstParticle->localTime, secondParticle->localTime) < 0) ? DBL_MAX : timeToEvent;
}
inline void AKCollisionDetectionEventDrivenLogic::insertEventInQueue(AKParticle const * firstParticle, AKParticle const * secondParticle)
{
    AKEvent *event = new AKEvent(dimension());
    prepeareEventForCollision(firstParticle, secondParticle, event);
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = const_cast<AKParticle*>(secondParticle);
    event->eventType = AKEventParticleToParticleType;
    event->nextCellIndex = -1;
    _eventsQueue->insert(event);
}
inline void AKCollisionDetectionEventDrivenLogic::prepeareEventForCollision(AKParticle const * particle, AKBox const * box, AKEvent* event)
{
    double finalTimeToEvent = std::numeric_limits<double>::max(), tmpTimeToEvent, tmpMeasure;
    int dim = dimension();
    int *measure = new int[dim];
    double systemMeasure;
    bool tmpIsSystemBound, isSystemBound = false;
    int nextCellIndex = -1;
    int sign = 0;
    int tmpTransitionIndex = -1, transtionIndex = -1;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < dim; i++) { // loop through coordinates
        for (int j = 0; j < 2; j++) { // loop through sign for special coordinates
            sign = (j == 0) ? 1 : -1;
            tmpMeasure = box->center[i] + (box->radius[i] * sign);
            systemMeasure = _bounds->center[i] + (_bounds->radius[i] * sign);
            tmpIsSystemBound = IS_EQUAL_WITH_ERROR(tmpMeasure, systemMeasure);//(tmpMeasure == systemMeasure);
            tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(particle, tmpMeasure, array[i], tmpIsSystemBound, (j == 0));
            tmpTransitionIndex++;
            if (tmpTimeToEvent - particle->localTime > 0.00000000001 && tmpTimeToEvent < finalTimeToEvent) {
                finalTimeToEvent = tmpTimeToEvent;
                isSystemBound = tmpIsSystemBound;
                transtionIndex = tmpTransitionIndex;
                setMeasure(measure, &nextCellIndex, sign, array[i], particle);
            }
        }
    }
    event->timeToEvent = finalTimeToEvent;
    if (isSystemBound) {
        event->eventType = AKEventParticleToBoundType;
        for (int i = 0; i < dim; i++) {
            event->measure[i] = measure[i];
        }
        event->nextCellIndex = -1;
        event->transitionIndex = -1;
    } else {
        event->eventType = AKEventParticleToCellType;
        event->nextCellIndex = nextCellIndex;
        event->transitionIndex = transtionIndex;
    }
}
inline void AKCollisionDetectionEventDrivenLogic::insertEventInQueue(AKParticle const * particle, AKBox const * box)
{
    
    AKEvent *event = new AKEvent(dimension());
    event->firstParticle = const_cast<AKParticle*>(particle);
    event->secondParticle = nullptr;
    prepeareEventForCollision(particle, box, event);
    _eventsQueue->insert(event);
}
inline void AKCollisionDetectionEventDrivenLogic::addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex)
{
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList = &cell->insideParticles;
    int                     particlesCount = particlesList->size() & INT_MAX;
    for (int i = startIndex; i < particlesCount; i++) {
        secondParticle = particlesList->at(i);
        if (particle != secondParticle) {
            insertEventInQueue(particle, secondParticle);
        }
    }
}
inline void AKCollisionDetectionEventDrivenLogic::addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes, int neighborsCount)
{
    int                     neighborIndex;
    const AKCell            *neigbor;
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList;
    int                     particlesCount;
    for (int i = 0; i < neighborsCount; i++) {
        neighborIndex = neighborsIndexes[i];
        if (neighborIndex == -1) {
            continue;
        }
        neigbor = &(_cellList->at(neighborsIndexes[i]));
        particlesList = &neigbor->insideParticles;
        particlesCount = particlesList->size() & INT_MAX;
        for (int j = 0; j < particlesCount; j++) {
            secondParticle = particlesList->at(j);
            insertEventInQueue(particle, secondParticle);
        }
    }
}
inline void AKCollisionDetectionEventDrivenLogic::updateParticlesLocation(double time)
{
    unsigned int particlesCount = _particleList->size() & INT_MAX;
    AKParticle *currentParticle;
    double deltaS;
    for (unsigned int i = 0; i < particlesCount; i++) {
        currentParticle = _particleList->at(i);
        deltaS = time - _timeTotal;
        AKPhysicsUtils::getInstance().changeParticlePlaceInTime(currentParticle, deltaS);
    }
}
inline void AKCollisionDetectionEventDrivenLogic::handleParticleToParticleCollisionEvent()
{
    /*
     1. Change velocity of each of two  particles
     2. Recompute time to events for each of two particles
     */
    // 1
    AKParticle *p1 = _nextEvent->firstParticle;
    AKParticle *p2 = _nextEvent->secondParticle;
    AKPhysicsUtils::getInstance().changeParticleVelocityAfterCollisionWithAnotherParticle(p1, p2);
    // 2
    updateEventsForPartilce(p1);
    updateEventsForPartilce(p2);
}
inline void AKCollisionDetectionEventDrivenLogic::handleParticleToBoundCollisionEvent()
{
    /*
     1. Change velocity of particle
     2. Recompute time to events for cuurnt particle
     */
    // 1
    AKParticle *particle = _nextEvent->firstParticle;
    AKPhysicsUtils::getInstance().changeParticleVelocityAfterCollisionWithBound(particle, _nextEvent->measure);
    // 2
    updateEventsForPartilce(particle);
}
inline void AKCollisionDetectionEventDrivenLogic::handleParticleLeftCellCollisionEvent()
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
    if (cellIndex != _nextEvent->nextCellIndex) {
        //throw exception(); // Need to understand why its happens
    }
    particle->cellIndex = cellIndex;
    cell = &(_cellList->at(cellIndex));
    cell->addParticle(particle);
    // 2
    removeEventsForParticle(particle, _nextEvent->transitionIndex);
    // 3
    addEventsRelatedToParticle(particle);
}
inline void AKCollisionDetectionEventDrivenLogic::addEventsRelatedToParticle(AKParticle* particle)
{
    // 3.1 Compute events with particles and other particles inside current cell
    // 3.2 Compute events with particles inside current cell and bound of cell
    // 3.3 Compute events with particles inside current cell and new reachable neighbors cells
    unsigned int cellIndex;
    AKCell *cell;
    AKBox *box;
    const int *neighborsIndexes;
    
    // 3.1
    cellIndex = particle->cellIndex;
    cell = &(_cellList->at(cellIndex));
    //addEventsForParticleAndParticlesInCurrentCell(particle, cell, 0);
    // 3.2
    box = &cell->bounds;
    insertEventInQueue(particle, box);
    // 3.3
    if (_nextEvent->nextCellIndex != -1) {
        neighborsIndexes = _reachableNeighbors[_nextEvent->nextCellIndex];
        int neighborsCount = countUnreachableCells();
        int *reachableIndexes = new int[neighborsCount];
        for (int i = _nextEvent->transitionIndex*neighborsCount, j = 0; j < neighborsCount; j++, i++) {
            reachableIndexes[j] = neighborsIndexes[i];
        }
        if (neighborsIndexes != NULL) {
            addEventsForParticleAndParticlesInNeighborCells(particle, reachableIndexes, neighborsCount);
        }
    } else {
        throw exception();
    }
}
inline void AKCollisionDetectionEventDrivenLogic::removeEventsForParticle(AKParticle* particle, int transitionIndex) // O(n)
{
    // Need to correct get only 3 or 9 cell that are actually unreachable
    int *unreachableCells = _unrachableNeighbors[particle->cellIndex];
    _eventsQueue->deleteElementForParticle(particle, unreachableCells, transitionIndex);
}
inline void AKCollisionDetectionEventDrivenLogic::updateEventsForPartilce(AKParticle* particle)
{
    _eventsQueue->updateEventForParticle(particle);
}
#pragma mark - AKPriorityQueueDelegate
void AKCollisionDetectionEventDrivenLogic::recomputeEvent(AKEvent* event)
{
    if (event->secondParticle != NULL) {
        prepeareEventForCollision(event->firstParticle, event->secondParticle, event);
    } else {
        unsigned int cellIndex; AKCell *cell;
        cellIndex = event->firstParticle->cellIndex;
        cell = &(_cellList->at(cellIndex));
        prepeareEventForCollision(event->firstParticle, &cell->bounds, event);
    }
}