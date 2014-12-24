//
//  AKCollisionDetectionDiscreteTimeLogic.h
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
#include "AKEvent.h"
#include <iostream>
#include <cmath>
#include "AKGeometricUtils.h"

using std::vector;
typedef vector<AKCell>             AKCellsList;

class AKCollisionDetectionDiscreteTimeLogic {
    
#ifdef UNIT_TESTS
    friend class collision_detection_suite;
    friend class collision_detection_test;
#endif
    
protected:
    /* ------------------------------ METHODS ------------------------------ */
    DEFINE_IS_EUQAL_WITH_ERROR
    AKEvent*    insertEventInQueue(AKParticle const * firstParticle, AKParticle const * secondParticle);
    AKEvent*    insertEventInQueue(AKParticle const * firstParticle, AKBox const * box);
    void        nextEventFromListEvents();
    void        updateParticlesLocation(double time);
    void        addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex);
    void        addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const int *neighborsIndexes);
    void        handleParticleToParticleCollisionEvent();
    void        handleParticleToBoundCollisionEvent();
    void        handleParticleLeftCellCollisionEvent();
    void        removeEventsForParticle(AKParticle* particle);
    void        addEventsRelatedToParticle(AKParticle* particle);
    inline bool     isEventContainsParticle(AKEvent *event, AKParticle* particle) {
        return (*(event->firstParticle) == *particle || *(event->secondParticle) == *particle);
    }
    /* ------------------------------ Virtual ------------------------------ */
    virtual int dimension() = 0;
    virtual int indexOfCellForParticle(AKParticle const * particle) = 0;
    virtual void setMeasure(int* measure, int*nextIndexCell, int sign, AKCollisionCompareType compareType, AKParticle const *particle) = 0;
    /* ----------------------------- VARIABLES ----------------------------- */
    bool                _isBoundsAlreadySet = false;    // Flag manage is bounds was already set
    bool                _isParticlesAlreadySet = false; // Flag manage is particles was already set
    AKBox               *_bounds;                       // Bounds of system
    AKParticlesList     *_particleList;                 // List of particles
    AKPriorityQueue     *_eventsQueue;                  // Queue of events
    int                 _cellsCountInRow;               // Number of cells in each imaginary row
    int                 _cellsCountInCol;               // Number of cells in each imaginary column
    double              _cellWidth;                     // Width of cell
    double              _cellHeight;                    // Height of cell
    AKEvent             *_nextEvent;                    // Event that happens in time with smallest interval
    double              _timeTotal;                     // Counter of globale time
    double              _timeToEvent;                   // Time to event occurs

public:
    AKCellsList         *_cellList;                     // List of cells
    /* ------------------------------ METHODS ------------------------------ */
    void                fillEventsInQueue(unsigned int capacity = 100);
    void                setParticlesList(AKParticlesList *particlsList);
    
    /* ------------------------------ Virtual ------------------------------ */
    virtual void        updateEventQueueInTime(double time);
    virtual void        setBound(AKBox* bounds, float originX = 0, float originY = 0, float originZ = 0, unsigned int row = 1, unsigned int col = 1, unsigned int range = 1) = 0;
    virtual void        drawCells() = 0;
};

#endif