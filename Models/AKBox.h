using namespace std;

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
    
    virtual void draw() {
#ifdef __glut_h__
        glColor3f(1.0, 1.0, 1.0);
        glLoadIdentity();
        glBegin(GL_LINES);
        // Left
        glVertex2f(center[0] - radius[0], center[1] - radius[1]);
        glVertex2f(center[0] - radius[0], center[1] + radius[1]);
        // Top
        glVertex2f(center[0] - radius[0], center[1] + radius[1]);
        glVertex2f(center[0] + radius[0], center[1] + radius[1]);
        // Right
        glVertex2f(center[0] + radius[0], center[1] - radius[1]);
        glVertex2f(center[0] + radius[0], center[1] + radius[1]);
        // Bottom
        glVertex2f(center[0] - radius[0], center[1] - radius[1]);
        glVertex2f(center[0] + radius[0], center[1] - radius[1]);
        glEnd();
#endif
    }
};

#endif
