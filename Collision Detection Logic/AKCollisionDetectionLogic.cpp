//
//  AKCollisionDetectionLogic.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "AKCollisionDetectionLogic.h"
#include "AKPhysicsUtils.h"
#include "AKGeometricUtils.h"
#include "AKException.h"
#include "AKDefines.h"
#include <cmath>
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
            neighborsIndexes = cell->neighbors;
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
    assert(_nextEvent->firstParticle->sphere.center[0] + _nextEvent->firstParticle->sphere.radius < DISPLAY_WIDTH);
    assert(_nextEvent->firstParticle->sphere.center[0] - _nextEvent->firstParticle->sphere.radius >= -1);
    assert(_nextEvent->firstParticle->sphere.center[1] + _nextEvent->firstParticle->sphere.radius < DISPLAY_HEIGHT);
    assert(_nextEvent->firstParticle->sphere.center[1] - _nextEvent->firstParticle->sphere.radius >= -1);
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
void AKCollisionDetectionLogic::setBound(AKBox* bounds, unsigned int row, unsigned int col, unsigned int range)
{
    _bounds = new AKBox(*bounds);
    _cellsCountInCol = col;
    _cellsCountInRow = row;
    _cellsCountInRange = range;
    _cellWidth = (2 * abs(_bounds->center[0])) / _cellsCountInCol;
    _cellHeight = (2 * abs(_bounds->center[1])) / _cellsCountInRow;
    if (range > 0) {
        _cellDepth = (2 * abs(_bounds->center[2])) / _cellsCountInRange;
    }
    _isBoundsAlreadySet = true;
    // Init cells list
    unsigned int cellsCount = _cellsCountInCol * _cellsCountInRow;
    if (range != 0) {
        cellsCount *= _cellsCountInRange;
    }
    _cellList = new AKCellsList();
    for (int i = 0; i < cellsCount; i++) {
        AKCell cell = AKCell(2);
        cell.index = i;
        // Compute cell bounds
        cell.bounds.center[0] = _cellWidth * (i % _cellsCountInRow) + _cellWidth / 2;
        cell.bounds.center[1] = _cellHeight * (i / _cellsCountInCol) + _cellHeight / 2;
        cell.bounds.radius[0] = _cellWidth / 2;
        cell.bounds.radius[1] = _cellHeight / 2;
        if (range != 0) {
            cell.bounds.radius[2] = _cellDepth / 2;
        }
        //
        fillNeighborsForCell(&cell, i);
        _cellList->push_back(cell);
    }
}
void AKCollisionDetectionLogic::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        cell->draw();
    }
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
inline int AKCollisionDetectionLogic::indexOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = (particle->sphere.center)[0], pointY = (particle->sphere.center)[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    if (particle->sphere.is2dDimension) {
        returnIndex = indexY * _cellsCountInRow + indexX;
    } else {
        double pointZ = (particle->sphere.center)[2];
        int indexZ = static_cast<int>(pointZ / _cellDepth);
        returnIndex = indexZ * _cellsCountInCol * _cellsCountInRow + indexY * _cellsCountInRow + indexX;
    }
    assert (returnIndex < _cellList->size() && returnIndex >= 0);
    return returnIndex;
}
void AKCollisionDetectionLogic::fillNeighborsForCell(AKCell *cell, int index)
{
    /*
        Way in which we will move to detect collision with neighborhood partices
     
      \    |   /
       \ __|__/
        |     |
     ---|     |---
        |_____|
       /   |   \
      /    |    \
     */

    int leftNeighbor = -1, leftTopNeighbor = -1, topNeighbor = -1, topRightNeighbor = -1, rightNeightbor = -1, bottomRightNeighbor = -1, bottomNeighbor = -1, leftBottomNeighbor = -1;
    
    if (index % _cellsCountInRow != 0) {
        leftNeighbor = index - 1;
    }
    if (index < (_cellsCountInCol - 1) * _cellsCountInRow) {
        topNeighbor = index + _cellsCountInRow;
    }
    if ((index+1) % _cellsCountInRow != 0) {
        rightNeightbor = index + 1;
    }
    if (index >= _cellsCountInRow) {
        bottomNeighbor = index - _cellsCountInRow;
    }
    if (leftNeighbor != - 1 && topNeighbor != - 1) {
        leftTopNeighbor = topNeighbor - 1;
    }
    if (topNeighbor != -1 && rightNeightbor != - 1) {
        topRightNeighbor = topNeighbor + 1;
    }
    if (bottomNeighbor != -1 && rightNeightbor != -1) {
        bottomRightNeighbor = bottomNeighbor + 1;
    }
    if (bottomNeighbor != -1 && leftNeighbor != -1) {
        leftBottomNeighbor = bottomNeighbor - 1;
    }
    int array[8] = { leftNeighbor, leftTopNeighbor, topNeighbor, topRightNeighbor, rightNeightbor, bottomRightNeighbor, bottomNeighbor, leftBottomNeighbor };
    std::cout << index << " :";
    for (int i = 0; i < 8; i++) {
        std::cout << array[i] << ",";
    }
    std:cout << std::endl;
    cell->addNeighbors(array);
}
inline AKEvent* AKCollisionDetectionLogic::insertEventInQueue(AKParticle const * firstParticle, AKParticle const * secondParticle)
{
    double timeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenTwoParticles(firstParticle, secondParticle);
    if (isnan(timeToEvent) || timeToEvent - firstParticle->localTime < 0) {
        return nullptr;
    }
    AKEvent *event = new AKEvent(2);
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = const_cast<AKParticle*>(secondParticle);
    event->eventType = AKEventParticleToParticleType;
    event->nextCellIndex = -1;
    _eventsQueue->insert(event);
    return event;
}
inline AKEvent* AKCollisionDetectionLogic::insertEventInQueue(AKParticle const * firstParticle, AKBox const * box)
{
    // TODO: After some period of time, smallest time to event has a wrong value, therefore all particles start to left bounds
    double timeToEvent = std::numeric_limits<double>::max(), tmpTimeToEvent, tmpMeasure;
    int measure[2];
    double systemMeasure;
    bool tmpIsSystemBound, isSystemBound = false;
    int nextCellIndex = -1;
    int kof = 0;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < 2; i++) { // loop through coordinates
        for (int j = 0; j < 2; j++) { // loop through sign for special coordinates
            kof = (j == 0) ? 1 : -1;
            tmpMeasure = box->center[i] + (box->radius[i] * kof);
            systemMeasure = _bounds->center[i] + (_bounds->radius[i] * kof);
            tmpIsSystemBound = (tmpMeasure == systemMeasure);
            tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i], tmpIsSystemBound, (j == 0));
            if (tmpTimeToEvent - firstParticle->localTime > 0 && tmpTimeToEvent < timeToEvent) {
                timeToEvent = tmpTimeToEvent;
                isSystemBound = tmpIsSystemBound;
                if (kof == 1) {
                    if (array[i] == AKCollisionCompareXType) {
                        measure[0] = 1; measure[1] = 1; nextCellIndex = firstParticle->cellIndex + 1;
                    } else if(array[i] == AKCollisionCompareYType) {
                        measure[0] = 1; measure[1] =  0; nextCellIndex = firstParticle->cellIndex + _cellsCountInRow;
                    } else {
                        // TODO: Need to implement for 3D case
                    }
                } else {
                    if (array[i] == AKCollisionCompareXType) {
                        measure[0] = 0; measure[1] = 0; nextCellIndex = firstParticle->cellIndex - 1;
                    } else if (array[i] == AKCollisionCompareYType) {
                        measure[0] = 0; measure[1] = 1; nextCellIndex = firstParticle->cellIndex - _cellsCountInRow;
                    } else {
                        // TODO: Need to implement for 3D case
                    }
                }
            }
        }
    }
    AKEvent *event = new AKEvent(2);
    event->timeToEvent = timeToEvent;
    event->firstParticle = const_cast<AKParticle*>(firstParticle);
    event->secondParticle = nullptr;
    if (isSystemBound) {
        event->eventType = AKEventParticleToBoundType;
        for (int i = 0; i < 2; i++) {
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
    neighborsIndexes = cell->neighbors;
    addEventsForParticleAndParticlesInNeighborCells(particle, neighborsIndexes);
}
inline void AKCollisionDetectionLogic::removeEventsForParticle(AKParticle* particle) // O(n)
{
    _eventsQueue->deleteElementForParticle(particle);
}
