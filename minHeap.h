#ifndef POPOLVAR_MINHEAP_H
#define POPOLVAR_MINHEAP_H

#define NEIGHBORS 4

typedef struct vertex {
    int index, weight, cost, prevShortest, visited;
    char type;
    struct vertex* next[NEIGHBORS];
} VERTEX;

typedef struct minHeap {
    int size, capacity;
    VERTEX** array;
} MINHEAP;

int getLeftChildIndex(int parentIndex);
int getRightChildIndex(int parentIndex);
int getParentIndex(int childIndex);
int hasLeftChild(int index);
int hasRightChild(int index);
int hasParent(int index);
VERTEX* leftChild(int index);
VERTEX* rightChild(int index);
VERTEX* parent(int index);
void swap(int indexOne, int indexTwo);
void checkCapacity();
VERTEX* heapGetMin();
VERTEX* heapExtractMin();
void heapifyDown();
void heapAdd(VERTEX* vertex);
void heapifyUp();
void initializeHeap();
void printHeap();
int isHeapEmpty();

#endif //POPOLVAR_MINHEAP_H
