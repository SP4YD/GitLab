#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MY_INT_MAX 2147483647

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

int GraphEntry (int N, int M, unsigned int** adjArray) {
    unsigned long long len;
    int from, where;

    for (int i = 0; i < M; ++i) {
        if (scanf ("%d %d %llu", &from, &where, &len) != 3) {
            return 1;
        }

        char code = CheckSecondInput (from, where, N, len);
        if (code) {
            return code;
        }

        if (from != where) {
            adjArray[from - 1][where - 1] = adjArray[where - 1][from - 1] = (unsigned int)len;
        }
    }

    return 0;
}

int PrintWithVerification (unsigned int** adjArray, int N, int F) {
    int countForOverflow = 0;

    for (int i = 0; i < N; ++i) {
        if (adjArray[i][i] == UINT_MAX) {
            printf ("oo ");
        }
        else if (adjArray[i][i] > INT_MAX) {
            printf ("INT_MAX+ ");
        }
        else {
            printf ("%u ", adjArray[i][i]);
        }

        if (adjArray[i][i] >= INT_MAX && adjArray[i][i] < UINT_MAX && i != F) {
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

int FindNextVertex (unsigned int** adjArray, int N, char* visited) {
    int index = INT_MAX;
    unsigned long long distance = LLONG_MAX;

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            if ((unsigned long long) adjArray[i][i] < distance) {
                distance = adjArray[i][i];
                index = i;
            }
        }
    }

    return index;
}

int AlgorithmDijkstra (int N, int M, int S, unsigned int** adjArray, int* parents) {
    int vertexNow = S;
    int code = GraphEntry (N, M, adjArray);
    if (code) {
        return code;
    }

    for (int i = 0; i < N; ++i) {
        adjArray[i][i] = UINT_MAX;
        parents[i] = INT_MAX;
    }

    adjArray[S][S] = 0;
    parents[S] = S;
    char* visited = calloc (N, sizeof (char));

    while (vertexNow != INT_MAX) {
        visited[vertexNow] = 1;
        for (int i = 0; i < N; ++i) {
            if (i != vertexNow && adjArray[vertexNow][i] != 0) {
                unsigned long long distance = (unsigned long long)adjArray[vertexNow][i];
                distance += (unsigned long long)adjArray[vertexNow][vertexNow];

                if ((unsigned long long)adjArray[i][i] > distance) {
                    parents[i] = vertexNow;
                    if (distance > INT_MAX) {
                        adjArray[i][i] = UINT_MAX - 1;
                    }
                    else {
                        adjArray[i][i] = (unsigned int)distance;
                    }
                }
            }
        }

        vertexNow = FindNextVertex (adjArray, N, visited);
    }

    free (visited);

    return 0;
}

/// ////////////////////////////////////////////////// enum и ОТЧИСТКА ПАМЯТИ

int main () {
    int N, S, F, M;

    //freopen ("in.txt", "r", stdin);

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

    unsigned int** adjArray = calloc (N, sizeof (unsigned int*));
    for (int i = 0; i < N; ++i) {
        adjArray[i] = calloc (N, sizeof (unsigned int));
    }
    int* parents = calloc (N, sizeof (int));

    switch (AlgorithmDijkstra (N, M, S - 1, adjArray, parents)) {
        case(0): {
            int index = F - 1;

            if (PrintWithVerification (adjArray, N, index) && adjArray[index][index] > INT_MAX) {
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
            
            break;
        }
        case(1): {
            printf ("bad number of lines");
            break;
        }
        case(2): {
            printf ("bad vertex");
            break;
        }
        case(3): {
            printf ("bad length");
            break;
        }
    }

    free (parents);
    free (adjArray);

    return 0;
}
