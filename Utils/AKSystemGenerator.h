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


typedef enum {
    AKSpecialCoordinateXType = 1,
    AKSpecialCoordinateYType = 2,
    AKSpecialCoordinateZType = 3
} AKSpecialCoordinateType;

using namespace std;

bool isCircleIntersectWithAlreadyExistingCircles();
bool isCirclesIntersectIn2D(double x1, double y1, double r1, double x2, double y2, double r2);
bool isCirclesIntersectIn3D(double x1, double y1, double r1, double x2, double y2, double r2, double z1, double z2);

class AKSystemGenerator {

private:
    unsigned int dimension;
    int countOfParticles;
    int actualCountOfParticles;
    
    double *xArr, *yArr, *zArr, *rArr;
    double x, y, z, radius, vx, vy, vz, mass;
    
    AKSystemGenerator() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    AKSystemGenerator(AKSystemGenerator const&);              // Don't Implement
    void operator=(AKSystemGenerator const&); // Don't implement
    
    void generateXCoordinate()
    {
        if (SPECIAL_COORDINATE_TYPE != AKSpecialCoordinateXType) {
            x = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            while (x == 0 || x < COORDINATE_MIN_VALUE || x > COORDINATE_MAX_VALUE) {
                x = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            }
        } else {
            x = SPECIAL_COORDINATE;
        }
    }
    void generateYCoordinate()
    {
        if (SPECIAL_COORDINATE_TYPE != AKSpecialCoordinateYType) {
            y = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            while (y == 0 || y < COORDINATE_MIN_VALUE || y > COORDINATE_MAX_VALUE) {
                y = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            }
        } else {
            y = SPECIAL_COORDINATE;
        }
    }
    void generateZCoordinate()
    {
        if (SPECIAL_COORDINATE_TYPE != AKSpecialCoordinateZType) {
            z = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            while (z == 0 || z < COORDINATE_MIN_VALUE || z > COORDINATE_MAX_VALUE) {
                z = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
            }
        } else {
            z = SPECIAL_COORDINATE;
        }
    }
    void generateMass()
    {
        mass = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        while (mass == 0 || mass < MASS_MIN_VALUE || mass > MASS_MAX_VALUE) {
            mass = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        }
    }
    void generateRadius()
    {
        radius = rand() % 20;
        //bool unsuccessWithDimension = (is2DDimension) ? false : radius > z;
        while (radius > x || radius > y || radius < RADIUS_MIN_VALUE || radius > RADIUS_MAX_VALUE) {
            radius = rand() % 20;
        }
    }
    void generateSpeedX()
    {
        vx = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        while (vx == 0 || vx < VELOCITY_MIN_VALUE || vx > VELOCITY_MAX_VALUE) {
            vx = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        }
    }
    void generateSpeedY()
    {
        vy = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        while (vy == 0 || vy < VELOCITY_MIN_VALUE || vy > VELOCITY_MAX_VALUE) {
            vy = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        }
    }
    void generateSpeedZ()
    {
        vz = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        while (vz == 0 || vz < VELOCITY_MIN_VALUE || vz > VELOCITY_MAX_VALUE) {
            vz = rand() % (int)COORDINATE_MAX_VALUE - RADIUS_MAX_VALUE - 1;
        }
    }
    
    void generateVelocityAndMass()
    {
        generateSpeedX();
        generateSpeedY();
        if (ALLOW_Z_MOVING && !is2DDimension) {
            generateSpeedZ();
        }
        generateMass();
    }
    
    void generateCircle()
    {
        generateXCoordinate();
        generateYCoordinate();
        if (!is2DDimension) {
            generateZCoordinate();
        }
        generateRadius();
    }
    
    bool isCircleIntersectWithAlreadyExistingCircles()
    {
        if (actualCountOfParticles == 0) {
            return false;
        }
        for (int i = 0; i < actualCountOfParticles; i++) {
            if (is2DDimension) {
                if (isCirclesIntersectIn2D(x, y, radius, xArr[i], yArr[i], rArr[i])) {
                    return true;
                }
            } else {
                if(isCirclesIntersectIn2D(x, y, radius, xArr[i], yArr[i], rArr[i])) {//if (isCirclesIntersectIn3D(x, y, radius, xArr[i], yArr[i], rArr[i], z, zArr[i])) {
                    return true;
                }
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
    bool isCirclesIntersectIn3D(double x1, double y1, double r1, double x2, double y2, double r2, double z1, double z2)
    {
        // TODO: Need to implement
        return false;
    }
    void convertDataInJSON()
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
        // Adding new circle until it do not intersect with circles that already exists
        while (actualCountOfParticles != countOfParticles) {
            generateCircle();
            if (!isCircleIntersectWithAlreadyExistingCircles()) {
                generateVelocityAndMass();
                xArr[actualCountOfParticles] = x;
                yArr[actualCountOfParticles] = y;
                if (!is2DDimension) {
                    zArr[actualCountOfParticles] = z;
                }
                rArr[actualCountOfParticles] = radius;
                myfile << x << "," << y << ",";
                if (!is2DDimension) {
                    myfile << z << ",";
                }
                myfile << radius << "," << vx << "," << vy << ",";
                if (!is2DDimension) {
                    myfile << vz << ",";
                }
                myfile << mass;
                actualCountOfParticles++;
                if (actualCountOfParticles < countOfParticles) {
                    myfile << "|";
                }
            }
        }
        // myfile << "Writing this to a file.\n";
        
        myfile.close();
        cout << "Data was wrote in file " << fileName << endl;
    }

    
public:
    double MASS_MIN_VALUE, MASS_MAX_VALUE, VELOCITY_MIN_VALUE, VELOCITY_MAX_VALUE,
        COORDINATE_MIN_VALUE, COORDINATE_MAX_VALUE, RADIUS_MIN_VALUE, RADIUS_MAX_VALUE, SPECIAL_COORDINATE;
    bool is2DDimension, ALLOW_Z_MOVING;
    int SPECIAL_COORDINATE_TYPE = -1;
    void generateParticles(unsigned int count){
        cout << "UTILITY READY TO INSTALL SYSTEM'S ARGUMENTS"<< endl;
        cout << "Please enter the " << endl;
#pragma mark - READ DIMENSION
        cout << "Dimension: ";
        dimension = (is2DDimension) ? 2 : 3;
        cout << "Dimension = " << dimension << endl;
#pragma mark - READ COUNT OF PARTICLES
        cout << "Count of particles: ";
        string inputCountOfParticles = "";
        countOfParticles = count;
        xArr = new double[countOfParticles];
        yArr = new double[countOfParticles];
        zArr = new double[countOfParticles];
        rArr = new double[countOfParticles];
        cout << "Count of particles = " << countOfParticles << endl;
#pragma mark - FILLING ARRAY OF PARTICLES
        if ((SPECIAL_COORDINATE < COORDINATE_MIN_VALUE || SPECIAL_COORDINATE > COORDINATE_MAX_VALUE) && is2DDimension) {
            SPECIAL_COORDINATE = (COORDINATE_MIN_VALUE + COORDINATE_MAX_VALUE) / 2;
        }
        convertDataInJSON();
    }
    static AKSystemGenerator& getInstance()
    {
        static AKSystemGenerator    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
};



#endif
