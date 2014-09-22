//
//  AKShape.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_SHAPE
#define AK_SHAPE

class AKShape {
    
private:
    bool                _is2Ddimension;
    
public:
    const bool is2Ddimension() const { return _is2Ddimension; }
    void set2Ddimension(bool is2Ddimension) { _is2Ddimension = is2Ddimension; }
};

#endif