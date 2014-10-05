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

typedef enum {
    AKEventParticleToParticleType,
    AKEventParticleToBoundType,
    AKEventParticleToCellType,
} AKEventType;

using std::vector;

struct AKEvent {
    AKEventType     eventType;
    AKParticle      *firstParticle;
    AKParticle      *secondParticle;
    double          measure;
    double          timeToEvent;
};

typedef vector<AKCell*>     AKCellsList;
typedef vector<AKEvent*>    AKEventsList;

class AKCollisionDetectionLogic {
    
private:
    /* ----------------------------- VARIABLES ----------------------------- */
    bool                _isBoundsAlreadySet = false;    // Flag manage is bounds was already set
    bool                _isParticlesAlreadySet = false; // Flag manage is particles was already set
    AKBox               *_bounds;                       // bounds of system
    AKParticlesList     *_particleList;                 // list of particles
    AKEventsList        *_eventsList;                   // list of events
    AKCellsList         *_cellList;                     // list of cells
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
    /* ------------------------------ METHODS ------------------------------ */
    void            fillNeighborsForCell(AKCell *cell,int index);
    inline int      indexOfCellForParticle(AKParticle const * particle);
    inline AKEvent* getEvent(AKParticle const * firstParticle, AKParticle const * secondParticle);
    inline AKEvent* getEvent(AKParticle const * firstParticle, AKBox const * box);
    inline AKEvent* nextEventFromListEvents();
    inline void     updateParticlesLocation();
    inline void     addEventsForParticleAndParticlesInCurrentCell(AKParticle *particle, AKCell* cell, unsigned int startIndex);
    inline void     addEventsForParticleAndParticlesInNeighborCells(AKParticle *particle, const unsigned int *neighborsIndexes);
    inline void     handleParticleToParticleCollisionEvent();
    inline void     handleParticleToBoundCollisionEvent();
    inline void     handleParticleLeftCellCollisionEvent();
    inline void     removeEventsForParticle(AKParticle* particle);
    inline void     addEventsRelatedToParticle(AKParticle* particle);
    inline bool     isEventContainsParticle(AKEvent *event, AKParticle* particle);
    
public:
    /* ------------------------------ METHODS ------------------------------ */
    void fillEventsInQueue();
    void updateEventQueueInTime(int time);
    void setParticlesList(AKParticlesList *particlsList);
    void setBound(AKBox* bounds, unsigned int row = 1, unsigned int col = 1, unsigned int range = 0);
};

#endif