using namespace std;

#ifndef __AKSphere_h__
#define __AKSphere_h__

#include "AKShape.h"
#include "AKVisualizedModel.h"

struct AKSphere: public AKShape, public AKVisualizedModel
{
    double center[3];
    double radius;
    
    AKSphere(unsigned int dimension)
    {
        is2dDimension = (dimension == 2);
    }
    ~AKSphere()
    {
        radius = NULL;
    }
    
    bool operator==(const AKSphere &other) const {
        for (int i = 0; i < 3; i++) if (!IS_EQUAL_WITH_ERROR(this->center[i], other.center[i])) return false;
        return IS_EQUAL_WITH_ERROR(this->radius, other.radius);
    }
    
    bool operator!=(const AKSphere &other) const {
        return !(*this == other);
    }
    
    virtual void draw() {
        glLoadIdentity();
        float x = center[0];
        float y = center[1];
        float z = 0;
        if (!is2dDimension) {
            z = center[2];
        }
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(x, y, z);
        glutSolidSphere(radius, 20, 2);
    }
};

#endif
