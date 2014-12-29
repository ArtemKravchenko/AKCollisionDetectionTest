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

vector<string> specialElements() {
    vector<string> *elems = new vector<string>();
    elems->push_back("188.9761,130.768979,7,-6.1514561,9.32459503,20");
    return *elems;
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
    vector<string> *elems = new vector<string>();
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
    AKSystemGenerator::getInstance().RADIUS_MIN_VALUE = 3;
    AKSystemGenerator::getInstance().RADIUS_MAX_VALUE = 4;
    AKSystemGenerator::getInstance().COORDINATE_MIN_VALUE = 2 * AKSystemGenerator::getInstance().RADIUS_MAX_VALUE;
    AKSystemGenerator::getInstance().COORDINATE_MAX_VALUE = min(min(_displayWidth, _displayHeight), _displayDepth) - AKSystemGenerator::getInstance().RADIUS_MAX_VALUE*2;
    AKSystemGenerator::getInstance().MASS_MIN_VALUE = 10;
    AKSystemGenerator::getInstance().MASS_MAX_VALUE = 20;
    AKSystemGenerator::getInstance().VELOCITY_MIN_VALUE = 2;
    AKSystemGenerator::getInstance().VELOCITY_MAX_VALUE = 10;
    if (!is2DDimension) {
        AKSystemGenerator::getInstance().ALLOW_Z_MOVING = false;
    }
    AKSystemGenerator::getInstance().generateParticles(64);
}

void AKOpenGLHandheld::setUpModels()
{
    if (!_displayDepthWasSet) _displayDepth = 600;
    if (!_displayWidthWasSet) _displayWidth = 600;
    if (!_displayHeightWasSet) _displayHeight = 600;
    initSystemData();
    if (is2DDimension) {
        _collisionDetectionLogic = new AKCollisionDetectionEventDrivenLogic2D();// AKCollisionDetectionDiscreteTimeLogic2D();
    } else {
        _collisionDetectionLogic = new AKCollisionDetectionEventDrivenLogic3D();// AKCollisionDetectionDiscreteTimeLogic3D();
    }
    
    AKBox *bounds = new AKBox((is2DDimension) ? 2 :3);
    bounds->center[0] = _displayWidth / 2; bounds->center[1] = _displayHeight / 2;
    if (!is2DDimension) {
        bounds->center[2] = _displayDepth / 2;
    }
    bounds->radius[0] = _displayWidth / 2; bounds->radius[1] = _displayHeight / 2;
    if (!is2DDimension) {
        bounds->radius[2] = _displayDepth / 2;
    }
    
    _collisionDetectionLogic->setBound(bounds, originX, originY, originZ, 8, 8, 1);
    vector<string> particlesData = readArrayOfParticles();
    AKParticlesList *_particleList = new AKParticlesList();
    this->_particlesArray = new vector<AKParticle*>();
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
        this->_particlesArray->push_back(particle);
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
    int particlesCount = this->_particlesArray->size() & INT_MAX;
    for (int i = 0; i < particlesCount; i++) {
        AKParticle *particle = _particlesArray->at(i);
        AKShapeVisualizer::getInstance().visualizeParticle(particle, originX, originY, originZ);
    }
}
void AKOpenGLHandheld::handleTimerChanges(double time)
{
    _collisionDetectionLogic->updateEventQueueInTime(time);
}

bool AKOpenGLHandheld::getIs2Dimension()
{
    return is2DDimension;
}
float AKOpenGLHandheld::getDisplayWidth()
{
    return _displayWidth;
}
void AKOpenGLHandheld::setDisplayWidth(float value)
{
    _displayWidthWasSet = true;
    _displayWidth = value;
}
float AKOpenGLHandheld::getDisplayHeight()
{
    return _displayHeight;
}
void AKOpenGLHandheld::setDisplayHeight(float value)
{
    _displayHeightWasSet = true;
    _displayHeight = value;
}
float AKOpenGLHandheld::getDisplayDepth()
{
    return _displayDepth;
}
void AKOpenGLHandheld::setDisplayDepth(float value)
{
    _displayDepthWasSet = true;
    _displayDepth = value;
}