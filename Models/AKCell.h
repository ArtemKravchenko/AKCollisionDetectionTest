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
    int                 neighbors[4];
    unsigned int        index;
    
    void addParticle(AKParticle* particle)
    {
        if (insideParticles.size() != 0) {
            insideParticles = AKParticlesList();
        }
        insideParticles.push_back(particle);
    }
    void addNeighbors(int array[4])
    {
        neighbors[0] = array[0];
        neighbors[1] = array[1];
        neighbors[2] = array[2];
        neighbors[3] = array[3];
    }
    
    bool operator==(const AKCell &other) const {
        int particlesCount = this->insideParticles.size() & INT_MAX;
        for (int i = 0; i < particlesCount; i++) if (*(this->insideParticles.at(i)) != *other.insideParticles.at(i)) return false;
        for (int i = 0; i < 4; i++) if (this->neighbors[i] != other.neighbors[i]) return false;
        return (this->index == other.index) && (this->bounds == other.bounds);
    }
    
    bool operator!=(const AKCell &other) const {
        return !(*this == other);
    }
};

#endif
