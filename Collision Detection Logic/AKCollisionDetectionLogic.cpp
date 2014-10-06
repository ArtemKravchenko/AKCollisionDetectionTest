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
#include <limits>
#include "AKException.h"

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
    _eventsList = new AKEventsList();
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
    const unsigned int      *neighborsIndexes;
    const AKBox             *box;
    for (int i = 0; i < cellsCount; i++) {
        cell = _cellList->at(i);
        particlesList = cell->insideParticles;
        for (int j1 = 0; j1 < particlesCount; j1++) {
            // 1. Compute events with particles inside current cell
            firstParticle = particlesList->at(j1);
            addEventsForParticleAndParticlesInCurrentCell(firstParticle, cell, j1);
            // 2. Compute events with particles inside current cell and bound of cell
            box = cell->bounds;
            event = getEvent(firstParticle, box);
            _eventsList->push_back(event);
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
    AKRectangle const *rect = _bounds->rectangle;
    VectorXd const *center = rect->center;
    _cellWidth = (2 * abs((*center)[0])) / _cellsCountInCol;
    _cellHeight = (2 * abs((*center)[1])) / _cellsCountInRow;
    if (range > 0) {
        _cellDepth = (2 * abs((*center)[2])) / _cellsCountInRange;
    }
    _isBoundsAlreadySet = true;
}
/* -------------------  PRIVATE FUNCTIONS ------------------- */
inline AKEvent* AKCollisionDetectionLogic::nextEventFromListEvents()
{
    // TODO: Need to implement
    return nullptr;
}
inline int AKCollisionDetectionLogic::indexOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = ((VectorXd)*particle->sphere->center)[0], pointY = ((VectorXd)*particle->sphere->center)[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    if (particle->is2Ddimension) {
        returnIndex = indexY * _cellsCountInRow + indexX;
    } else {
        double pointZ = ((VectorXd)*particle->sphere->center)[2];
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
            topRightNeighbor = topRightNeighbor + 1;
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
    AKEvent *event = new AKEvent();
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = const_cast<AKParticle*>(secondParticle);
    event->eventType = AKEventParticleToParticleType;
    event->measure = -1;
    return event;
}
inline AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKBox const * box)
{
    double timeToEvent = std::numeric_limits<double>::max(), measure = 0.0, tmpTimeToEvent, tmpMeasure;
    AKRectangle const* rect = box->rectangle;
    VectorXd const *boxCenter = rect->center;
    VectorXd const *boxRadius = rect->radius;
    VectorXd const *systemBoundsCenter = _bounds->rectangle->center;
    VectorXd const *systemBoundsRadius = _bounds->rectangle->radius;
    int count = (!rect->is2Ddimension) ? 3 : 2;
    double systemMeasure;
    bool isSystemBound;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < count; i++) {
        tmpMeasure = (*boxCenter)[i] + (*boxRadius)[i];
        systemMeasure = (*systemBoundsCenter)[i] + (*systemBoundsRadius)[i];
        isSystemBound = (tmpMeasure == systemMeasure);
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i], isSystemBound, true);
        if (tmpTimeToEvent > 0 && tmpTimeToEvent < timeToEvent) {
            timeToEvent = tmpTimeToEvent;
            measure = tmpMeasure;
        }
        tmpMeasure = (*boxCenter)[i] - (*boxRadius)[i];
        systemMeasure = (*systemBoundsCenter)[i] - (*systemBoundsRadius)[i];
        isSystemBound = (tmpMeasure == systemMeasure);
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i], isSystemBound, false);
        if (tmpTimeToEvent > 0 && tmpTimeToEvent < timeToEvent) {
            timeToEvent = tmpTimeToEvent;
            measure = tmpMeasure;
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
            _eventsList->push_back(event);
        }
    }
}
inline void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const unsigned int *neighborsIndexes)
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
            _eventsList->push_back(event);
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
    double measure = _nextEvent->measure;
    AKPhysicsUtils::getInstance().changeParticleVelocityAfterCollisionWithBound(particle, measure);
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
    const unsigned int *neighborsIndexes;
    
    cellIndex = indexOfCellForParticle(particle);
    cell = _cellList->at(cellIndex);
    addEventsForParticleAndParticlesInCurrentCell(particle, cell, 0);
    box = cell->bounds;
    event = getEvent(particle, box);
    _eventsList->push_back(event);
    neighborsIndexes = cell->neighbors;
    addEventsForParticleAndParticlesInNeighborCells(particle, neighborsIndexes);
}
inline void AKCollisionDetectionLogic::removeEventsForParticle(AKParticle* particle)
{
    int index = 0;
    AKEvent *event = _eventsList->at(index);
    while (true) {
        if (isEventContainsParticle(event, particle)) {
            _eventsList->erase(_eventsList->begin() + index);
        } else {
            if (++index == _eventsList->size()) {
                return;
            }
        }
    }
}
inline bool AKCollisionDetectionLogic::isEventContainsParticle(AKEvent *event, AKParticle* particle)
{
    return (event->firstParticle == particle || event->secondParticle == particle);
}

