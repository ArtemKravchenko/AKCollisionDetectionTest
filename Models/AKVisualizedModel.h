#include <string>
#include <vector>
#include <exception>

#ifndef __AKVisualizedModel_h__
#define __AKVisualizedModel_h__
#include "AKDefines.h"

struct AKVisualizedModel
{
    virtual void draw() = 0;
    static bool isVisible;
    DEFINE_IS_EUQAL_WITH_ERROR
};
#endif
