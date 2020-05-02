#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "minHeap.h"

//#define NEIGHBORS 4

typedef struct map {
    int n, m, tDwake;
    char** ptr;
} MAP;

/*typedef struct vertex {
    int index, weight, cost, prevShortest;
    char type;
    struct vertex* next[NEIGHBORS];
} VERTEX;*/

MAP loadMap(MAP map)
{
    int n, m, tDwake;
    //handle input
    FILE* input = fopen("input.txt","r");
    fscanf(input, "%d %d %d", &n, &m, &tDwake);

    //create map, n rows, m cols
    map.ptr = (char**)calloc(n, sizeof(char*));

    for (int i = 0; i < n; ++i)
        map.ptr[i] = (char*)calloc(m, sizeof(char));

    map.n = n;
    map.m = m;
    map.tDwake = tDwake;

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            char symbol;
            fscanf(input, "%c", &symbol);
            if (symbol <= 'A' || symbol >= 'z')
                fscanf(input, "%c", &symbol);
            map.ptr[r][c] = symbol;
        }
    }

    return map;
}

void printMap(MAP map)
{
    for (int i = 0; i < map.n; ++i) {
        for (int j = 0; j < map.m; ++j) {
            printf("%c ", map.ptr[i][j]);
        }
        printf("\n");
    }
}

VERTEX** transformToGraph(MAP map, VERTEX** vertexList)
{
    //transform map to adjacency list graph
    for (int i = 0; i < map.n; ++i) {
        for (int j = 0; j < map.m; ++j) {
            char symbol = map.ptr[i][j];
            int cols = map.m;
            int index = (i * cols) + j;

            vertexList[index]->index = index;

            if (symbol == 'C' || symbol == 'D' || symbol == 'P')
                vertexList[index]->weight = 1;
            else if (symbol == 'H')
                vertexList[index]->weight = 2;

            vertexList[index]->type = symbol;

            if (index == 0)
                vertexList[index]->cost = 1;
            else
                vertexList[index]->cost = INT_MAX;

            vertexList[index]->prevShortest = -1;
            for (int k = 0; k < NEIGHBORS; ++k)
                vertexList[index]->next[k] = NULL;

            //Add neighbors
            //right
            if (j + 1 < map.m && symbol != 'N' ) {
                if (map.ptr[i][j + 1] == 'C' || map.ptr[i][j + 1] == 'H' || map.ptr[i][j + 1] == 'P' || map.ptr[i][j + 1] == 'D')
                    vertexList[index]->next[0] = vertexList[index + 1];
            }
            //bottom
            if (i + 1 < map.n && symbol != 'N' ) {
                if (map.ptr[i + 1][j] == 'C' || map.ptr[i + 1][j] == 'H' || map.ptr[i + 1][j] == 'P' || map.ptr[i + 1][j] == 'D')
                    vertexList[index]->next[1] = vertexList[index + cols];
            }
            //left
            if (j - 1 >= 0 && symbol != 'N' ) {
                if (map.ptr[i][j - 1] == 'C' || map.ptr[i][j - 1] == 'H' || map.ptr[i][j - 1] == 'P' || map.ptr[i][j - 1] == 'D')
                    vertexList[index]->next[2] = vertexList[index - 1];
            }
            //top
            if (i - 1 >= 0 && symbol != 'N' ) {
                if (map.ptr[i - 1][j] == 'C' || map.ptr[i - 1][j] == 'H' || map.ptr[i - 1][j] == 'P' || map.ptr[i - 1][j] == 'D')
                    vertexList[index]->next[3] = vertexList[index - cols];
            }
        }
    }

    return vertexList;
}

VERTEX** createVertexList(VERTEX** vertexList, int nOfVerteces) {
    vertexList = (VERTEX**)malloc(nOfVerteces * sizeof(VERTEX*));

    for (int i = 0; i < nOfVerteces; ++i) {
        vertexList[i] = (VERTEX*)malloc(sizeof(VERTEX));
        vertexList[i]->weight = vertexList[i]->cost = vertexList[i]->visited = 0;
        for (int j = 0; j < NEIGHBORS; ++j)
            vertexList[i]->next[j] = NULL;
    }

    return vertexList;
}

void printVertexList(VERTEX** vertexList, int nOfVerteces) {
    printf("\n");
    for (int i = 0; i < nOfVerteces; ++i) {
        printf("%d%c: ", vertexList[i]->index, vertexList[i]->type);
        for (int j = 0; j < NEIGHBORS; ++j) {
            if(vertexList[i]->next[j] != NULL)
                printf("%d%c ", vertexList[i]->next[j]->index, vertexList[i]->next[j]->type);
        }
        printf("\n");
    }
}

void dijkstra(VERTEX **vertexList, int startingIndex, int nOfVerteces) {
    initializeHeap();
    vertexList[startingIndex]->cost = 1;
    heapAdd(vertexList[startingIndex]);
    for (int i = 0; i < nOfVerteces; ++i) {
        if(i != startingIndex)
            vertexList[i]->cost = INT_MAX;
    }

    while(!isHeapEmpty()) {
        VERTEX* currentVertex = heapGetMin();

        for (int i = 0; i < NEIGHBORS; ++i) {
            VERTEX* currentNeighbor = currentVertex->next[i];
            //if neighbor exist
            if (currentNeighbor != NULL) {
                //and wasn't visited then examine this
                if (currentNeighbor->visited == 0) {
                    int costFromStart = currentVertex->cost + currentNeighbor->weight;
                    if (costFromStart < currentNeighbor->cost) {
                        //updated smallest cost from start
                        currentNeighbor->cost = costFromStart;
                        //set new previous point which leads to this smaller cost
                        currentNeighbor->prevShortest = currentVertex->index;
                        heapAdd(currentNeighbor);
                    }
                }
            }
        }
        //we examined current vertex, we can throw it away from heap
        heapExtractMin();
        currentVertex->visited = 1;
    }

    freeHeap();
    printf("\nDijkstra done!\n");
}

void reconstructPathFromTo(int start, int index, MAP map, VERTEX **vertexList, int max) {
    //if vertices aren't in range
    if(start >= max || index >= max) {
        printf("Vertices aren't in range!");
        return;
    }

    int i, end, previousIndex, pathCost = 0;
    int* path = (int*)malloc(30 * sizeof(int));
    if(path == NULL) {
        printf("Can't allocate path array!");
        return;
    }
    //remeber end index
    end = index;

    //insert end index to path
    path[0] = end;
    i = 1;

    //reconstruct path from end to start
    while(index != start) {
        previousIndex = vertexList[index]->prevShortest;
        path[i++] = previousIndex;
        index = previousIndex;
    }
    i--;

    printf("\nPath from %d to %d:\n", start, end);
    for (int j = i; j >= 0; --j) {
        int x = path[j] % map.m;
        int y = path[j] / map.n;
        //printf("%d ", path[j]);
        printf("%d %d\n", x, y);
    }
    printf("Cost: %d", vertexList[end]->cost);

    free(path);
}

int main()
{
    //map handling
    MAP map;
    map = loadMap(map);
    printMap(map);

    //working with vertices
    int nOfVertices = map.n * map.m;
    VERTEX **vertexList = NULL;
    vertexList = createVertexList(vertexList, nOfVertices);
    vertexList = transformToGraph(map, vertexList);
    printVertexList(vertexList, nOfVertices);

    dijkstra(vertexList, 0, nOfVertices);
    reconstructPathFromTo(0, 20, map, vertexList, nOfVertices);

    free(vertexList);

    VERTEX **vertexList2 = NULL;
    vertexList2 = createVertexList(vertexList2, nOfVertices);
    vertexList2 = transformToGraph(map, vertexList2);
    dijkstra(vertexList2, 20, nOfVertices);
    reconstructPathFromTo(20, 50, map, vertexList2, nOfVertices);

    /*initializeHeap();
    dijkstra(vertexList, 0, nOfVertices);
    reconstructPathFromTo(20, 50, map, vertexList, nOfVertices);*/

    //printf("\nDragon previous: %d", vertexList[8]->prevShortest);

    /*
    //working with minHeap
    printf("\nminHeap:\n");
    initializeHeap();
    printf("is empty?: %d\n", isHeapEmpty());
    vertexList[0]->cost = 10;
    vertexList[1]->cost = 13;
    vertexList[2]->cost = 7;
    heapAdd(vertexList[0]);
    heapAdd(vertexList[1]);
    heapAdd(vertexList[2]);
    printHeap();

    printf("\n");
    heapExtractMin();
    printHeap();
    printf("\nis empty?: %d\n", isHeapEmpty());
    */

    return 0;
}
