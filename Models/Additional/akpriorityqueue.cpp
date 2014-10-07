#include "akpriorityqueue.h"

static unsigned int const defaultSize = 100;

// CONSTRUCTORS
template <typename Key>
AKPriorityQueue<Key>::AKPriorityQueue()
{
    _arrayOfElements = new unsigned int[defaultSize];
}
template <typename Key>
AKPriorityQueue<Key>::AKPriorityQueue(unsigned int capacity)
{
    _arrayOfElements   = new unsigned int[capacity*capacity + 1];
}
template <typename Key>
AKPriorityQueue<Key>::~AKPriorityQueue()
{
    delete [] _arrayOfElements;
    _arrayOfElements = nullptr;
}
// PUBLIC METHODS
template <typename Key>
bool AKPriorityQueue<Key>::isEmpty()
{
    return N == 0;
}
template <typename Key>
unsigned int AKPriorityQueue<Key>::size()
{
    return N;
}
template <typename Key>
void AKPriorityQueue<Key>::insert(Key *key)
{
    _arrayOfElements[++N] = key;
    ((AKCompatibleToQueue*)_arrayOfElements[N])->index = N;
    swim(N);
}
template <typename Key>
Key* AKPriorityQueue<Key>::delMin()
{
    Key min = _arrayOfElements[1];
    exch(1, N--);
    _arrayOfElements[N+1] = nullptr;
    sink(1);
    return min;
}
template <typename Key>
void AKPriorityQueue<Key>::deleteElement(Key *key)
{
    unsigned int k = ((AKCompatibleToQueue*)key)->index;
    deleteElement(k);
}
// PRIVATE METHODS
template <typename Key>
bool AKPriorityQueue<Key>::less(unsigned int i, unsigned int j)
{
    return _arrayOfElements[i].compareTo(_arrayOfElements[j]) < 0;
}
template <typename Key>
void AKPriorityQueue<Key>::exch(unsigned int i, unsigned int j)
{
    // Workaround
    ((AKCompatibleToQueue*)_arrayOfElements[i])->index = j;
    ((AKCompatibleToQueue*)_arrayOfElements[j])->index = i;
    // Main flow
    Key *t = _arrayOfElements[i];
    _arrayOfElements[i] = _arrayOfElements[j];
    _arrayOfElements[j] = t;
}
template <typename Key>
void AKPriorityQueue<Key>::swim(unsigned int k)
{
    while (k > 1 && less(k/2, k))
    {
        exch(k/2, k);
        k = k/2;
    }
}
template <typename Key>
void AKPriorityQueue<Key>::sink(unsigned int k)
{
    while (2*k <= N)
    {
        unsigned int j = 2*k;
        if (j < N && less(j, j+1)) j++;
        if (!less(k, j)) break;
        exch(k, j);
        k = j;
    }
}
template <typename Key>
void AKPriorityQueue<Key>::deleteElement(unsigned int k)
{
    exch(k, N--);
    _arrayOfElements[N+1] = nullptr;
    sink(k);
}