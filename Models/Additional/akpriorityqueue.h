#ifndef AKPRIORITYQUEUE_H
#define AKPRIORITYQUEUE_H

#include <vector>

using namespace std;

template <typename Key>
class AKPriorityQueue
{
    struct AKNode {
        Key *key;
        AKNode *parent;
        AKNode *leftChild;
        AKNode *rightChild;
    };
private:
    // Tree implementation
    AKNode *root;
    AKNode *last;
    // Vector implementation
    std::vector<Key> *vectorOfElements;
    // Array implementation
    int N = 0;
    Key* arrayOfElements[];
    bool less(int i, int j);
    void exch(int i, int j);
    void swim(int k);
    void sink(int k);
public:
    AKPriorityQueue();
    void    insert(Key *v);
    int     size();
    bool    isEmpty();
    Key*    delMax();
};

#endif // AKPRIORITYQUEUE_H
