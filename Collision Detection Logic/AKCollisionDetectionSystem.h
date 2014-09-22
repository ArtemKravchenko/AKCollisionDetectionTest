//
//  AKCollisionDetectionSystem.h
//  AKCollisionDetectionFramework
//
//  Created by Artem Kravchenko on 9/21/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AK_COLLISION_DETECTION_SYSTEM
#define AK_COLLISION_DETECTION_SYSTEM

#include "AKBox.h"
#include <vector>
#include "AKCollisionDetectionLogic.h"

typedef std::vector<AKBox*> AKCellsList;

class AKCollisionDetectionSystem {
    
private:
    AKBox                       *_bounds;   // bounds of system
    AKCellsList                 *_cells;    // cells bounds
    AKCollisionDetectionLogic   *_logic;    // logic that manipulate process of collision detection of particles
    
    
public:
    void prepeareSystemToStart();
    void updateSystemInTime(int time);
    
};

#endif