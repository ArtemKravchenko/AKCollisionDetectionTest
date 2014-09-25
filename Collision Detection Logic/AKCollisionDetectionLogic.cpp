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

// Singleton

/* -------------------  PUBLIC FUNCTIONS ------------------- */
void AKCollisionDetectionLogic::fillEventsInQueue()
{
    /* -------------- PROCESS OF FILLING CELL BY PARTICLES -------------- */
    int                     particlesCount = _particleList->size() & INT_MAX;
    int                     cellIndex;
    AKCell                  *cell;
    for (int i = 0; i < particlesCount; i++) {
        AKParticle *particle = _particleList->at(i);
        cellIndex = numberOfCellForParticle(particle);
        cell = _cellList->at(cellIndex);
        cell->addParticle(particle);
        fillNeighborsForCell(cell, cellIndex);
    }
    /* --------------- PROCESS OF FILLING PRIORITY QUEUE --------------- */
    int                     cellsCount = _cellList->size() & INT_MAX;
    AKEvent                 *event;
    AKParticlesList const   *particlesList;
    AKParticle              *firstParticle;
    const int               *neighborsIndexes;
    const AKBox             *box;
    for (int i = 0; i < cellsCount; i++) {
        cell = _cellList->at(i);
        particlesList = cell->getInsideParticles();
        for (int j1 = 0; j1 < particlesCount; j1++) {
            // 1. Compute events with particles inside current cell
            firstParticle = particlesList->at(j1);
            addEventsForParticleAndParticlesInCurrentCell(firstParticle, cell, j1);
            // 2. Compute events with particles inside current cell and bound of cell
            box = cell->getBounds();
            event = getEvent(firstParticle, box);
            // 3. Compute events with particles inside current cell and neighbor cell
            neighborsIndexes = cell->getNeighbors();
            addEventsForParticleAndParticlesInNeighborCells(firstParticle, neighborsIndexes);
        }
    }
}
void AKCollisionDetectionLogic::updateEventQueueInTime(int time)
{
    // TODO: Need to implement
}
void AKCollisionDetectionLogic::setParticlesList(AKParticlesList *particlsList)
{
    _particleList = new AKParticlesList(*particlsList);
}
void AKCollisionDetectionLogic::setBound(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);
}
void AKCollisionDetectionLogic::setCellRowAndCol(int row, int col)
{
    _cellsCountInCol = col;
    _cellsCountInRow = row;
    // TODO: Need to implement
}
/* -------------------  PRIVATE FUNCTIONS ------------------- */
int AKCollisionDetectionLogic::numberOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = ((VectorXd)*particle->getSphere()->getCenter())[0], pointY = ((VectorXd)*particle->getSphere()->getCenter())[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    if (particle->is2Ddimension()) {
        returnIndex = indexY * _cellsCountInRow + indexX;
    } else {
        double pointZ = ((VectorXd)*particle->getSphere()->getCenter())[2];
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
AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKParticle const * secondParticle)
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
AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKBox const * box)
{
    // TODO: !!! Need to take acount that there are two separate types of events with cell's bound:
    // 1. Particle collided with system bound
    // 2. Particle left measure of cell
    double timeToEvent = std::numeric_limits<double>::max(), measure = 0.0, tmpTimeToEvent, tmpMeasure;
    AKRectangle const* rect = box->getRectangle();
    VectorXd const *boxCenter = rect->getCenter();
    VectorXd const *boxRadius = rect->getRadius();
    int count = (!rect->is2Ddimension()) ? 3 : 2;
    AKCollisionCompareType array[3] = {AKCollisionCompareXType, AKCollisionCompareYType, AKCollisionCompareZType};
    // Finding the lowest value of collision time with greater X, lesser X, greater Y, lesser Y, greater Z, lesser Z coordinate of box
    for (int i = 0; i < count; i++) {
        tmpMeasure = (*boxCenter)[i] + (*boxRadius)[i];
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i]);
        if (tmpTimeToEvent > 0 && tmpTimeToEvent < timeToEvent) {
            timeToEvent = tmpTimeToEvent;
            measure = tmpMeasure;
        }
        tmpMeasure = (*boxCenter)[i] - (*boxRadius)[i];
        tmpTimeToEvent = AKGeometricUtils::getInstance().getTimeToCollisionBetweenParticleAndBound(firstParticle, tmpMeasure, array[i]);
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
void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, int startIndex)
{
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList = cell->getInsideParticles();
    AKEvent                 *event;
    int                     particlesCount = particlesList->size() & INT_MAX;
    for (int i = startIndex; i < particlesCount; i++) {
        secondParticle = particlesList->at(i);
        event = getEvent(particle, secondParticle);
        _eventsQueue->push(event);
    }
}
void AKCollisionDetectionLogic::addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes)
{
    const AKCell            *neigbor;
    AKParticle              *secondParticle;
    AKParticlesList const   *particlesList;
    int                     particlesCount;
    AKEvent                 *event;
    for (int i = 0; i < 4; i++) {
        neigbor = _cellList->at(neighborsIndexes[i]);
        particlesList = neigbor->getInsideParticles();
        particlesCount = particlesList->size() & INT_MAX;
        for (int j = 0; j < particlesCount; j++) {
            secondParticle = particlesList->at(j);
            event = getEvent(particle, secondParticle);
            _eventsQueue->push(event);
        }
    }
}
