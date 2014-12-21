#include <string>
#include <vector>
#include <exception>
#include "AKDefines.h"
using namespace std;

#ifndef __AKCell_h__
#define __AKCell_h__

#include "AKBox.h"
#include "AKParticle.h"


static const int NEIGHBORS_COUNT_2D = 8;
static const int NEIGHBORS_COUNT_3D = 26;
typedef std::vector<AKParticle*> AKParticlesList;
typedef std::vector<int> AKNeigbors;

struct AKCell: public AKVisualizedModel
{
    AKNeigbors      neighbors;
    int             neigborsCount;
    int             index;
    AKBox           bounds;
    AKParticlesList insideParticles;
    
    AKCell(unsigned int dimension) : bounds(dimension) {
        neigborsCount = (bounds.is2dDimension) ? NEIGHBORS_COUNT_2D : NEIGHBORS_COUNT_3D;
    }
    ~AKCell() { }
    
    void addParticle(AKParticle* particle)
    {
        if (insideParticles.size() == 0) {
            insideParticles = AKParticlesList();
        }
        insideParticles.push_back(particle);
    }
    void addNeighborAtIndex(unsigned int neighbor)
    {
        if (neighbors.size() == 0) {
            neighbors = AKNeigbors();
        }
        if (neighbors.size() < neigborsCount) {
            neighbors.push_back(neighbor);
        }
    }
    
    bool operator==(const AKCell &other) const {
        int particlesCount = this->insideParticles.size() & INT_MAX;
        for (int i = 0; i < particlesCount; i++) if (*(this->insideParticles.at(i)) != *other.insideParticles.at(i)) return false;
        for (int i = 0; i < neigborsCount; i++) if (this->neighbors[i] != other.neighbors[i]) return false;
        return (this->index == other.index) && (this->bounds == other.bounds);
    }
    
    bool operator!=(const AKCell &other) const {
        return !(*this == other);
    }
};

#endif
