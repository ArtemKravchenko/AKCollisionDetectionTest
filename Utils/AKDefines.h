//
//  AKUtils.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AKCollisionDetectionTest_AKDefines_h
#define AKCollisionDetectionTest_AKDefines_h

#ifndef AKOpenGLInclude
#define AKOpenGLInclude

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#endif

static double GLOBAL_MIN_RADIUS;
static double GLOBAL_BOUND_X;
static double GLOBAL_BOUND_Y;
static double GLOBAL_BOUND_Z;

#define DEFINE_IS_EUQAL_WITH_ERROR\
        constexpr static const double ERROR = 0.01; \
        inline bool isEqualWithError(double value1, double value2) const { \
        return (value1 - ERROR) <= value2 && (value1 + ERROR) >= value2; }

#define IS_EQUAL_WITH_ERROR(value1, value2) isEqualWithError(value1, value2)

#define DIMENSION_FROM_BOOL(is2Ddimension, dimensionName) int dimensionName = (is2Ddimension) ? 2 : 3;

#define ARRAY_FROM_VECTOR_XD(vector, array, count) \
        array[0] = vector[0]; \
        array[1] = vector[1]; \
        if (count == 3) \
           array[2] = vector[2];

#define VECTOR_DIFERRENCE_FOR_ARRAYS(array1, array2, retArray, count) \
        for(int i = 0; i < count; i++) \
            retArray[i] = array1[i] - array2[i];

#define VECTOR_SUM_FOR_ARRAYS(array1, array2, retArray, count) \
        for(int i = 0; i < count; i++) \
            retArray[i] = array1[i] + array2[i];

#define DOT_PRODUCT_FOR_ARRAYS(array1, array2, retValue, count) \
        for(int i = 0; i < count; i++) \
            retValue += array1[i] * array2[i];

#define PRODUCT_VECTOR_BY_NUMBER(array, number, retArray, count) \
        for(int i = 0; i < count; i++) \
            retArray[i] = array[i] * number;

#endif
