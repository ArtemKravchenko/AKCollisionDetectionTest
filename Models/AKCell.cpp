//
//  AKCell.cpp
//  AKCollisionDetectionTest
//
//  Created by Artem Kravchenko on 9/22/14.
//  Copyright (c) 2014 ShowTime. All rights reserved.
//

#include <stdio.h>
#include "AKCell.h"

// Constructors and Destructors
AKCell::AKCell()
{
    _bounds = new AKBox();
    _insideParticles = new AKParticlesList();
}
AKCell::AKCell(AKBox* bounds)
{
    _bounds = new AKBox(*bounds);

}
AKCell::~AKCell()
{
    delete _bounds;
    delete _insideParticles;
}
// Public functions
void AKCell::addNeighbors(int neighbors[4])
{
    _insideParticles = new AKParticlesList();
    _neighbors[0] = neighbors[0];
    _neighbors[1] = neighbors[1];
    _neighbors[2] = neighbors[2];
    _neighbors[3] = neighbors[3];
}
void AKCell::addParticle(AKParticle* particle)
{
    _insideParticles->push_back(particle);
}
AKBox const * AKCell::getBounds() const
{
    return _bounds;
}
AKParticlesList const * AKCell::getInsideParticles() const
{
    return _insideParticles;
}
int const * AKCell::getNeighbors()
{
    return _neighbors;
}