#ifndef AKPRIORITYQUEUE_H
#define AKPRIORITYQUEUE_H

#include <vector>
#include "AKEvent.h"

using namespace std;

class AKPriorityQueue
{
protected:
    unsigned int N = 0;
    bool less(unsigned int i, unsigned int j);
    void exch(unsigned int i, unsigned int j);
    void swim(unsigned int k);
    void sink(unsigned int k);
    void deleteElement(unsigned int k);
public:
    AKEvent **_arrayOfElements;
    AKPriorityQueue();
    AKPriorityQueue(unsigned int capacity);
    ~AKPriorityQueue();
    unsigned int  size();
    void insert(AKEvent* v);
    bool isEmpty();
    AKEvent* delMin();
    void deleteElement(AKEvent* key);
    void deleteElementForParticle(AKParticle*);
};

#endif // AKPRIORITYQUEUE_H
