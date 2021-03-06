//
//  AKCollisionDetectionDiscreteTimeLogic2D.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionDiscreteTimeLogic2D.h"
#include <assert.h>

#pragma mark - /* ---------- PROTECTED ---------- */
#pragma mark - filling reachable neighbors
void AKCollisionDetectionDiscreteTimeLogic2D::fillReachableNeighbors()
{
    unsigned int countOfCells = _cellList->size() & INT_MAX;
    _reachableNeighbors = new int*[countOfCells];
    int *arrayPart;
    int indexOfArayPart = 0;
    int sign[2] = {1, -1}, koef[2] = {1, _cellsCountInRow};
    int index = -1;
    for (int i = 0; i < countOfCells; i++) {
        indexOfArayPart = 0;
        _reachableNeighbors[i] = new int[12];
        for (int j = 0; j < 12; j++) _reachableNeighbors[i][j] = - 1;
        for (int i2 = 0; i2 < 2; i2++) {
            for (int i1 = 0; i1 < 2; i1++) {
                if (!validIndexForSignAndKoef(i, (sign[i1] == 1) ? -1 : 1, koef[i2])) { indexOfArayPart+=3; continue; }
                index = i + sign[i1] * koef[i2];
                arrayPart = fillReachableNeighborsForCell(i, sign[i1], koef[i2], (i2 == 0) ? koef[1] : koef[0]);
                for (int i3 = 0;i3 < 3; i3++ ) {
                    if (arrayPart[i3] < -1) {
                        throw exception();
                    }
                    _reachableNeighbors[i][indexOfArayPart++] = arrayPart[i3];
                }
            }
        }
    }
}
int* AKCollisionDetectionDiscreteTimeLogic2D::fillReachableNeighborsForCell(int index, int sign, int koef, int reflectKoef)
{
    int *arrayPart = new int[3];
    
    if (validIndexForSignAndKoef(index, sign, koef)) {
        arrayPart[0] = index + koef*sign;
        if (validPossitiveNeighbors(index, sign, koef)) {
            arrayPart[1] = arrayPart[0] + reflectKoef;
        } else {
            arrayPart[1] = -1;
        }
        if (validNegativeNeighbors(index, sign, koef)) {
            arrayPart[2] = arrayPart[0] - reflectKoef;
        } else {
            arrayPart[2] = -1;
        }
    } else {
        arrayPart[0] = -1;
        arrayPart[1] = -1;
        arrayPart[2] = -1;
    }
    return arrayPart;
}
#pragma mark - filling unreachable neigbors
void AKCollisionDetectionDiscreteTimeLogic2D::fillUnreachableNeighbors()
{
    unsigned int countOfCells = _cellList->size() & INT_MAX;
    _unrachableNeighbors = new int*[countOfCells];
    int *arrayPart;
    int indexOfArayPart = 0;
    int sign[2] = {-1, 1}, koef[2] = {1, _cellsCountInRow};
    int index = -1;
    for (int i = 0; i < countOfCells; i++) {
        indexOfArayPart = 0;
        _unrachableNeighbors[i] = new int[12];
        for (int j = 0; j < 12; j++) _unrachableNeighbors[i][j] = - 1;
        for (int i2 = 0; i2 < 2; i2++) {
            for (int i1 = 0; i1 < 2; i1++) {
                if (!validIndexForSignAndKoef(i, sign[i1], koef[i2])) { indexOfArayPart+=3; continue; }
                index = i + sign[i1] * koef[i2];
                arrayPart = fillUnreachableNeighborsForCell(index, sign[i1], koef[i2], koef[(i2 == 0) ? 1 : 0]);
                for (int i3 = 0;i3 < 3; i3++ ) {
                    _unrachableNeighbors[i][indexOfArayPart++] = arrayPart[i3];
                }
            }
        }
    }
}
int* AKCollisionDetectionDiscreteTimeLogic2D::fillUnreachableNeighborsForCell(int index, int sign, int koef, int reflectKoef)
{
    int *arrayPart = new int[3];
    if (validIndexForSignAndKoef(index, sign, koef)) {
        arrayPart[0] = index + sign*koef;
        if (validPossitiveNeighbors(arrayPart[0], sign, koef)) {
            arrayPart[1] = arrayPart[0] + reflectKoef;
        } else {
            arrayPart[1] = -1;
        }
        if (validNegativeNeighbors(arrayPart[0], sign, koef)) {
            arrayPart[2] = arrayPart[0] - reflectKoef;
        } else {
            arrayPart[2] = -1;
        }
    } else {
        arrayPart[0] = -1;
        arrayPart[1] = -1;
        arrayPart[2] = -1;
    }
    return arrayPart;
}
bool AKCollisionDetectionDiscreteTimeLogic2D::validIndexForSignAndKoef(int index, int sign, int koef)
{
    if (index + sign*koef < 0 || index + sign*koef >= _cellsCountInRow * _cellsCountInCol) {
        return false;
    }
    if (sign == - 1) {
        if (koef == 1) {
            return (index % _cellsCountInRow != 0);
        } else {
            return (index >= _cellsCountInRow);
        }
    } else {
        if (koef == 1) {
            return ((index+1) % _cellsCountInRow != 0);
        } else {
            return (index < _cellsCountInRow*(_cellsCountInCol-1));
        }
    }
}
bool AKCollisionDetectionDiscreteTimeLogic2D::validPossitiveNeighbors(int index, int sign, int koef)
{
    if (koef == 1) {
        return (index < (_cellsCountInCol - 1)  * _cellsCountInRow);
    } else {
        return ((index + 1) % _cellsCountInRow != 0);
    }
}
bool AKCollisionDetectionDiscreteTimeLogic2D::validNegativeNeighbors(int index, int sign, int koef)
{
    if (koef == 1) {
        return (index - _cellsCountInRow >= 0);
    } else {
        return (index % _cellsCountInRow != 0);
    }
}
#pragma mark - Set Bounds
int AKCollisionDetectionDiscreteTimeLogic2D::countOfCell()
{
    return _cellsCountInCol * _cellsCountInRow;
}
void AKCollisionDetectionDiscreteTimeLogic2D::fillCell(AKCell& cell, int index)
{
    cell.bounds.center[0] = _cellWidth * (index % _cellsCountInRow) + _cellWidth / 2;
    cell.bounds.center[1] = _cellHeight * (index / _cellsCountInRow) + _cellHeight / 2;
    cell.bounds.radius[0] = _cellWidth / 2;
    cell.bounds.radius[1] = _cellHeight / 2;
}
void AKCollisionDetectionDiscreteTimeLogic2D::fillNeighborsForCell(AKCell *cell,int index)
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
    
    int topNeighbor = -1, topRightNeighbor = -1, rightNeightbor = -1, bottomRightNeighbor = -1;//, leftNeighbor = -1, leftTopNeighbor = -1, bottomNeighbor = -1, leftBottomNeighbor = -1;
    /*
    if (index % _cellsCountInRow != 0) {
        leftNeighbor = index - 1;
    }
     */
    if (index < (_cellsCountInCol - 1) * _cellsCountInRow) {
        topNeighbor = index + _cellsCountInRow;
    }
    if ((index+1) % _cellsCountInRow != 0) {
        rightNeightbor = index + 1;
    }
    /*
    if (index >= _cellsCountInRow) {
        bottomNeighbor = index - _cellsCountInRow;
    }
    if (leftNeighbor != - 1 && topNeighbor != - 1) {
        leftTopNeighbor = topNeighbor - 1;
    }
     */
    if (topNeighbor != -1 && rightNeightbor != - 1) {
        topRightNeighbor = topNeighbor + 1;
    }
    if (index >= _cellsCountInRow && rightNeightbor != -1) {
        bottomRightNeighbor = index - _cellsCountInRow + 1;
    }
    /*
    if (bottomNeighbor != -1 && leftNeighbor != -1) {
        leftBottomNeighbor = bottomNeighbor - 1;
    }
     */
    int array[4] = { bottomRightNeighbor, rightNeightbor, topRightNeighbor, topNeighbor };
    //int array[8] = { leftNeighbor, leftTopNeighbor, topNeighbor, topRightNeighbor, rightNeightbor, bottomRightNeighbor, bottomNeighbor, leftBottomNeighbor };
    std::cout << index << " :";
    for (int i = 0; i < 4; i++) {
        std::cout << array[i] << ",";
        cell->addNeighborAtIndex(array[i]);
    }
    std:cout << std::endl;
}
#pragma mark - Fill event in queue
inline int AKCollisionDetectionDiscreteTimeLogic2D::dimension()
{
    return 2;
}
inline int AKCollisionDetectionDiscreteTimeLogic2D::countUnreachableCells()
{
    return 3;
}
inline int AKCollisionDetectionDiscreteTimeLogic2D::countOfHalfNeighbors()
{
    return 4;
}
inline void AKCollisionDetectionDiscreteTimeLogic2D::setMeasure(int* measure, int* nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle)
{
    if (sign == 1) {
        if (compareType == AKCollisionCompareXType) {
            measure[0] = -1; measure[1] = 0; *nextIndexCell = particle->cellIndex + 1;
        } else if(compareType == AKCollisionCompareYType) {
            measure[0] = 0; measure[1] =  1; *nextIndexCell = particle->cellIndex + _cellsCountInRow;
        }
    } else {
        if (compareType == AKCollisionCompareXType) {
            measure[0] = 1; measure[1] = 0; *nextIndexCell = particle->cellIndex - 1;
        } else if (compareType == AKCollisionCompareYType) {
            measure[0] = 0; measure[1] = -1; *nextIndexCell = particle->cellIndex - _cellsCountInRow;
        }
    }
}
#pragma mark - /* ---------- PUBLIC ---------- */
inline int AKCollisionDetectionDiscreteTimeLogic2D::indexOfCellForParticle(AKParticle const * particle)
{
    int indexX, indexY, returnIndex;
    double pointX = (particle->sphere.center)[0], pointY = (particle->sphere.center)[1];
    indexX = static_cast<int>(pointX / _cellWidth);
    indexY = static_cast<int>(pointY / _cellHeight);
    returnIndex = indexY * _cellsCountInRow + indexX;
    assert (returnIndex < _cellList->size() && returnIndex >= 0);
    return returnIndex;
}
void AKCollisionDetectionDiscreteTimeLogic2D::updateEventQueueInTime(double time)
{
    AKCollisionDetectionDiscreteTimeLogic::updateEventQueueInTime(time);
    assert(_nextEvent->firstParticle->sphere.center[0] + _nextEvent->firstParticle->sphere.radius < _bounds->center[0] + _bounds->radius[0]);
    assert(_nextEvent->firstParticle->sphere.center[0] - _nextEvent->firstParticle->sphere.radius >= -1);
    assert(_nextEvent->firstParticle->sphere.center[1] + _nextEvent->firstParticle->sphere.radius < _bounds->center[1] + _bounds->radius[1]);
    assert(_nextEvent->firstParticle->sphere.center[1] - _nextEvent->firstParticle->sphere.radius >= -1);
}
void AKCollisionDetectionDiscreteTimeLogic2D::setBound(AKBox* bounds, float originX, float originY, float originZ, unsigned int row, unsigned int col, unsigned int range)
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
    fillUnreachableNeighbors();
    fillReachableNeighbors();
}
void AKCollisionDetectionDiscreteTimeLogic2D::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        AKShapeVisualizer::getInstance().visualizeCell(cell, _originX, _originY, 0);
    }
}