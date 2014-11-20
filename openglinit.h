//
//  openglinit.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AKCollisionDetectionTest_openglinit_h
#define AKCollisionDetectionTest_openglinit_h

#include "AKDefines.h"

class AKOpenGLInitDelegate {
    
public:
    virtual void handleTimerChanges(double time) { /* Need to overload in inheritance classes */ }
    virtual void redrawObjects() { /*  Need to overload in inheritance classes */ }
};

class AKOpenGLInit {    
    
private:
    AKOpenGLInit() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKOpenGLInit(AKOpenGLInit const&);              // Don't Implement
    void operator=(AKOpenGLInit const&); // Don't implement
    
public:
    void initOpenGl(int *argc, char ** argv);
    void init();
    void startModellingProcess();
    AKOpenGLInitDelegate *delegate;
    // SINGLETON
    static AKOpenGLInit& getInstance()
    {
        static AKOpenGLInit    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};

#endif
