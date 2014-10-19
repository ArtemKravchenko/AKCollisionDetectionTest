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

struct AKCell : public AKShape {
    AKBox               bounds;
    AKParticlesList     insideParticles;
    unsigned int        neighbors[4];
    unsigned int        index;
    
    void addParticle(AKParticle* particle) { insideParticles->push_back(particle); }
    void addNeighbors(int neighbors[4])
    {
        insideParticles = new AKParticlesList();
        neighbors[0] = neighbors[0];
        neighbors[1] = neighbors[1];
        neighbors[2] = neighbors[2];
        neighbors[3] = neighbors[3];
    }
};

#endif
