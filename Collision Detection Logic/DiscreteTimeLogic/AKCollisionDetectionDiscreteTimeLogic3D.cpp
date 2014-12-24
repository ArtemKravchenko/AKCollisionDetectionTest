//
//  AKCollisionDetectionDiscreteTimeLogic3D.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionDiscreteTimeLogic3D.h"
#include <assert.h>

#pragma mark - /* ---------- PROTECTED ---------- */
#pragma mark - Set Bounds
void AKCollisionDetectionDiscreteTimeLogic3D::setSystemBounds(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);
}
int AKCollisionDetectionDiscreteTimeLogic3D::countOfCell()
{
    return AKCollisionDetectionDiscreteTimeLogic2D::countOfCell() * _cellsCountInRange;
}
void AKCollisionDetectionDiscreteTimeLogic3D::fillCell(AKCell& cell, int index)
{
    int indexXY = (index + (_cellsCountInCol * _cellsCountInRow)) % (_cellsCountInCol * _cellsCountInRow);
    AKCollisionDetectionDiscreteTimeLogic2D::fillCell(cell, indexXY);
    cell.bounds.center[2] = _cellDepth * (index / (_cellsCountInCol * _cellsCountInRow)) + _cellDepth / 2;
    cell.bounds.radius[2] = _cellDepth / 2;
}
void AKCollisionDetectionDiscreteTimeLogic3D::fillNeighborsForCell(AKCell *cell,int index)
{
    AKCollisionDetectionDiscreteTimeLogic2D::fillNeighborsForCell(cell, index);
    int full2DSquare = _cellsCountInCol * _cellsCountInRow;
    int leftNeighborBack = -1, leftTopNeighborBack = -1, topNeighborBack = -1, topRightNeighborBack = -1, rightNeightborBack = -1, bottomRightNeighborBack = -1, bottomNeighborBack = -1, leftBottomNeighborBack = -1, middleNeighborBack = -1,
    leftNeighborFront = -1, leftTopNeighborFront = -1, topNeighborFront = -1, topRightNeighborFront = -1, rightNeightborFront = -1, bottomRightNeighborFront = -1, bottomNeighborFront = -1, leftBottomNeighborFront = -1, middleNeighBorFront = -1;
    if (index >= full2DSquare) {
        middleNeighborBack = index - full2DSquare;
        if (index % _cellsCountInRow != 0) {
            leftNeighborBack = index - 1 - full2DSquare;
        }
        if (index < (_cellsCountInCol - 1) * _cellsCountInRow) {
            topNeighborBack = index + _cellsCountInRow - full2DSquare;
        }
        if ((index+1) % _cellsCountInRow != 0) {
            rightNeightborBack = index + 1 - full2DSquare;
        }
        if (index >= _cellsCountInRow) {
            bottomNeighborBack = index - _cellsCountInRow - full2DSquare;
        }
        if (leftNeighborBack != - 1 && topNeighborBack != - 1) {
            leftTopNeighborBack = topNeighborBack - 1 - full2DSquare;
        }
        if (topNeighborBack != -1 && rightNeightborBack != - 1) {
            topRightNeighborBack = topNeighborBack + 1 - full2DSquare;
        }
        if (bottomNeighborBack != -1 && rightNeightborBack != -1) {
            bottomRightNeighborBack = bottomNeighborBack + 1 - full2DSquare;
        }
        if (bottomNeighborBack != -1 && leftNeighborBack != -1) {
            leftBottomNeighborBack = bottomNeighborBack - 1 - full2DSquare;
        }
    }
    if (index < full2DSquare * (_cellsCountInRange - 1)) {
        middleNeighBorFront = index + full2DSquare;
        if (index % _cellsCountInRow != 0) {
            leftNeighborFront = index - 1 + full2DSquare;
        }
        if (index < (_cellsCountInCol - 1) * _cellsCountInRow) {
            topNeighborFront = index + _cellsCountInRow + full2DSquare;
        }
        if ((index+1) % _cellsCountInRow != 0) {
            rightNeightborFront = index + 1 + full2DSquare;
        }
        if (index >= _cellsCountInRow) {
            bottomNeighborFront = index - _cellsCountInRow + full2DSquare;
        }
        if (leftNeighborFront != - 1 && topNeighborFront != - 1) {
            leftTopNeighborFront = topNeighborFront - 1 + full2DSquare;
        }
        if (topNeighborFront != -1 && rightNeightborFront != - 1) {
            topRightNeighborFront = topNeighborFront + 1 + full2DSquare;
        }
        if (bottomNeighborFront != -1 && rightNeightborFront != -1) {
            bottomRightNeighborFront = bottomNeighborFront + 1 + full2DSquare;
        }
        if (bottomNeighborFront != -1 && leftNeighborFront != -1) {
            leftBottomNeighborFront = bottomNeighborFront - 1 + full2DSquare;
        }
    }
    int array[18] = { leftNeighborBack, leftTopNeighborBack, topNeighborBack, topRightNeighborBack, rightNeightborBack, bottomRightNeighborBack, bottomNeighborBack, leftBottomNeighborBack, middleNeighborBack, leftNeighborFront, leftTopNeighborFront, topNeighborFront, topRightNeighborFront, rightNeightborFront, bottomRightNeighborFront, bottomNeighborFront, leftBottomNeighborFront, middleNeighBorFront };
    std::cout << index << " :";
    for (int i = 0; i < 18; i++) {
        std::cout << array[i] << ",";
        cell->addNeighborAtIndex(array[i]);
    }
    std:cout << std::endl;
}
#pragma mark - Fill event in queue
inline int AKCollisionDetectionDiscreteTimeLogic3D::dimension()
{
    return 3;
}
inline void AKCollisionDetectionDiscreteTimeLogic3D::setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle)
{
    AKCollisionDetectionDiscreteTimeLogic2D::setMeasure(measure, nextIndexCell, sign, compareType, particle);
    if (sign == 1) {
        if (compareType == AKCollisionCompareXType) {
            measure[2] = 0;
        } else if(compareType == AKCollisionCompareYType) {
            measure[2] = 0;
        } else {
            measure[0] = 0; measure[1] = 0; measure[2] = -1; *nextIndexCell = particle->cellIndex + _cellsCountInRow*_cellsCountInCol;
        }
    } else {
        if (compareType == AKCollisionCompareXType) {
            measure[2] = 0;
        } else if (compareType == AKCollisionCompareYType) {
            measure[2] = 0;
        } else {
            measure[0] = 0; measure[1] = 0; measure[2] = 1; *nextIndexCell = particle->cellIndex - _cellsCountInRow*_cellsCountInCol;
        }
    }
}
#pragma mark - /* ---------- PUBLIC ---------- */
inline int AKCollisionDetectionDiscreteTimeLogic3D::indexOfCellForParticle(AKParticle const * particle)
{
    int index2D = AKCollisionDetectionDiscreteTimeLogic2D::indexOfCellForParticle(particle);
    double pointZ = (particle->sphere.center)[2];
    int indexZ = pointZ / _cellDepth;
    int returnIndex = _cellsCountInCol * _cellsCountInRow * indexZ + index2D;
    assert (returnIndex < _cellList->size() && returnIndex >= 0);
    return returnIndex;
}
void AKCollisionDetectionDiscreteTimeLogic3D::updateEventQueueInTime(double time)
{
    AKCollisionDetectionDiscreteTimeLogic2D::updateEventQueueInTime(time);
    assert(_nextEvent->firstParticle->sphere.center[2] + _nextEvent->firstParticle->sphere.radius < DISPLAY_WIDTH);
    assert(_nextEvent->firstParticle->sphere.center[2] - _nextEvent->firstParticle->sphere.radius >= -1);
}
void AKCollisionDetectionDiscreteTimeLogic3D::setBound(AKBox* bounds, float originX, float originY, float originZ, unsigned int row, unsigned int col, unsigned int range)
{
    _originZ = originZ;
    _cellsCountInRange = range;
    _cellDepth = (abs(bounds->center[2]) + bounds->radius[2]) / _cellsCountInRange;
    _isBoundsAlreadySet = true;
    AKCollisionDetectionDiscreteTimeLogic2D::setBound(bounds, originX, originY, originZ, row, col, range);
}
void AKCollisionDetectionDiscreteTimeLogic3D::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        AKShapeVisualizer::getInstance().visualizeCell(cell, _originX, _originY, _originZ);
    }
}