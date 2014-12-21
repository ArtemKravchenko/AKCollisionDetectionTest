#ifndef __AKBox_h__
#define __AKBox_h__

#include "AKShape.h"
#include "AKVisualizedModel.h"

struct AKBox: public AKShape, public AKVisualizedModel
{
    double center[3];
    double radius[3];
    
    AKBox(unsigned int dimension)
    {
        is2dDimension = (dimension == 2);
    }
    ~AKBox()
    {
    }
    
    bool operator==(const AKBox &other) const {
        for (int i = 0; i < 3; i++) if (!IS_EQUAL_WITH_ERROR(this->center[i], other.center[i]) || !IS_EQUAL_WITH_ERROR(this->radius[i], other.radius[i])) return false;
        return true;
    }
    
    bool operator!=(const AKBox &other) const {
        return !(*this == other);
    }
};

#endif
