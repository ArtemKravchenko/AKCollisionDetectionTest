//
//  AKCollisionDetectionEventDrivenLogic2D.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/28/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__AKCollisionDetectionEventDrivenLogic2D__
#define __AKCollisionDetectionTest__AKCollisionDetectionEventDrivenLogic2D__

#include "AKCollisionDetectionEventDrivenLogic.h"
#include "AKShapeVisualizer.h"

class AKCollisionDetectionEventDrivenLogic2D : public AKCollisionDetectionEventDrivenLogic {
    
protected:
    float               _originX;                       // The most left coordinate of system
    float               _originY;                       // The most bottom coordinate of system
    int                 _cellsCountInRow;               // Number of cells in each imaginary row
    int                 _cellsCountInCol;               // Number of cells in each imaginary column
    double              _cellWidth;                     // Width of cell
    double              _cellHeight;                    // Height of cell
    
    virtual int                     countOfCell();
    virtual void                    fillCell(AKCell& cell, int index);
    virtual void                    fillNeighborsForCell(AKCell *cell,int index);
    virtual int                     dimension();
    virtual void                    setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle);
    virtual void                    fillUnreachableNeighbors();
    virtual void                    fillReachableNeighbors();
    virtual int                     countUnreachableCells();
    virtual int                     countOfHalfNeighbors();
    int*                            fillUnreachableNeighborsForCell(int index, int sign, int koef, int reflectKoef);
    int*                            fillReachableNeighborsForCell(int index, int sign, int koef, int reflectKoef);
    bool                            validIndexForSignAndKoef(int index, int sign, int koef);
    bool                            validPossitiveNeighbors(int index, int sign, int koef);
    bool                            validNegativeNeighbors(int index, int sign, int koef);
    
public:
    virtual void    updateEventQueueInTime(double time);
    virtual int     indexOfCellForParticle(AKParticle const * particle);
    virtual void    setBound(AKBox* bounds, float originX = 0, float originY = 0, float originZ = 0, unsigned int row = 1, unsigned int col = 1, unsigned int range = 1);
    virtual void    drawCells();
};


#endif /* defined(__AKCollisionDetectionTest__AKCollisionDetectionEventDrivenLogic2D__) */
