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
#include "akpriorityqueue.h"
#include "akevent.h"

using std::vector;
typedef vector<AKCell*>             AKCellsList;

class AKCollisionDetectionLogic {

protected:
    /* ------------------------------ METHODS ------------------------------ */
    AKEvent* insertEventInQueue(AKParticle const * firstParticle, AKParticle const * secondParticle);
    AKEvent* insertEventInQueue(AKParticle const * firstParticle, AKBox const * box);
    AKEvent* nextEventFromListEvents();
    void     updateParticlesLocation();
    void     addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex);
    void     addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes);
    void     handleParticleToParticleCollisionEvent();
    void     handleParticleToBoundCollisionEvent();
    void     handleParticleLeftCellCollisionEvent();
    void     removeEventsForParticle(AKParticle* particle);
    void     addEventsRelatedToParticle(AKParticle* particle);
    bool     isEventContainsParticle(AKEvent *event, AKParticle* particle);
    /* ----------------------------- VARIABLES ----------------------------- */
    bool                _isBoundsAlreadySet = false;    // Flag manage is bounds was already set
    bool                _isParticlesAlreadySet = false; // Flag manage is particles was already set
    AKBox               *_bounds;                       // Bounds of system
    AKParticlesList     *_particleList;                 // List of particles
    AKPriorityQueue     *_eventsQueue;                  // Queue of events
    AKCellsList         *_cellList;                     // List of cells
    int                 _cellsCountInRow;               // Number of cells in each imaginary row
    int                 _cellsCountInCol;               // Number of cells in each imaginary column
    int                 _cellsCountInRange;             // Number of cells in each imaginary range
    double              _cellWidth;                     // Width of cell
    double              _cellHeight;                    // Height of cell
    double              _cellDepth;                     // Depth of cell
    AKEvent             *_nextEvent;                    // Event that happens in time with smallest interval
    double              _timeTotal;                     // Counter of globale time
    double              _timeToEvent;                   // Time to event occurs
    double              _error;                         // Error of computations

public:
    /* ------------------------------ METHODS ------------------------------ */
    void            fillNeighborsForCell(AKCell *cell,int index);
    int             indexOfCellForParticle(AKParticle const * particle);
    void fillEventsInQueue();
    void updateEventQueueInTime(int time);
    void setParticlesList(AKParticlesList *particlsList);
    void setBound(AKBox* bounds, unsigned int row = 1, unsigned int col = 1, unsigned int range = 0);
};

#endif