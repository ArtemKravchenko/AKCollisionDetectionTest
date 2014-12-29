#include "akpriorityqueue.h"

static unsigned int const defaultSize = 100;

// CONSTRUCTORS
AKPriorityQueue::AKPriorityQueue()
{
    _arrayOfElements = new AKEvent*[defaultSize];
}
AKPriorityQueue::AKPriorityQueue(unsigned int capacity)
{
    _arrayOfElements = new AKEvent*[capacity*capacity + 1];
    _reserveArray = new AKEvent*[capacity*capacity + 1];
    N = 0;
}
AKPriorityQueue::~AKPriorityQueue()
{
    delete [] _arrayOfElements;
    _arrayOfElements = nullptr;
    delete [] _reserveArray;
    _reserveArray = nullptr;
}
// PUBLIC METHODS
bool AKPriorityQueue::isEmpty()
{
    return N == 0;
}
unsigned int AKPriorityQueue::size()
{
    return N;
}
void AKPriorityQueue::insert(AKEvent *key)
{
    getCurrentArray()[++N] = key; //_arrayOfElements[++N] = key;
    getCurrentArray()[N]->index = N;//_arrayOfElements[N]->index = N;
    swim(N);
}
AKEvent* AKPriorityQueue::delMin()
{
    if (N == 0) {
        return nullptr;
    }
    /*
    AKEvent *min = _arrayOfElements[1];
    exch(1, N--);
    
    //delete _arrayOfElements[N+1];
    //_arrayOfElements[N+1] = nullptr;
    
    sink(1);
    return min;
    */
    return getCurrentArray()[1]; //new AKEvent(*_arrayOfElements[1]);
}
AKEvent* AKPriorityQueue::secondEvent()
{
    return getCurrentArray()[2]; //_arrayOfElements[2];
}
void AKPriorityQueue::updateEventForParticle(AKParticle* particle)
{
    unsigned int k;
    AKEvent *event;
    vector<AKEvent*> needToRecompute = vector<AKEvent*>();
    for (int i = 1; i < N+1; i++) {
        event = getCurrentArray()[i]; //event = _arrayOfElements[i];
        if (event->secondParticle != NULL) {
            if (event->firstParticle == particle || event->secondParticle == particle) {
                needToRecompute.push_back(event);
                continue;
            }
        } else {
            if (event->firstParticle == particle) {
                needToRecompute.push_back(event);
                continue;
            }
        }
    }
    for (int i = 0; i < needToRecompute.size(); i++) {
        event = needToRecompute.at(i);
        delegate->recomputeEvent(event);
        k = event->index;
        swim(k);
        sink(k);
    }
}
bool AKPriorityQueue::particleBelongsUnrechableCells(AKParticle* particle, int* cells, int transitionIndex)
{
    int cellsCount = delegate->countUnreachableCells();
    for (int i = cellsCount*transitionIndex; i < cellsCount; i++) {
        if (particle->cellIndex == cells[i]) {
            return true;
        }
    }
    return false;
}
// PRIVATE METHODS
bool AKPriorityQueue::less(unsigned int i, unsigned int j)
{
    return getCurrentArray()[i]->timeToEvent < getCurrentArray()[j]->timeToEvent;;//_arrayOfElements[i]->timeToEvent < _arrayOfElements[j]->timeToEvent;
}
void AKPriorityQueue::exch(unsigned int i, unsigned int j)
{
    // Workaround
    getCurrentArray()[i]->index = j;//_arrayOfElements[i]->index = j;
    getCurrentArray()[j]->index = i; //_arrayOfElements[j]->index = i;
    // Main flow
    AKEvent* t = getCurrentArray()[i]; //AKEvent* t = _arrayOfElements[i];
    getCurrentArray()[i] = getCurrentArray()[j];//_arrayOfElements[i] = _arrayOfElements[j];
    getCurrentArray()[j] = t;//_arrayOfElements[j] = t;
}
void AKPriorityQueue::swim(unsigned int k)
{
    while (k > 1 && less(k, k/2))
    {
        exch(k/2, k);
        k = k/2;
    }
}
void AKPriorityQueue::sink(unsigned int k)
{
    while (2*k <= N)
    {
        unsigned int j = 2*k;
        if (j < N && less(j+1, j)) j++;
        if (!less(j, k)) break;
        exch(k, j);
        k = j;
    }
}
#pragma mark - DELETE ELEMENT METHODS
void AKPriorityQueue::deleteElement(AKEvent *key)
{
    unsigned int k = key->index;
    deleteElement(k);
}
void AKPriorityQueue::deleteElementForParticle(AKParticle* particle)
{
    AKEvent *event;
    bool isEventHaveTheSamePartilce = false;
    for (int i = 1; i < N+1; i++) {
        event = getCurrentArray()[i]; //event = _arrayOfElements[i];
        if (event->firstParticle != NULL) {
            isEventHaveTheSamePartilce = (event->firstParticle == particle);
        }
        if (!isEventHaveTheSamePartilce && event->secondParticle != NULL) {
            isEventHaveTheSamePartilce = (event->secondParticle == particle);
        }
        if (isEventHaveTheSamePartilce) {
            deleteElement(event);
            i--;
        }
    }
}
void AKPriorityQueue::deleteElementForParticle(AKParticle* particle, int* unreachableNeighbors, int transitionIndex)
{
    AKEvent *event;
    vector<AKEvent*> needToDelete = vector<AKEvent*>();
    for (int i = 1; i < N+1; i++) {
        event = getCurrentArray()[i]; //event = _arrayOfElements[i];
        if (event->secondParticle != NULL) {
            if ((event->firstParticle == particle && particleBelongsUnrechableCells(event->secondParticle, unreachableNeighbors, transitionIndex)) ||
                (event->secondParticle == particle && particleBelongsUnrechableCells(event->firstParticle, unreachableNeighbors, transitionIndex))) {
                needToDelete.push_back(event);
                //i--;
            }
        } else {
            if (event->firstParticle != NULL) {
                if (event->firstParticle == particle) {
                    needToDelete.push_back(event);
                    //i--;
                }
            }
        }
    }
    for (int i = 0; i < needToDelete.size(); i++) {
        event = needToDelete.at(i);
        deleteElement(event);
    }
}
void AKPriorityQueue::deleteElement(unsigned int k)
{
    exch(k, N--);
    delete getCurrentArray()[N+1];//_arrayOfElements[N+1];
    getCurrentArray()[N+1] = nullptr;//_arrayOfElements[N+1] = nullptr;
    if (k < N) {
        swim(k);
        sink(k);
    }
}
#pragma mark - Methods that need for new technic of updating priority queue
int  AKPriorityQueue::nearestPowerOfTwo(unsigned int x)
{
    unsigned int powersOfTwo[32] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608, 16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};
    unsigned int index = 0;
    while (x > powersOfTwo[index]) {
        index++;
    }
    return powersOfTwo[index-1];
}
AKEvent** AKPriorityQueue::getCurrentArray()
{
    return (_useReserveArray) ? _reserveArray : _arrayOfElements;
}