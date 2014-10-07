#ifndef AKPRIORITYQUEUE_H
#define AKPRIORITYQUEUE_H

#include <vector>

using namespace std;

template <typename Key>
class AKPriorityQueue
{
private:
    unsigned int N = 0;
    Key* _arrayOfElements[];
    bool less(unsigned int i, unsigned int j);
    void exch(unsigned int i, unsigned int j);
    void swim(unsigned int k);
    void sink(unsigned int k);
    void deleteElement(unsigned int k);
public:
    AKPriorityQueue();
    AKPriorityQueue(unsigned int capacity);
    ~AKPriorityQueue();
    unsigned int  size();
    void insert(Key *v);
    bool isEmpty();
    Key* delMin();
    void deleteElement(Key *key);
};

struct AKCompatibleToQueue {
    unsigned int index;
};

#endif // AKPRIORITYQUEUE_H
