#ifndef AKPRIORITYQUEUE_H
#define AKPRIORITYQUEUE_H

#include <vector>
#include "akevent.h"

using namespace std;

class AKPriorityQueue
{
private:
public:
    unsigned int N = 0;
    AKEvent **_arrayOfElements;
    bool less(unsigned int i, unsigned int j);
    void exch(unsigned int i, unsigned int j);
    void swim(unsigned int k);
    void sink(unsigned int k);
    void deleteElement(unsigned int k);
    // -- -- //
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
