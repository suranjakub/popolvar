#include <stdio.h>
#include <stdlib.h>

#include "minHeap.h"

MINHEAP minHeap;

int getLeftChildIndex(int parentIndex) {
    return 2 * parentIndex + 1;
}

int getRightChildIndex(int parentIndex) {
    return 2 * parentIndex + 2;
}

int getParentIndex(int childIndex) {
    return (childIndex - 1) / 2;
}

int hasLeftChild(int index) {
    int size = minHeap.size;
    return getLeftChildIndex(index) < size;
}

int hasRightChild(int index) {
    int size = minHeap.size;
    return getRightChildIndex(index) < size;
}

int hasParent(int index) {
    return getParentIndex(index) >= 0;
}

VERTEX* leftChild(int index) {
    return minHeap.array[getLeftChildIndex(index)];
}

VERTEX* rightChild(int index) {
    return minHeap.array[getRightChildIndex(index)];
}

VERTEX* parent(int index) {
    return minHeap.array[getParentIndex(index)];
}

void swap(int indexOne, int indexTwo) {
    VERTEX* temp = minHeap.array[indexOne];
    minHeap.array[indexOne] = minHeap.array[indexTwo];
    minHeap.array[indexTwo] = temp;
}

void checkCapacity() {
    if (minHeap.size == minHeap.capacity) {
        int newCapacity = minHeap.capacity;
        VERTEX** newArray = (VERTEX**)realloc(minHeap.array, newCapacity);
        minHeap.array = newArray;
        minHeap.capacity *= 2;
    }
}

VERTEX* heapGetMin() {
    if(minHeap.size == 0)
        return NULL;
    return minHeap.array[0];
}

VERTEX* heapExtractMin() {
    if(minHeap.size == 0)
        return NULL;

    VERTEX* vertex = minHeap.array[0];
    minHeap.array[0] = minHeap.array[minHeap.size-1];
    minHeap.size--;

    heapifyDown();

    return vertex;
}

void heapifyDown() {
    int index = 0;
    while(hasLeftChild(index)) {
        //find smaller item of both children
        int smallerIdx = getLeftChildIndex(index);
        if(hasLeftChild(index) && rightChild(index)->cost < leftChild(index)->cost)
            smallerIdx = getRightChildIndex(index);

        //if heap property is accomplished
        if(minHeap.array[index]->cost < minHeap.array[smallerIdx]->cost)
            return;
        else
            swap(index, smallerIdx);

        index = smallerIdx;
    }
}

void heapAdd(VERTEX* vertex) {
    checkCapacity();
    minHeap.array[minHeap.size] = vertex;
    minHeap.size++;

    heapifyUp();
}

void heapifyUp() {
    int index = minHeap.size - 1;

    while (hasParent(index) && parent(index)->cost > minHeap.array[index]->cost) {
        swap(getParentIndex(index), index);
        index = getParentIndex(index);
    }
}

void initializeHeap() {
    int capacity = minHeap.capacity = INIT_SIZE;
    minHeap.size = 0;
    minHeap.array = (VERTEX**)malloc(capacity * sizeof(VERTEX*));
    for (int i = 0; i < capacity; ++i) {
        minHeap.array[i] = NULL;
    }
    /*for (int i = 0; i < capacity; ++i)
        minHeap.array[i] = (VERTEX*)malloc(sizeof(VERTEX));*/
}

void freeHeap() {
    minHeap.capacity = minHeap.size = 0;
    free(minHeap.array);
    minHeap.array = NULL;
}

void printHeap() {
    int size = minHeap.size;
    for (int i = 0; i < size; ++i)
        printf("%d ", minHeap.array[i]->cost);
    printf("\n");
}

int isHeapEmpty() {
    return minHeap.size == 0;
}
