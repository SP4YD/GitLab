#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct TList TList;
typedef struct TVertexList TVertexList;

struct TList {
    short Vertex;
    unsigned int Len;
};


struct TVertexList {
    char Visited;
    short Vertex;
    short SizeArray;
    unsigned long long Distance;
    TList Array[5001];
};

int Comparator (const void* first, const void* second) {
    if (((TList*)first)->Len < ((TList*)second)->Len) {
        return 0;
    } else {
        return 1;
    }
}

int CheckFirstInput (int N, int M) {
    if (N < 0 || N > 5000) {
        return 1;
    }

    if (M < 0 || M > (N * N - N) / 2) {
        return 2;
    }

    if (N == 0) {
        return 3;
    }

    if (M == 0 && N != 1) {
        return 4;
    }

    return 0;
}

int CheckSecondInput (int from, int where, int N, unsigned long long len) {
    if (from < 1 || from > N || where < 1 || where > N) {
        return 2;
    }

    if (len > INT_MAX) {
        return 3;
    }

    return 0;
}

void AddElementInList (TVertexList* AdjList, int from, int where, unsigned long long len) {
    TList* Array = AdjList[from].Array;
    int size = AdjList[from].SizeArray;

    Array[size].Len = (unsigned int)len;
    Array[size].Vertex = where;
    AdjList[from].SizeArray += 1;
}

int GraphEntry (int N, int M, TVertexList* adjList) {
    unsigned long long len;
    int from, where;

    for (int i = 0; i < N; ++i) {
        adjList[i].Vertex = i;
        adjList[i].Visited = 0;
        adjList[i].SizeArray = 0;
        adjList[i].Distance = LLONG_MAX;
    }

    for (int i = 0; i < M; ++i) {
        if (scanf ("%d %d %llu", &from, &where, &len) != 3) {
            return 1;
        }

        char code = CheckSecondInput (from, where, N, len);
        if (code) {
            return code;
        }

        if (from != where) {
            AddElementInList (adjList, from - 1, where - 1, len);
            AddElementInList (adjList, where - 1, from - 1, len);
        }
    }

    return 0;
}

int PrintWithVerification (TVertexList* adjList, int N, int F) {
    int countForOverflow = 0;

    for (int i = 0; i < N; ++i) {
        if (adjList[i].Distance == LLONG_MAX) {
            printf ("oo ");
        }
        else if (adjList[i].Distance > INT_MAX) {
            printf ("INT_MAX+ ");
        }
        else {
            printf ("%llu ", adjList[i].Distance);
        }

        if (adjList[i].Distance >= INT_MAX && adjList[i].Distance < LLONG_MAX && i != F) {
            ++countForOverflow;
        }
    }
    printf ("\n");

    if (countForOverflow > 1) {
        return 1;
    }
    else {
        return 0;
    }
}

int FindNextVertex (TVertexList* adjList, int N) {
    int index = INT_MAX;
    unsigned long long distance = INT_MAX;

    for (int i = 0; i < N; ++i) {
        if (!adjList[i].Visited) {
            if (adjList[i].Distance < distance) {
                distance = adjList[i].Distance;
                index = i;
            }
        }
    }

    return index;
}

int AlgorithmDijkstra (int N, int M, int S, TVertexList* adjList, int* parents) {
    int vertexNow = S;
    int code = GraphEntry (N, M, adjList);
    if (code) {
        return code;
    }

    for (int i = 0; i < N; ++i) {
        qsort (adjList[i].Array, adjList[i].SizeArray, sizeof (TList), Comparator);
        parents[i] = INT_MAX;
    }

    adjList[S].Distance = 0;
    parents[S] = S;

    while (vertexNow != INT_MAX) {
        adjList[vertexNow].Visited = 1;
        for (int i = 0; i < adjList[vertexNow].SizeArray; ++i) {
            TVertexList* neighboringVertex = &adjList[adjList[vertexNow].Array[i].Vertex];
            unsigned long long distance = adjList[vertexNow].Array[i].Len + adjList[vertexNow].Distance;
            
            if (neighboringVertex->Distance > distance) {
                parents[adjList[vertexNow].Array[i].Vertex] = vertexNow;
                neighboringVertex->Distance = distance;
            }
        }

        vertexNow = FindNextVertex (adjList, N);
    }

    return 0;
}

/// ////////////////////////////////////////////////// enum и ОТЧИСТКА ПАМЯТИ
/////////////////////////////// Проверить не является ли переменная Vertex лишней

int main () {
    int N, S, F, M;

    if (scanf ("%d\n%d %d\n%d", &N, &S, &F, &M) != 4) {
        return 0;
    }

    switch (CheckFirstInput (N, M)) {
        case(1): {
            printf ("bad number of vertices");
            return 0;
        }
        case(2): {
            printf ("bad number of edges");
            return 0;
        }
        case(3): {
            printf ("bad vertex");
            return 0;
        }
        case(4): {
            printf ("0 ");

            for (int i = 0; i < N - 1; ++i) {
                printf ("oo ");
            }

            printf ("\nno path");
            return 0;
        }
    }

    TVertexList* adjList = calloc (N, sizeof (TVertexList));
    int* parents = calloc (N, sizeof (int));

    switch (AlgorithmDijkstra (N, M, S - 1, adjList, parents)) {
        case(0): {
            int index = F - 1;

            if (PrintWithVerification (adjList, N, index) && adjList[index].Distance > INT_MAX) {
                printf ("overflow");
            }
            else if (parents[index] == INT_MAX) {
                printf ("no path");
            }
            else {
                printf ("%d ", F);

                while (parents[index] != index) {
                    printf ("%d ", parents[index] + 1);
                    index = parents[index];
                }
            }
            
            return 0;
        }
        case(1): {
            printf ("bad number of lines");
            return 0;
        }
        case(2): {
            printf ("bad vertex");
            return 0;
        }
        case(3): {
            printf ("bad length");
            return 0;
        }
    }

    return 0;
}
