#ifndef AKPRIORITYQUEUE_H
#define AKPRIORITYQUEUE_H

#include <vector>
#include "AKEvent.h"

using namespace std;


class AKPriorityQueueDelegate {
    
public:
    virtual void recomputeEvent(AKEvent*) = 0;
    virtual int  countUnreachableCells() = 0;
};

class AKPriorityQueue
{
protected:
    bool less(unsigned int i, unsigned int j);
    void exch(unsigned int i, unsigned int j);
    void swim(unsigned int k);
    void sink(unsigned int k);
    void deleteElement(unsigned int k);
    void printQueue();
    int  nearestPowerOfTwo(unsigned int);
    AKEvent** getCurrentArray();
    bool _useReserveArray;
    AKEvent **_reserveArray;
public:
    AKEvent **_arrayOfElements;
    AKPriorityQueueDelegate *delegate;
    unsigned int N = 0;
    AKPriorityQueue();
    AKPriorityQueue(unsigned int capacity);
    ~AKPriorityQueue();
    unsigned int  size();
    void insert(AKEvent* v);
    bool isEmpty();
    AKEvent* delMin();
    AKEvent* secondEvent();
    void deleteElement(AKEvent* key);
    void deleteElementForParticle(AKParticle*);
    void deleteElementForParticle(AKParticle*, int*, int);   // Remove only events with another particles only from unreachable cells
    void updateEventForParticle(AKParticle*);
    bool particleBelongsUnrechableCells(AKParticle*, int*, int);
};

#endif // AKPRIORITYQUEUE_H
