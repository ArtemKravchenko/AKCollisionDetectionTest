

#ifndef __AKParticle_h__
#define __AKParticle_h__

#include "AKVisualizedModel.h"
#include "AKSphere.h"
using namespace std;

struct AKParticle: public AKVisualizedModel
{
    double              mass;
    double              localTime;
    double              velocity[3];
    int                 cellIndex;
    AKSphere            sphere;
    
    AKParticle(unsigned int dimension) : sphere (dimension)
    {
        
    }
    ~AKParticle()
    {
        mass = NULL;
        localTime = NULL;
        cellIndex = NULL;
    }
    
    virtual void draw() {
        sphere.draw();
    }
        
    bool operator==(const AKParticle &other) const {
        for (int i = 0; i < 3; i++) {
            if (!IS_EQUAL_WITH_ERROR(this->velocity[i], other.velocity[i])) {
                return false;
            }
        }
        return (this->sphere == other.sphere) && (this->mass == other.mass) && (IS_EQUAL_WITH_ERROR(this->localTime, other.localTime)) && this->cellIndex == other.cellIndex;
    }
    
    bool operator!=(const AKParticle &other) const {
        return !(*this == other);
    }
};

#endif
