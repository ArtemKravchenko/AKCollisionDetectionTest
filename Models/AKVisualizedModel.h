//
//  AKVisualizedModel.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_VISUALIZED_MODEL
#define AK_VISUALIZED_MODEL

class AKVisualizedModel {

private:
    bool _isVisible;

public:
    bool isVisible() { return _isVisible; }
    void setVisible(bool visible) { _isVisible = visible; }
};

#endif