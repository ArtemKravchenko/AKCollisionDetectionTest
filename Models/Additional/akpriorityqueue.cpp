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
    N = 0;
}
AKPriorityQueue::~AKPriorityQueue()
{
    delete [] _arrayOfElements;
    _arrayOfElements = nullptr;
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
    _arrayOfElements[++N] = key;
    _arrayOfElements[N]->index = N;
    swim(N);
}
AKEvent* AKPriorityQueue::delMin()
{
    if (N == 0) {
        return nullptr;
    }
    AKEvent *min = _arrayOfElements[1];
    exch(1, N--);
    _arrayOfElements[N+1] = nullptr;
    sink(1);
    return min;
}
void AKPriorityQueue::deleteElement(AKEvent *key)
{
    unsigned int k = key->index;
    deleteElement(k);
}
void AKPriorityQueue::deleteElementForParticle(AKParticle* particle)
{
    AKEvent *event;
    for (int i = 0; i < N; i++) {
        event = _arrayOfElements[i];
        // TODO: Need to clearly recheck
        if ((event->firstParticle) == particle ||
            (event->secondParticle) == particle) {
            deleteElement(event);
            N--;
            i++;
        }
    }
}
// PRIVATE METHODS
bool AKPriorityQueue::less(unsigned int i, unsigned int j)
{
    return _arrayOfElements[i]->timeToEvent < _arrayOfElements[j]->timeToEvent;
}
void AKPriorityQueue::exch(unsigned int i, unsigned int j)
{
    // Workaround
    _arrayOfElements[i]->index = j;
    _arrayOfElements[j]->index = i;
    // Main flow
    AKEvent* t = _arrayOfElements[i];
    _arrayOfElements[i] = _arrayOfElements[j];
    _arrayOfElements[j] = t;
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
void AKPriorityQueue::deleteElement(unsigned int k)
{
    exch(k, N--);
    _arrayOfElements[N+1] = nullptr;
    sink(k);
}