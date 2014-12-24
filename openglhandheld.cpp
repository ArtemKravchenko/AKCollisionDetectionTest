//
//  openglhandheld.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 10/31/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include "openglhandheld.h"
#include "AKSphere.h"
#include <string>
#include "AKSystemGenerator.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <istream>
#include <sstream>
#include <fstream>
#include "AKShapeVisualizer.h"

using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// px, py, radius, vx, vy, mass
vector<string> arrayOfParticles()
{
    return {
        "210,400,15,5,-5,2",
        "584.5,15.5,15,-5,5,2",
        "400,200,15,-5,5,2",
        "15.5,584.5,15,5,-5,2",
        "101.182,200,15,2.34774,-6.66994,2",
        "400,500,15,-5,5,2",
        "20,200,15,-5,5,2",
        "45.5,504.5,15,5,-5,2",
        "50,50,15,5,-5,2",
        "80,80,15,-5,5,2",
        "110,150,15,-5,5,2",
        "300,300,15,5,-5,2",
        "350,350,15,10,6.66994,2",
        "400,400,15,-5,5,2",
        "500,100,15,-5,5,2",
        "550,550,15,5,-5,2",
    };
}

vector<string> readArrayOfParticles() {
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
    string slash = "/";
    string fileName = "example_collision.txt";
    fileName = path + slash + fileName;
    string line;
    vector<string> *elems = new vector<string>();;
    ifstream myfile (fileName);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            split(line, '|', *elems);
            cout << line << '\n';
        }
        myfile.close();
    }
    return *elems;
}
void AKOpenGLHandheld::initSystemData()
{
    AKSystemGenerator::getInstance().is2DDimension = is2DDimension;
    AKSystemGenerator::getInstance().COORDINATE_MIN_VALUE = 16;
    AKSystemGenerator::getInstance().COORDINATE_MAX_VALUE = 300;//584;
    AKSystemGenerator::getInstance().MASS_MIN_VALUE = 2;
    AKSystemGenerator::getInstance().MASS_MAX_VALUE = 20;
    AKSystemGenerator::getInstance().VELOCITY_MIN_VALUE = 2;
    AKSystemGenerator::getInstance().VELOCITY_MAX_VALUE = 10;
    AKSystemGenerator::getInstance().RADIUS_MIN_VALUE = 2;
    AKSystemGenerator::getInstance().RADIUS_MAX_VALUE = 3;
    if (!is2DDimension) {
        AKSystemGenerator::getInstance().SPECIAL_COORDINATE = 5;
        AKSystemGenerator::getInstance().ALLOW_Z_MOVING = true;
    }
    AKSystemGenerator::getInstance().SPECIAL_COORDINATE_TYPE = AKSpecialCoordinateZType;
    AKSystemGenerator::getInstance().generateParticles(150);
}

void AKOpenGLHandheld::setUpModels()
{
    initSystemData();
    if (is2DDimension) {
        _collisionDetectionLogic = new AKCollisionDetectionDiscreteTimeLogic2D();
    } else {
        _collisionDetectionLogic = new AKCollisionDetectionDiscreteTimeLogic3D();
    }
    
    AKBox *bounds = new AKBox((is2DDimension) ? 2 :3);
    bounds->center[0] = DISPLAY_WIDTH / 4; bounds->center[1] = DISPLAY_WIDTH / 4;
    if (!is2DDimension) {
        bounds->center[2] = DISPLAY_DEPTH / 4;
    }
    bounds->radius[0] = DISPLAY_HEIGHT / 4; bounds->radius[1] = DISPLAY_HEIGHT / 4;
    if (!is2DDimension) {
        bounds->radius[2] = DISPLAY_DEPTH / 4;
    }
    
    _collisionDetectionLogic->setBound(bounds, originX, originY, originZ, 1, 1, 1);
    vector<string> particlesData = readArrayOfParticles(); //
    AKParticlesList *_particleList = new AKParticlesList();
    this->particlesArray = new vector<AKParticle*>();
    vector<string> *elems;
    AKParticle* particle;
    int index = 0;
    for (int i = 0; i < particlesData.size(); i++) {
        elems = new vector<string>();
        particle = new AKParticle((is2DDimension) ? 2 : 3);
        split(particlesData.at(i), ',', *elems);
        particle->sphere.center[0] = std::stod(elems->at(index++));
        particle->sphere.center[1] = std::stod(elems->at(index++));
        if (is2DDimension) {
            particle->sphere.center[2] = 10;
        } else {
            particle->sphere.center[2] = std::stod(elems->at(index++));
        }
        
        particle->sphere.radius = std::stod(elems->at(index++));
        particle->velocity[0] = std::stod(elems->at(index++));
        particle->velocity[1] = std::stod(elems->at(index++));
        if (!is2DDimension) {
            particle->velocity[2] = std::stod(elems->at(index++));
        }
        particle->mass = std::stod(elems->at(index++));
        this->particlesArray->push_back(particle);
        _particleList->push_back(particle);
        index = 0;
    }
    
    _collisionDetectionLogic->setParticlesList(_particleList);
    _collisionDetectionLogic->fillEventsInQueue();
}

#pragma mark - OpenGLInit delegate

void AKOpenGLHandheld::redrawObjects()
{
    _collisionDetectionLogic->drawCells();
    int particlesCount = this->particlesArray->size() & INT_MAX;
    for (int i = 0; i < particlesCount; i++) {
        AKParticle *particle = particlesArray->at(i);
        AKShapeVisualizer::getInstance().visualizeParticle(particle, originX, originY, originZ);
    }
}

void AKOpenGLHandheld::handleTimerChanges(double time)
{
    _collisionDetectionLogic->updateEventQueueInTime(time);
}