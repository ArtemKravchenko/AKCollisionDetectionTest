//
//  AKCollisionDetectionLogic3D.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionLogic3D.h"
#include <assert.h>

#pragma mark - /* ---------- PROTECTED ---------- */
#pragma mark - Set Bounds
void AKCollisionDetectionLogic3D::setSystemBounds(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);
}
int AKCollisionDetectionLogic3D::countOfCell()
{
    return AKCollisionDetectionLogic2D::countOfCell() * _cellsCountInRange;
}
void AKCollisionDetectionLogic3D::fillCell(AKCell& cell, int index)
{
    AKCollisionDetectionLogic2D::fillCell(cell, index);
    cell.bounds.center[2] = _cellDepth * (index / _cellsCountInRange) + _cellDepth / 2;
    cell.bounds.radius[2] = _cellDepth / 2;
}
void AKCollisionDetectionLogic3D::fillNeighborsForCell(AKCell *cell,int index)
{
    // TODO: Need to implement
}
#pragma mark - Fill event in queue
inline int AKCollisionDetectionLogic3D::dimension()
{
    return 3;
}
inline void AKCollisionDetectionLogic3D::setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle)
{
    // TODO: Need to implement
}
#pragma mark - /* ---------- PUBLIC ---------- */
inline int AKCollisionDetectionLogic3D::indexOfCellForParticle(AKParticle const * particle)
{
    int index2D = AKCollisionDetectionLogic2D::indexOfCellForParticle(particle);
    double pointZ = (particle->sphere.center)[2];
    int indexZ = pointZ / _cellsCountInRange;
    int returnIndex = _cellsCountInCol * _cellsCountInRow * indexZ + index2D;
    return returnIndex;
}
void AKCollisionDetectionLogic3D::updateEventQueueInTime(double time)
{
    AKCollisionDetectionLogic2D::updateEventQueueInTime(time);
    assert(_nextEvent->firstParticle->sphere.center[2] + _nextEvent->firstParticle->sphere.radius < DISPLAY_WIDTH);
    assert(_nextEvent->firstParticle->sphere.center[2] - _nextEvent->firstParticle->sphere.radius >= -1);
}
void AKCollisionDetectionLogic3D::setBound(AKBox* bounds, float originX, float originY, float originZ, unsigned int row, unsigned int col, unsigned int range)
{
    _originZ = originZ;
    _cellsCountInRange = range;
    _cellDepth = (abs(_bounds->center[2]) + _bounds->radius[2]) / _cellsCountInRange;
    _isBoundsAlreadySet = true;
    AKCollisionDetectionLogic2D::setBound(bounds, originX, originY, originZ, row, col, range);
}
void AKCollisionDetectionLogic3D::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        AKShapeVisualizer::getInstance().visualizeCell(cell, _originX, _originY, _originZ);
    }
}