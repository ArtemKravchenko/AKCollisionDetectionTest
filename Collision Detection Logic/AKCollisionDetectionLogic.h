//
//  AKCollisionDetectionLogic.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_COLLISION_DETECTION_LOGIC
#define AK_COLLISION_DETECTION_LOGIC

#include "AKCell.h"
#include <vector>
#include <queue>

typedef enum {
    AKEventParticleToParticleType,
    AKEventParticleToBoundType,
    AKEventParticleToCellType,
} AKEventType;

struct AKEvent {
    AKEventType     eventType;
    AKParticle      *firstParticle;
    AKParticle      *secondParticle;
    double          measure;
    double          timeToEvent;
};

typedef std::vector <AKCell*> AKCellsList;
typedef std::priority_queue<AKEvent*> AKEventsQueue;

class AKCollisionDetectionLogic {
    
private:
    /* ----------------------------- VARIABLES ----------------------------- */
    AKBox               *_bounds;           // bounds of system
    AKParticlesList     *_particleList;     // list of particles
    AKEventsQueue       *_eventsQueue;      // priority queue of events
    AKCellsList         *_cellList;         // list of cells
    int                 _cellsCountInRow;   // Number of cells in each imaginary row
    int                 _cellsCountInCol;   // Number of cells in each imaginary column
    double              _cellWidth;         // Width of cell
    double              _cellHeight;        // Height of cell
    double              _cellDepth;         // Depth of cell
    AKEvent             *nextEvent;         // Event that happens in time with smallest interval
    /* ------------------------------ METHODS ------------------------------ */
    int         numberOfCellForParticle(AKParticle const * particle);
    void        fillNeighborsForCell(AKCell *cell,int index);
    AKEvent*    getEvent(AKParticle const * firstParticle, AKParticle const * secondParticle);
    AKEvent*    getEvent(AKParticle const * firstParticle, AKBox const * box);
    void        addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, int startIndex);
    void        addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes);
    
public:
    /* ------------------------------ METHODS ------------------------------ */
    void fillEventsInQueue();
    void updateEventQueueInTime(int time);
    void setParticlesList(AKParticlesList *particlsList);
    void setBound(AKBox* bounds);
    void setCellRowAndCol(int row, int col);
};

#endif