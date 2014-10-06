//
//  AKException.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/5/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AKCollisionDetectionTest_AKException_h
#define AKCollisionDetectionTest_AKException_h

class wrongEventTypeException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Wrong event type";
    }
};

class wrongCollisionCompareTypeException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Wrong collision compare type";
    }
};

#endif
