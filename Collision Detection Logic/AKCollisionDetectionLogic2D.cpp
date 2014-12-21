//
//  AKCollisionDetectionLogic2D.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionLogic2D.h"
#include <assert.h>

#pragma mark - /* ---------- PROTECTED ---------- */
#pragma mark - Set Bounds
void AKCollisionDetectionLogic2D::setSystemBounds(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);
}
int AKCollisionDetectionLogic2D::countOfCell()
{
    return _cellsCountInCol * _cellsCountInRow;
}
void AKCollisionDetectionLogic2D::fillCell(AKCell& cell, int index)
{
    cell.bounds.center[0] = _cellWidth * (index % _cellsCountInRow) + _cellWidth / 2;
    cell.bounds.center[1] = _cellHeight * (index / _cellsCountInRow) + _cellHeight / 2;
    cell.bounds.radius[0] = _cellWidth / 2;
    cell.bounds.radius[1] = _cellHeight / 2;
}
void AKCollisionDetectionLogic2D::fillNeighborsForCell(AKCell *cell,int index)
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
#pragma mark - Fill event in queue
inline int AKCollisionDetectionLogic2D::dimension()
{
    return 2;
}
inline void AKCollisionDetectionLogic2D::setMeasure(int* measure, int* nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle)
{
    if (sign == 1) {
        if (compareType == AKCollisionCompareXType) {
            measure[0] = 1; measure[1] = 1; *nextIndexCell = particle->cellIndex + 1;
        } else if(compareType == AKCollisionCompareYType) {
            measure[0] = 1; measure[1] =  0; *nextIndexCell = particle->cellIndex + _cellsCountInRow;
        }
    } else {
        if (compareType == AKCollisionCompareXType) {
            measure[0] = 0; measure[1] = 0; *nextIndexCell = particle->cellIndex - 1;
        } else if (compareType == AKCollisionCompareYType) {
            measure[0] = 0; measure[1] = 1; *nextIndexCell = particle->cellIndex - _cellsCountInRow;
        }
    }
}
#pragma mark - /* ---------- PUBLIC ---------- */
inline int AKCollisionDetectionLogic2D::indexOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = (particle->sphere.center)[0], pointY = (particle->sphere.center)[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    returnIndex = indexY * _cellsCountInRow + indexX;
    assert (returnIndex < _cellList->size() && returnIndex >= 0);
    return returnIndex;
}
void AKCollisionDetectionLogic2D::updateEventQueueInTime(double time)
{
    AKCollisionDetectionLogic::updateEventQueueInTime(time);
    assert(_nextEvent->firstParticle->sphere.center[0] + _nextEvent->firstParticle->sphere.radius < DISPLAY_WIDTH);
    assert(_nextEvent->firstParticle->sphere.center[0] - _nextEvent->firstParticle->sphere.radius >= -1);
    assert(_nextEvent->firstParticle->sphere.center[1] + _nextEvent->firstParticle->sphere.radius < DISPLAY_HEIGHT);
    assert(_nextEvent->firstParticle->sphere.center[1] - _nextEvent->firstParticle->sphere.radius >= -1);
}
void AKCollisionDetectionLogic2D::setBound(AKBox* bounds, float originX, float originY, float originZ, unsigned int row, unsigned int col, unsigned int range)
{
    _originX = originX;
    _originY = originY;
    setSystemBounds(bounds);
    _cellsCountInCol = col;
    _cellsCountInRow = row;
    _cellWidth = (abs(_bounds->center[0]) + _bounds->radius[0]) / _cellsCountInCol;
    _cellHeight = (abs(_bounds->center[1]) + _bounds->radius[1]) / _cellsCountInRow;
    _isBoundsAlreadySet = true;
    // Init cells list
    unsigned int cellsCount = countOfCell();
    _cellList = new AKCellsList();
    for (int i = 0; i < cellsCount; i++) {
        AKCell cell = AKCell((_bounds->is2dDimension) ? 2 : 3);
        cell.index = i;
        fillCell(cell, i);
        fillNeighborsForCell(&cell, i);
        _cellList->push_back(cell);
    }
}
void AKCollisionDetectionLogic2D::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        AKShapeVisualizer::getInstance().visualizeCell(cell, _originX, _originY, 0);
    }
}