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
    AKParticle              *firstParticle, *secondParticle;
    const AKCell            *neigbor;
    const int               *neighborsIndexes;
    const AKBox             *box;
    for (int i = 0; i < cellsCount; i++) {
        cell = _cellList->at(i);
        particlesList = cell->getInsideParticles();
        particlesCount = particlesList->size() & INT_MAX;
        for (int j1 = 0; j1 < particlesCount; j1++) {
            // 1. Compute events with particles inside current cell
            firstParticle = particlesList->at(j1);
            for (int j2 = j1; j2 < particlesCount; j2++) {
                secondParticle = particlesList->at(j2);
                event = getEvent(firstParticle, secondParticle);
                _eventsQueue->push(event);
            }
            // 2. Compute events with particles inside current cell and bound of cell
            box = cell->getBounds();
            event = getEvent(firstParticle, box);
            // 3. Compute events with particles inside current cell and neighbor cell
            neighborsIndexes = cell->getNeighbors();
            for (int j3 = 0; j3 < 4; j3++) {
                neigbor = _cellList->at(neighborsIndexes[j3]);
                particlesList = neigbor->getInsideParticles();
                particlesCount = particlesList->size() & INT_MAX;
                for (int j31 = 0; j31 < particlesCount; j31++) {
                    secondParticle = particlesList->at(j31);
                    event = getEvent(firstParticle, secondParticle);
                    _eventsQueue->push(event);
                }
            }
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
        returnIndex = 0;
        // TODO: Need to implement
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
    int rightNeightbor;
    int topNeighbor;
    int bottomRightNeighbor;
    int topRightNeighbor;
    int array[4] = {topNeighbor, topRightNeighbor, rightNeightbor, bottomRightNeighbor};
    cell->addNeighbors(array);
}
AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKParticle const * secondParticle)
{
    return nullptr;
}
AKEvent* AKCollisionDetectionLogic::getEvent(AKParticle const * firstParticle, AKBox const * box)
{
    return nullptr;
}

