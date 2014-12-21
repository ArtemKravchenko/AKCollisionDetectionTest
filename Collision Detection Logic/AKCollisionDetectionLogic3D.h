//
//  AKCollisionDetectionLogic3D.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 12/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef __AKCollisionDetectionTest__AKCollisionDetectionLogic3D__
#define __AKCollisionDetectionTest__AKCollisionDetectionLogic3D__

#include <stdio.h>
#include "AKCollisionDetectionLogic2D.h"

class AKCollisionDetectionLogic3D : public AKCollisionDetectionLogic2D {

protected:
    float           _originZ;                       // The most nearest coordinate of system
    int             _cellsCountInRange;             // Number of cells in each imaginary range
    double          _cellDepth;                     // Depth of cell
    
    virtual void    setSystemBounds(AKBox* bounds);
    virtual int     countOfCell();
    virtual void    fillCell(AKCell& cell, int index);
    virtual void    fillNeighborsForCell(AKCell *cell,int index);
    virtual int     dimension();
    virtual void    setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle);
    
public:
    virtual void    updateEventQueueInTime(double time);
    virtual int     indexOfCellForParticle(AKParticle const * particle);
    virtual void    setBound(AKBox* bounds, float originX = 0, float originY = 0, float originZ = 0, unsigned int row = 1, unsigned int col = 1, unsigned int range = 1);
    virtual void    drawCells();
};

#endif /* defined(__AKCollisionDetectionTest__AKCollisionDetectionLogic3D__) */
