//
//  AKCollisionDetectionEventDrivenLogic3D.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/28/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "AKCollisionDetectionEventDrivenLogic3D.h"
#include <assert.h>

#pragma mark - /* ---------- PROTECTED ---------- */
void AKCollisionDetectionEventDrivenLogic3D::fillReachableNeighbors()
{
    // TOOD: Need to recheck !!!
    unsigned int countOfCells = _cellList->size() & INT_MAX;
    _reachableNeighbors = new int*[countOfCells];
    int *arrayPart;
    int indexOfArayPart = 0;
    int sign[2] = {1, -1}, koef[2] = {1, _cellsCountInRow};
    int index = -1;
    for (int i = 0; i < countOfCells; i++) {
        indexOfArayPart = 0;
        _reachableNeighbors[i] = new int[56];
        for (int j = 0; j < 12; j++) { _reachableNeighbors[i][j] = - 1; }
        for (int i2 = 0; i2 < 2; i2++) {
            for (int i1 = 0; i1 < 2; i1++) {
                if (!validIndexForSignAndKoef(i, (sign[i1] == 1) ? -1 : 1, koef[i2])) { indexOfArayPart+=9; continue; }
                index = i + sign[i1] * koef[i2];
                arrayPart = fillReachableNeighborsForCell(i, sign[i1], koef[i2]);
                for (int i3 = 0;i3 < 9; i3++ ) {
                    _reachableNeighbors[i][indexOfArayPart++] = arrayPart[i3];
                }
            }
        }
    }
}
int* AKCollisionDetectionEventDrivenLogic3D::fillReachableNeighborsForCell(int index, int sign, int koef)
{
    bool right, left, top, bottom;
    int full2DSquare = _cellsCountInCol * _cellsCountInRow;
    int *arrayPart = new int[9];
    for (int i = 0; i < 9; i++) arrayPart[i] = -1;
    
    if (validIndexForSignAndKoef(index, sign, koef)) {
        arrayPart[0] = index + sign*koef;
        
        right = validRightNeighbor(arrayPart[0], sign, koef);
        if (right) {
            if (koef == 1) {
                arrayPart[1] = arrayPart[0] + full2DSquare;
            } else {
                arrayPart[1] = arrayPart[0] + 1;
            }
        }
        bottom = validBottomNeighbor(arrayPart[0], sign, koef);
        if (bottom) {
            if (koef == _cellsCountInRow) {
                arrayPart[2] = arrayPart[0] - full2DSquare;
            } else {
                arrayPart[2] = arrayPart[0] - _cellsCountInRow;
            }
        }
        left = validLeftNeighbor(arrayPart[0], sign, koef);
        if (left) {
            if (koef == 1) {
                arrayPart[3] = arrayPart[0] - full2DSquare;
            } else {
                arrayPart[3] = arrayPart[0] - 1;
            }
        }
        top = validTopNeighbor(arrayPart[0], sign, koef);
        if (top) {
            if (koef == _cellsCountInRow) {
                arrayPart[4] = arrayPart[0] + full2DSquare;
            } else {
                arrayPart[4] = arrayPart[0] + _cellsCountInRow;
            }
        }
        if (right && bottom) {
            if (koef == 1) {
                arrayPart[5] = arrayPart[2] + full2DSquare;
            } else {
                arrayPart[5] = arrayPart[2] + 1;
            }
        }
        if (left && bottom) {
            if (koef == 1) {
                arrayPart[6] = arrayPart[2] - full2DSquare;
            } else {
                arrayPart[6] = arrayPart[2] - 1;
            }
        }
        if (top && left) {
            if (koef == 1) {
                arrayPart[7] = arrayPart[4] - full2DSquare;
            } else {
                arrayPart[7] = arrayPart[4] - 1;
            }
        }
        if (top && right) {
            if (koef == 1) {
                arrayPart[8] = arrayPart[4] + full2DSquare;
            } else {
                arrayPart[8] = arrayPart[4] + 1;
            }
        }
    }
    return arrayPart;
}
#pragma mark - filling unreachable neigbors
void AKCollisionDetectionEventDrivenLogic3D::fillUnreachableNeighbors()
{
    // TOOD: Need to recheck !!!
    unsigned int countOfCells = _cellList->size() & INT_MAX;
    int full2DSquare = _cellsCountInCol*_cellsCountInRow;
    _unrachableNeighbors = new int*[countOfCells];
    int *arrayPart;
    int indexOfArayPart = 0;
    int sign[2] = {-1, 1}, koef[3] = {1, _cellsCountInRow, full2DSquare};
    int index = -1;
    for (int i = 0; i < countOfCells; i++) {
        cout << "i=" << i << ":"<< " ";
        indexOfArayPart = 0;
        _unrachableNeighbors[i] = new int[56];
        for (int j = 0; j < 12; j++) { _unrachableNeighbors[i][j] = - 1; }
        for (int i2 = 0; i2 < 3; i2++) {
            for (int i1 = 0; i1 < 2; i1++) {
                if (!validIndexForSignAndKoef(i, sign[i1], koef[i2])) { indexOfArayPart+=9; continue; }
                index = i + sign[i1] * koef[i2];
                arrayPart = fillUnreachableNeighborsForCell(index, sign[i1], koef[i2]);
                for (int i3 = 0;i3 < 9; i3++ ) {
                    cout << arrayPart[i3];
                    if (indexOfArayPart != 55) {
                        cout << ",";
                    }
                    _unrachableNeighbors[i][indexOfArayPart++] = arrayPart[i3];
                }
            }
        }
        cout << endl;
    }
}
bool AKCollisionDetectionEventDrivenLogic3D::validIndexForSignAndKoef(int index, int sign, int koef)
{
    int full2DSquare = _cellsCountInCol * _cellsCountInRow;
    if (index + sign*koef < 0 || index + sign*koef >= _cellsCountInRow * _cellsCountInCol * _cellsCountInRange) {
        return false;
    }
    if (sign == - 1) {
        if (koef == 1) {
            return ((index % full2DSquare) % _cellsCountInRow != 0);
        } else if (koef == _cellsCountInRow) {
            return ((index % full2DSquare) >= _cellsCountInRow);
        } else {
            return (index >= full2DSquare);
        }
    } else {
        if (koef == 1) {
            return (((index % full2DSquare)+1) % _cellsCountInRow != 0);
        } else if (koef == _cellsCountInRow) {
            return ((index % full2DSquare) < _cellsCountInRow*(_cellsCountInCol-1));
        } else {
            return (index < full2DSquare * (_cellsCountInRange - 1));
        }
    }
}
int* AKCollisionDetectionEventDrivenLogic3D::fillUnreachableNeighborsForCell(int index, int sign, int koef)
{
    bool right, left, top, bottom;
    int full2DSquare = _cellsCountInCol * _cellsCountInRow;
    int *arrayPart = new int[9];
    for (int i = 0; i < 9; i++) { arrayPart[i] = -1; }
    if (validIndexForSignAndKoef(index, sign, koef)) {
        arrayPart[0] = index + sign*koef;
        
        right = validRightNeighbor(arrayPart[0], sign, koef);
        if (right) {
            if (koef == 1) {
                arrayPart[1] = arrayPart[0] + full2DSquare;
            } else {
                arrayPart[1] = arrayPart[0] + 1;
            }
        }
        bottom = validBottomNeighbor(arrayPart[0], sign, koef);
        if (bottom) {
            if (koef == _cellsCountInRow) {
                arrayPart[2] = arrayPart[0] - full2DSquare;
            } else {
                arrayPart[2] = arrayPart[0] - _cellsCountInRow;
            }
        }
        left = validLeftNeighbor(arrayPart[0], sign, koef);
        if (left) {
            if (koef == 1) {
                arrayPart[3] = arrayPart[0] - full2DSquare;
            } else {
                arrayPart[3] = arrayPart[0] - 1;
            }
        }
        top = validTopNeighbor(arrayPart[0], sign, koef);
        if (top) {
            if (koef == _cellsCountInRow) {
                arrayPart[4] = arrayPart[0] + full2DSquare;
            } else {
                arrayPart[4] = arrayPart[0] + _cellsCountInRow;
            }
        }
        if (right && bottom) {
            if (koef == 1) {
                arrayPart[5] = arrayPart[2] + full2DSquare;
            } else {
                arrayPart[5] = arrayPart[2] + 1;
            }
        }
        if (left && bottom) {
            if (koef == 1) {
                arrayPart[6] = arrayPart[2] - full2DSquare;
            } else {
                arrayPart[6] = arrayPart[2] - 1;
            }
        }
        if (top && left) {
            if (koef == 1) {
                arrayPart[7] = arrayPart[4] - full2DSquare;
            } else {
                arrayPart[7] = arrayPart[4] - 1;
            }
        }
        if (top && right) {
            if (koef == 1) {
                arrayPart[8] = arrayPart[4] + full2DSquare;
            } else {
                arrayPart[8] = arrayPart[4] + 1;
            }
        }
    }
    return arrayPart;
}
bool AKCollisionDetectionEventDrivenLogic3D::validRightNeighbor(int index, int sign, int koef)
{
    if (koef == 1) {
        return index < _cellsCountInCol*_cellsCountInRow*(_cellsCountInRow - 1);
    } else {
        return ((index + 1) % _cellsCountInRow != 0) ;
    }
}
bool AKCollisionDetectionEventDrivenLogic3D::validLeftNeighbor(int index, int sign, int koef)
{
    if (koef == 1) {
        return index >= _cellsCountInRow*_cellsCountInCol;
    } else {
        return index % _cellsCountInRow != 0;
    }
}
bool AKCollisionDetectionEventDrivenLogic3D::validTopNeighbor(int index, int sign, int koef)
{
    if (koef == _cellsCountInRow) {
        return index < _cellsCountInCol*_cellsCountInRow*(_cellsCountInRow - 1);
    } else {
        return (index % (_cellsCountInCol*_cellsCountInRow)) < _cellsCountInRow*(_cellsCountInCol-1);
    }
}
bool AKCollisionDetectionEventDrivenLogic3D::validBottomNeighbor(int index, int sign, int koef)
{
    if (koef == _cellsCountInRow) {
        return index >= _cellsCountInRow*_cellsCountInCol;
    } else {
        return (index % (_cellsCountInRow*_cellsCountInCol)) >= _cellsCountInRow;
    }
}
#pragma mark - Set Bounds
int AKCollisionDetectionEventDrivenLogic3D::countOfCell()
{
    return AKCollisionDetectionEventDrivenLogic2D::countOfCell() * _cellsCountInRange;
}
void AKCollisionDetectionEventDrivenLogic3D::fillCell(AKCell& cell, int index)
{
    int indexXY = (index + (_cellsCountInCol * _cellsCountInRow)) % (_cellsCountInCol * _cellsCountInRow);
    AKCollisionDetectionEventDrivenLogic2D::fillCell(cell, indexXY);
    cell.bounds.center[2] = _cellDepth * (index / (_cellsCountInCol * _cellsCountInRow)) + _cellDepth / 2;
    cell.bounds.radius[2] = _cellDepth / 2;
}
void AKCollisionDetectionEventDrivenLogic3D::fillNeighborsForCell(AKCell *cell,int index)
{
    AKCollisionDetectionEventDrivenLogic2D::fillNeighborsForCell(cell, index);
    int full2DSquare = _cellsCountInCol * _cellsCountInRow;
    int topNeighborBack = -1, topRightNeighborBack = -1, rightNeightborBack = -1, bottomRightNeighborBack = -1, bottomNeighborBack = -1,
    topNeighborFront = -1, topRightNeighborFront = -1, rightNeightborFront = -1, bottomRightNeighborFront = -1, bottomNeighborFront = -1, middleNeighBorFront = -1;
    if (index >= full2DSquare) {
        if (cell->neighbors.at(3) != -1) {
            topNeighborBack = index + _cellsCountInRow - full2DSquare;
        }
        if (cell->neighbors.at(1) != -1) {
            rightNeightborBack = index + 1 - full2DSquare;
        }
        if ((index % full2DSquare) >= _cellsCountInRow) {
            bottomNeighborBack = index - _cellsCountInRow - full2DSquare;
        }
        if (topNeighborBack != -1 && rightNeightborBack != - 1) {
            topRightNeighborBack = topNeighborBack + 1;
        }
        if (bottomNeighborBack != -1 && rightNeightborBack != -1) {
            bottomRightNeighborBack = bottomNeighborBack + 1;
        }
    }
    if (index < full2DSquare * (_cellsCountInRange - 1)) {
        middleNeighBorFront = index + full2DSquare;
        if ((cell->neighbors.at(3) != -1)) {
            topNeighborFront = index + _cellsCountInRow + full2DSquare;
        }
        if (cell->neighbors.at(1) != -1) {
            rightNeightborFront = index + 1 + full2DSquare;
        }
        if ((index % full2DSquare) >= _cellsCountInRow) {
            bottomNeighborFront = index - _cellsCountInRow + full2DSquare;
        }
        if (topNeighborFront != -1 && rightNeightborFront != - 1) {
            topRightNeighborFront = topNeighborFront + 1;
        }
        if (bottomNeighborFront != -1 && rightNeightborFront != -1) {
            bottomRightNeighborFront = bottomNeighborFront + 1;
        }
    }
    int array[9] = { topRightNeighborBack, rightNeightborBack, bottomRightNeighborBack, topNeighborFront, topRightNeighborFront, rightNeightborFront, bottomRightNeighborFront, bottomNeighborFront, middleNeighBorFront };
    std::cout << index << " :";
    for (int i = 0; i < countUnreachableCells(); i++) {
        std::cout << array[i] << ",";
        cell->addNeighborAtIndex(array[i]);
    }
    std:cout << std::endl;
}
#pragma mark - Fill event in queue
inline int AKCollisionDetectionEventDrivenLogic3D::dimension()
{
    return 3;
}
inline int  AKCollisionDetectionEventDrivenLogic3D::countUnreachableCells()
{
    return 9;
}
inline int AKCollisionDetectionEventDrivenLogic3D::countOfHalfNeighbors()
{
    return 13;
}
inline void AKCollisionDetectionEventDrivenLogic3D::setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle)
{
    AKCollisionDetectionEventDrivenLogic2D::setMeasure(measure, nextIndexCell, sign, compareType, particle);
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
inline int AKCollisionDetectionEventDrivenLogic3D::indexOfCellForParticle(AKParticle const * particle)
{
    int index2D = AKCollisionDetectionEventDrivenLogic2D::indexOfCellForParticle(particle);
    double pointZ = (particle->sphere.center)[2];
    int indexZ = pointZ / _cellDepth;
    int returnIndex = _cellsCountInCol * _cellsCountInRow * indexZ + index2D;
    assert (returnIndex < _cellList->size() && returnIndex >= 0);
    return returnIndex;
}
void AKCollisionDetectionEventDrivenLogic3D::updateEventQueueInTime(double time)
{
    AKCollisionDetectionEventDrivenLogic2D::updateEventQueueInTime(time);
    assert(_nextEvent->firstParticle->sphere.center[2] + _nextEvent->firstParticle->sphere.radius < _bounds->center[2] + _bounds->radius[2]);
    assert(_nextEvent->firstParticle->sphere.center[2] - _nextEvent->firstParticle->sphere.radius >= -1);
}
void AKCollisionDetectionEventDrivenLogic3D::setBound(AKBox* bounds, float originX, float originY, float originZ, unsigned int row, unsigned int col, unsigned int range)
{
    _originZ = originZ;
    _cellsCountInRange = range;
    _cellDepth = (abs(bounds->center[2]) + bounds->radius[2]) / _cellsCountInRange;
    _isBoundsAlreadySet = true;
    AKCollisionDetectionEventDrivenLogic2D::setBound(bounds, originX, originY, originZ, row, col, range);
}
void AKCollisionDetectionEventDrivenLogic3D::drawCells()
{
    int count = _cellList->size() & INT_MAX;
    for (int i = 0; i < count; i++) {
        AKCell *cell = &_cellList->at(i);
        AKShapeVisualizer::getInstance().visualizeCell(cell, _originX, _originY, _originZ);
    }
}