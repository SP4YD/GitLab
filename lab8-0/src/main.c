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
    char Using;
    short Vertex;
    short Size;
    TList Array[5000];
};

int Comparator(const void* first, const void* second) {
    if (((TList*)first)->Len > ((TList*)second)->Len) {
        return 0;
    } else {
        return 1;
    }
}

void FreeAll(int** answer, TVertexList* AdjList) {
    free(answer[0]);
    free(answer[1]);
    free(answer);
    free(AdjList);
}

int CheckFirstInput(int N, int M) {
    if (N < 0 || N > 5000) {
        return 1;
    }

    if (M < 0 || M >(N * N - N) / 2) {
        return 2;
    }

    if (!M && N == 1) {
        return 3;
    }

    if (!M || M < (N - 1)) {

        return 4;
    }

    return 0;
}

int CheckSecondInput(int from, int where, int N, unsigned long long len) {
    if (from < 1 || from > N || where < 1 || where > N) {
        return 2;
    }

    if (len > INT_MAX) {
        return 3;
    }

    return 0;
}

int CheckGraph(TVertexList* AdjList, int N) {
    for (int i = 0; i < N; ++i) {
        if (!AdjList[i].Size) {
            return 4;
        }
    }
    return 0;
}

void AddElementInList(TVertexList* AdjList, int from, unsigned long long len, int where) {
    TList* Array = AdjList[from].Array;
    int size = AdjList[from].Size;

    Array[size].Len = (unsigned int)len;
    Array[size].Vertex = where;
    AdjList[from].Size += 1;
}

int GraphEntry(int N, int M, TVertexList* AdjList) {
    unsigned long long len;
    int from, where;
    char check = 0;

    for (int i = 0; i < N; ++i) {
        AdjList[i].Vertex = i;
        //AdjList[i].Array = calloc(N - 1, sizeof(TList));
        AdjList[i].Using = 0;
        AdjList[i].Size = 0;
    }

    for (int i = 0; i < M; ++i) {
        if (scanf("%d %d %llu", &from, &where, &len) != 3) {
            return 1;
        }

        char code = CheckSecondInput(from, where, N, len);
        if (code) { return code; }

        if (from != where) {
            AddElementInList(AdjList, from - 1, len, where - 1);
            AddElementInList(AdjList, where - 1, len, from - 1);
            check = 1;
        }
    }

    char code = CheckGraph(AdjList, N);
    if (code) { return code; }

    return check ? 0 : 4;
}

int FindIndex(int N, TVertexList* AdjList, int notFirst) {
    if (!notFirst) {
        return 0;
    }

    unsigned int min = UINT_MAX, index = 0;
    for (int i = 0; i < N; ++i) {
        if (AdjList[i].Using && AdjList[i].Size) {
            if (!AdjList[AdjList[i].Array[AdjList[i].Size - 1].Vertex].Using) {
                if (min > AdjList[i].Array[AdjList[i].Size - 1].Len) {
                    min = AdjList[i].Array[AdjList[i].Size - 1].Len;
                    index = i;
                }
            }
            else {
                AdjList[i].Size -= 1;
                --i;
            }
        }
    }

    return index;
}

int AlgorithmPrima(int N, int M, TVertexList* AdjList, int** answer, int* answerCount) {
    int code = GraphEntry(N, M, AdjList);
    if (code) { return code; }

    for (int i = 0; i < N; ++i) {
        qsort(AdjList[i].Array, AdjList[i].Size, sizeof(TList), Comparator);
    }

    while (*answerCount != N - 1) {
        int index = FindIndex(N, AdjList, *answerCount);

        AdjList[index].Using = 1;
        AdjList[AdjList[index].Array[AdjList[index].Size - 1].Vertex].Using = 1;
        answer[0][*answerCount] = AdjList[index].Vertex + 1;
        if (!AdjList[index].Size) {
            return 4;
        }
        answer[1][*answerCount] = AdjList[AdjList[index].Array[AdjList[index].Size - 1].Vertex].Vertex + 1;

        AdjList[index].Size -= 1;

        ++(*answerCount);
    }

    return 0;
}

int main()
{
    int N, M;

    if (scanf("%d\n%d", &N, &M) != 2) {
        printf("bad number of lines");
        return 0;
    }

    switch (CheckFirstInput(N, M)) {
    case(1): { printf("bad number of vertices"); return 0; }
    case(2): { printf("bad number of edges"); return 0; }
    case(3): { return 0; }
    case(4): { printf("no spanning tree"); return 0; }
    }

    TVertexList* AdjList = calloc(N, sizeof(TVertexList));
    int** answer = calloc(2, sizeof(int*)), answerCount = 0;
    answer[0] = calloc(N - 1, sizeof(int));
    answer[1] = calloc(N - 1, sizeof(int));

    switch (AlgorithmPrima(N, M, AdjList, answer, &answerCount)) {
    case(0): {
        for (int i = 0; i < answerCount; ++i) {
            printf("%d %d\n", answer[0][i], answer[1][i]);
        }
        FreeAll(answer, AdjList);
        return 0;
    }
    case(1): { printf("bad number of lines"); FreeAll(answer, AdjList); return 0; }
    case(2): { printf("bad vertex"); FreeAll(answer, AdjList); return 0; }
    case(3): { printf("bad length"); FreeAll(answer, AdjList); return 0; }
    case(4): { printf("no spanning tree"); FreeAll(answer, AdjList); return 0; }
    }
}
