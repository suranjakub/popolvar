#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NEIGHBORS 4

typedef struct map {
    int n, m, tDwake;
    char** ptr;
} MAP;

typedef struct vertex {
    int index, weight, cost, prevShortest;
    char type;
    struct vertex* next[NEIGHBORS];
} VERTEX;

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

            if (symbol == 'C')
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

            //add neighbors
            //right
            if (j + 1 < map.m && symbol != 'N' ) {
                if (map.ptr[i][j + 1] == 'C' || map.ptr[i][j + 1] == 'H')
                    vertexList[index]->next[0] = vertexList[index + 1];
            }
            //bottom
            if (i + 1 < map.n && symbol != 'N' ) {
                if (map.ptr[i + 1][j] == 'C' || map.ptr[i + 1][j] == 'H')
                    vertexList[index]->next[1] = vertexList[index + cols];
            }
            //left
            if (j - 1 > 0 && symbol != 'N' ) {
                if (map.ptr[i][j - 1] == 'C' || map.ptr[i][j - 1] == 'H')
                    vertexList[index]->next[2] = vertexList[index - 1];
            }
            //top
            if ( i - 1 > 0 && symbol != 'N' ) {
                if (map.ptr[i - 1][j] == 'C' || map.ptr[i - 1][j] == 'H')
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
        vertexList[i]->weight = vertexList[i]->cost =0;
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

int main()
{
    MAP map;
    map = loadMap(map);
    printMap(map);

    int nOfVerteces = map.n * map.m;
    VERTEX** vertexList;
    vertexList = createVertexList(vertexList, nOfVerteces);
    vertexList = transformToGraph(map, vertexList);
    printVertexList(vertexList, nOfVerteces);

    return 0;
}
