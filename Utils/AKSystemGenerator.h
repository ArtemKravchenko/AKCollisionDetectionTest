//
//  AKSystemGenerator.h
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 11/15/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#ifndef AKCollisionDetectionTest_AKSystemGenerator_h
#define AKCollisionDetectionTest_AKSystemGenerator_h

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "AKParticle.h"
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

using namespace std;

void fillParticlesArray(int countOfParticles, int* actualCountOfParticles, AKParticle** circlesList);
AKParticle* generateCircle();
bool isCircleIntersectWithAlreadyExistingCircles(AKParticle* circle, int* actualCountOfParticles, AKParticle** circlesList);
bool isCirclesIntersectIn2D(double x1, double y1, double r1, double x2, double y2, double r2);
void convertDataInJSON(int dimension, int countOfPaticles, AKParticle** arrayOfCircles);

class AKSystemGenerator {
    
    
public:
    void generateSystem(int countOfParticles)
    {
        AKParticle** circlesList;
        int dimension = 2;
        int actualCountOfParticles = 0;
        cout << "UTILITY READY TO INSTALL SYSTEM'S ARGUMENTS"<< endl;
        cout << "Please enter the " << endl;
#pragma mark - REAS DIMENSION
        cout << "Dimension: ";
        // TODO: need to implement 3D case
        /*
         string inputDimension = "";
         getline(cin, inputDimension);
         dimension = stoi(inputDimension);
         */
        dimension = 2;
        cout << "Dimension = 2" << endl;
#pragma mark - READ COUNT OF PARTICLES
        //cout << "Count of particles: ";
        //string inputCountOfParticles = "";
        //getline(cin, inputCountOfParticles);
        //countOfParticles = stoi(inputCountOfParticles);
        circlesList = new AKParticle*[countOfParticles];
        cout << "Count of particles = " << countOfParticles << endl;
#pragma mark - FILLING ARRAY OF PARTICLES
        // TODO: need to ask user input
        bool isRandomGeneration = true;
        if (isRandomGeneration) {
            ::fillParticlesArray(countOfParticles, &actualCountOfParticles, circlesList);
        } else {
            // TODO: need to implement reading raw data from file
        }
        convertDataInJSON(dimension, countOfParticles, circlesList);
    }
    
    void fillParticlesArray(int countOfParticles, int* actualCountOfParticles, AKParticle** circlesList)
    {
        // Adding new circle until it do not intersect with circles that already exists
        while (*actualCountOfParticles != countOfParticles) {
            AKParticle *circle = generateCircle();
            if (!::isCircleIntersectWithAlreadyExistingCircles(circle, actualCountOfParticles, circlesList)) {
                circle->velocity[0] = rand() % 10 - 1 / (rand() % 100);
                circle->velocity[1] = rand() % 10 - 1 / (rand() % 100);
                circle->mass = rand() % 10 - 1 / (rand() % 100);
                //circle->localTime = 0;
                circlesList[(*actualCountOfParticles)++] = circle;
                circle = nullptr;
                delete circle;
            }
        }
    }
    
    AKParticle* generateCircle()
    {
        /*
        double x = rand() % 550 - 1 / (rand() % 100);
        double y = rand() % 550 - 1 / (rand() % 100);
        double r = rand() % 20;
        if (r < 5) {
            r += 5;
        }
        AKSphere sphere;
        sphere.center = new double[2];
        sphere.center[0] = x;
        sphere.center[1] = y;
        sphere.radius = r;
        AKParticle *circle = new AKParticle();
        circle->sphere = sphere;
        return circle;
         */
        return nullptr; // TODO: Need to implement
    }
    
    bool isCircleIntersectWithAlreadyExistingCircles(AKParticle* circle, int* actualCountOfParticles, AKParticle** circlesList)
    {
        if (actualCountOfParticles == 0) {
            return false;
        }
        for (int i = 0; i < *actualCountOfParticles; i++) {
            AKParticle *secondCircle = circlesList[i];
            if (isCirclesIntersectIn2D(circle->sphere.center[0], circle->sphere.center[1], circle->sphere.radius, secondCircle->sphere.center[0], secondCircle->sphere.center[1], secondCircle->sphere.radius)) {
                return true;
            }
        }
        return false;
    }
    
    bool isCirclesIntersectIn2D(double x1, double y1, double r1, double x2, double y2, double r2)
    {
        double deltaX = x1 - x2, deltaY = y1 - y2;
        double deltaR = r1 + r2;
        return deltaX*deltaX + deltaY*deltaY <= deltaR * deltaR;
    }
    
    void convertDataInJSON(int dimension, int countOfPaticles, AKParticle** arrayOfCircles)
    {
        // ----------------------------------------------------------------------------
        // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
        {
            // error!
        }
        CFRelease(resourcesURL);
        
        chdir(path);
        std::cout << "Current Path: " << path << std::endl;
#endif
        // ----------------------------------------------------------------------------
        
        ofstream myfile;
        string fileName = "example_collision.txt";
        string slash = "/";
        fileName = path + slash + fileName;
        myfile.open (fileName);
        myfile << "Writing this to a file.\n";
        myfile.close();
        cout << "Data was wrote in file " << fileName << endl;
    }
};



#endif
