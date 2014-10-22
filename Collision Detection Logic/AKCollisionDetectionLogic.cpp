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
#include "AKGeometricUtils.h"
#include "AKException.h"
#include "AKDefines.h"
#include <cmath>

// Singleton

/* -------------------  PUBLIC FUNCTIONS ------------------- */
void AKCollisionDetectionLogic::fillEventsInQueue()
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    /* -------------- PROCESS OF FILLING CELL BY PARTICLES -------------- */
    unsigned int                     particlesCount = _particleList->size() & INT_MAX;
    unsigned int                     cellIndex;
    AKCell                  *cell;
    _eventsQueue = new AKPriorityQueue();
    _cellList = new AKCellsList();
    _timeTotal = 0;
    _error = 0.000000001;
    for (int i = 0; i < particlesCount; i++) {
        AKParticle *particle = _particleList->at(i);
        cellIndex = indexOfCellForParticle(particle);
        cell = _cellList->at(cellIndex);
        cell->addParticle(particle);
        fillNeighborsForCell(cell, cellIndex);
    }
    /* --------------- PROCESS OF FILLING PRIORITY QUEUE --------------- */
    unsigned int                     cellsCount = _cellList->size() & INT_MAX;
    AKEvent                 *event;
    AKParticlesList const   *particlesList;
    AKParticle              *firstParticle;
    const int               *neighborsIndexes;
    const AKBox             *box;
    for (int i = 0; i < cellsCount; i++) {
        cell = _cellList->at(i);
        particlesList = cell->insideParticles;
        for (int j1 = 0; j1 < particlesCount; j1++) {
            // 1. Compute events with particles inside current cell
            firstParticle = particlesList->at(j1);
            addEventsForParticleAndParticlesInCurrentCell(firstParticle, cell, j1);
            // 2. Compute events with particles inside current cell and bound of cell
            box = &cell->bounds;
            event = getEvent(firstParticle, box);
            _eventsQueue->insert(event);
            // 3. Compute events with particles inside current cell and neighbor cell
            neighborsIndexes = cell->neighbors;
            addEventsForParticleAndParticlesInNeighborCells(firstParticle, neighborsIndexes);
        }
    }
    _timeTotal = 0;
    _timeToEvent = _nextEvent->timeToEvent;
}
void AKCollisionDetectionLogic::updateEventQueueInTime(int time)
{
    if (!_isBoundsAlreadySet || !_isParticlesAlreadySet)
        return;
    _timeTotal += time;
    updateParticlesLocation();
    if (abs(_timeToEvent) < _error) {
        AKEventType eventType = _nextEvent->eventType;
        switch (eventType) {
            case AKEventParticleToBoundType:
                handleParticleToBoundCollisionEvent();
                break;
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
        _nextEvent = nextEventFromListEvents();
    }
}
void AKCollisionDetectionLogic::setParticlesList(AKParticlesList *particlsList)
{
    _particleList = new AKParticlesList(*particlsList);
    _isParticlesAlreadySet = true;
}
void AKCollisionDetectionLogic::setBound(AKBox* bounds, unsigned int row, unsigned int col, unsigned int range)
{
    _bounds = new AKBox(*bounds);
    _cellsCountInCol = col;
    _cellsCountInRow = row;
    _cellsCountInRange = range;
    AKRectangle const *rect = &_bounds->rectangle;
    _cellWidth = (2 * abs(rect->center[0])) / _cellsCountInCol;
    _cellHeight = (2 * abs(rect->center[1])) / _cellsCountInRow;
    if (range > 0) {
        _cellDepth = (2 * abs(rect->center[2])) / _cellsCountInRange;
    }
    _isBoundsAlreadySet = true;
}
/* -------------------  PRIVATE FUNCTIONS ------------------- */
inline AKEvent* AKCollisionDetectionLogic::nextEventFromListEvents()
{
    return _eventsQueue->delMin();
}
inline int AKCollisionDetectionLogic::indexOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = (particle->sphere.center)[0], pointY = (particle->sphere.center)[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    if (particle->is2Ddimension) {
        returnIndex = indexY * _cellsCountInRow + indexX;
    } else {
        double pointZ = (particle->sphere.center)[2];
        int indexZ = static_cast<int>(pointZ / _cellDepth);
        returnIndex = indexZ * _cellsCountInCol * _cellsCountInRow + indexY * _cellsCountInRow + indexX;
    }
    return returnIndex;
}
void AKCollisionDetectionLogic::fillNeighborsForCell(AKCell *cell, int index)
{
    /*
        Way in which we will move to detect collision with neighborhood partices
     
           |   /
         __|__/
        |     |
        |     |---
        |_____|
               \
                \
     */
    int rightNeightbor = -1, topNeighbor = -1, bottomRightNeighbor = -1, topRightNeighbor = -1;
    if (index < (_cellsCountInCol - 1) * _cellsCountInRow) {
        topNeighbor = index + _cellsCountInRow;
        if ((index+1) % _cellsCountInRow != 0) {
            topRightNeighbor = topNeighbor + 1;
        }
    }
    if ((index+1) % _cellsCountInRow != 0) {
        rightNeightbor = index + 1;
    }
    if (index >= _cellsCountInRow && (index+1) % _cellsCountInRow != 0) {
        bottomRightNeighbor = index - _cellsCountInRow + 1;
    }
    int array[4] = {topNeighbor, topRightNeighbor, rightNeightbor, bottomRightNeighbor};
    cell->addNeighbors(array);
}
inline AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKParticle const * secondParticle)
{
    double timeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenTwoParticles(firstParticle, secondParticle);
    DIMENSION_FROM_BOOL(firstParticle->is2Ddimension, count)
    AKEvent *event = new AKEvent();
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = const_cast<AKParticle*>(secondParticle);
    event->eventType = AKEventParticleToParticleType;
    event->measure = new int[count]; event->measure[0] = -1; event->measure[1] = -1;
    return event;
}
inline AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKBox const * box)
{
    double timeToEvent = std::numeric_limits<double>::max(), tmpTimeToEvent, tmpMeasure;
    AKRectangle const* rect = &box->rectangle;
    DIMENSION_FROM_BOOL(rect->is2Ddimension, count)
    int *measure = new int[count];
    double systemMeasure;
    bool isSystemBound;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < count; i++) {
        tmpMeasure = rect->center[i] + rect->radius[i];
        systemMeasure = _bounds->rectangle.center[i] + _bounds->rectangle.radius[i];
        isSystemBound = (tmpMeasure == systemMeasure);
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i], isSystemBound, true);
        if (tmpTimeToEvent > 0 && tmpTimeToEvent < timeToEvent) {
            timeToEvent = tmpTimeToEvent;
            if (array[i] == AKCollisionCompareXType) {
                measure[0] = 0; measure[1] = 1;
            } else if(array[i] == AKCollisionCompareYType) {
                measure[0] = 1; measure[1] =  1;
            } else {
                // TODO: Need to implement for 3D case
            }
        }
        tmpMeasure = rect->center[i] - rect->radius[i];
        systemMeasure = _bounds->rectangle.center[i] - _bounds->rectangle.radius[i];
        isSystemBound = (tmpMeasure == systemMeasure);
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i], isSystemBound, false);
        if (tmpTimeToEvent > 0 && tmpTimeToEvent < timeToEvent) {
            timeToEvent = tmpTimeToEvent;
            if (array[i] == AKCollisionCompareXType) {
                measure[0] = 0; measure[1] = 0;
            } else if (array[i] == AKCollisionCompareYType) {
                measure[0] = 1; measure[1] = 0;
            } else {
                // TODO: Need to implement for 3D case
            }
        }
    }
    AKEvent *event = new AKEvent();
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = nullptr;
    event->measure = measure;
    return event;
}
inline void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex)
{
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList = cell->insideParticles;
    AKEvent                 *event;
    int                     particlesCount = particlesList->size() & INT_MAX;
    for (int i = startIndex; i < particlesCount; i++) {
        secondParticle = particlesList->at(i);
        if (particle != secondParticle) {
            event = getEvent(particle, secondParticle);
            _eventsQueue->insert(event);
        }
    }
}
inline void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes)
{
    const AKCell            *neigbor;
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList;
    int                     particlesCount;
    AKEvent                 *event;
    for (int i = 0; i < 4; i++) {
        neigbor = _cellList->at(neighborsIndexes[i]);
        particlesList = neigbor->insideParticles;
        particlesCount = particlesList->size() & INT_MAX;
        for (int j = 0; j < particlesCount; j++) {
            secondParticle = particlesList->at(j);
            event = getEvent(particle, secondParticle);
            _eventsQueue->insert(event);
        }
    }
}
inline void AKCollisionDetectionLogic::updateParticlesLocation()
{
    unsigned int particlesCount = _particleList->size() & INT_MAX;
    AKParticle *currentParticle;
    for (unsigned int i = 0; i < particlesCount; i++) {
        currentParticle = _particleList->at(i);
    }
	// TODO: Need to implement
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
    // TODO: don't need to remove events in case when
    //       just need to change time for events in which particles take participation
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
     1. Remove particle from cell
     2. Remove all events related to particle
     3. Add new events related to particles
     */
    // 1
    AKParticle *particle = _nextEvent->firstParticle;
    unsigned int cellIndex = indexOfCellForParticle(particle);
    AKCell *cell = _cellList->at(cellIndex);
    AKParticlesList *insideParticles = cell->insideParticles;
    insideParticles->erase(std::remove(insideParticles->begin(), insideParticles->end(), particle), insideParticles->end());
    // 2
    removeEventsForParticle(particle);
    // 3
    addEventsRelatedToParticle(particle);
}
inline void AKCollisionDetectionLogic::addEventsRelatedToParticle(AKParticle* particle)
{
    // 3.1 Compute events with particles inside current cell
    // 3.2 Compute events with particles inside current cell and bound of cell
    // 3.3 Compute events with particles inside current cell and neighbor cell
    unsigned int cellIndex;
    AKCell *cell;
    AKBox *box;
    AKEvent *event;
    const int *neighborsIndexes;
    
    cellIndex = indexOfCellForParticle(particle);
    cell = _cellList->at(cellIndex);
    addEventsForParticleAndParticlesInCurrentCell(particle, cell, 0);
    box = &cell->bounds;
    event = getEvent(particle, box);
    _eventsQueue->insert(event);
    neighborsIndexes = cell->neighbors;
    addEventsForParticleAndParticlesInNeighborCells(particle, neighborsIndexes);
}
inline void AKCollisionDetectionLogic::removeEventsForParticle(AKParticle* particle) // O(n)
{
    _eventsQueue->deleteElementForParticle(particle);
}
inline bool AKCollisionDetectionLogic::isEventContainsParticle(AKEvent *event, AKParticle* particle)
{
    return (event->firstParticle == particle || event->secondParticle == particle);
}

