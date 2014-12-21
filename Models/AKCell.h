#include <string>
#include <vector>
#include <exception>
#include "AKDefines.h"
using namespace std;

#ifndef __AKCell_h__
#define __AKCell_h__

#include "AKBox.h"
#include "AKParticle.h"


static const int NEIGHBORS_COUNT = 8;
typedef std::vector<AKParticle*> AKParticlesList;

struct AKCell: public AKVisualizedModel
{
    int             neighbors[NEIGHBORS_COUNT];
    int             index;
    AKBox           bounds;
    AKParticlesList insideParticles;
    
    AKCell(unsigned int dimension) : bounds(dimension) {  }
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
