//
//  AKCell.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_CELL
#define AK_CELL

#include "AKBox.h"
#include <vector>
#include "AKParticle.h"

typedef std::vector<AKParticle*> AKParticlesList;

class AKCell : public AKShape {
    
private:
    AKBox               *_bounds;
    AKParticlesList   *_insideParticles;
    int                 _neighbors[4];
    
public:
    AKCell();
    AKCell(AKBox* bounds);
    ~AKCell();
    void addNeighbors(int neighbors[4]);
    void addParticle(AKParticle *particle);
    AKBox const * getBounds() const;
    AKParticlesList const * getInsideParticles() const;
    int const * getNeighbors();
};

#endif
