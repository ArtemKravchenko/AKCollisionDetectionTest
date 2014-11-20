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

static const int NEIGHBORS_COUNT = 8;

typedef std::vector<AKParticle*> AKParticlesList;

struct AKCell : public AKShape {
    AKBox               bounds;
    AKParticlesList     insideParticles;
    int                 neighbors[8];
    unsigned int        index;
    
    AKCell(unsigned int dimension) : bounds(dimension) {}
    ~AKCell() {}
    
    void addParticle(AKParticle* particle)
    {
        if (insideParticles.size() == 0) {
            insideParticles = AKParticlesList();
        }
        insideParticles.push_back(particle);
    }
    void addNeighbors(int array[NEIGHBORS_COUNT])
    {
        for (int i = 0; i < NEIGHBORS_COUNT; i++) {
            neighbors[i] = array[i];
        }
    }
    
    bool operator==(const AKCell &other) const {
        int particlesCount = this->insideParticles.size() & INT_MAX;
        for (int i = 0; i < particlesCount; i++) if (*(this->insideParticles.at(i)) != *other.insideParticles.at(i)) return false;
        for (int i = 0; i < NEIGHBORS_COUNT; i++) if (this->neighbors[i] != other.neighbors[i]) return false;
        return (this->index == other.index) && (this->bounds == other.bounds);
    }
    
    bool operator!=(const AKCell &other) const {
        return !(*this == other);
    }
};

#endif
