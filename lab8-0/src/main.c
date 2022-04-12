#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct TList TList;
typedef struct TVertexList TVertexList;

struct TList {
    int Vertex;
    int Len;
    TList* Next;
};

struct TVertexList {
    char Using;
    int Vertex;
    int Len;
    TList* Next;
};

void FreeAll(int** answer, TVertexList* AdjList, int N) {
    free(answer[0]);
    free(answer[1]);
    free(answer);
    for (int i = 0; i < N; ++i) {
        TList* Curr = AdjList[i].Next;
        while (Curr != NULL)
        {
            TList* Temp = Curr;
            Curr = Curr->Next;
            free(Temp);
        }
    }
    free(AdjList);
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

void AddElementInList(TVertexList* AdjList, int from, int len, int where) {
    TList* TempNext;
    if (AdjList[from].Next && !(AdjList[from].Next->Len > len)) {
        TList* Curr = AdjList[from].Next;
        char stop = 0;

        while (!stop) {
            if (Curr->Next == NULL) {
                stop = 1;
            }
            else if (Curr->Next->Len > len) {
                stop = 1;
            }
            else {
                Curr = Curr->Next;
            }
        }

        TempNext = Curr->Next;
        Curr->Next = calloc(1, sizeof(TList));
        Curr->Next->Len = len;
        Curr->Next->Vertex = where;
        Curr->Next->Next = TempNext;
    }
    else {
        TempNext = AdjList[from].Next;
        AdjList[from].Next = calloc(1, sizeof(TList));
        AdjList[from].Next->Len = len;
        AdjList[from].Next->Vertex = where;
        AdjList[from].Next->Next = TempNext;
    }
}

int GraphEntry(int N, int M, TVertexList* AdjList) {
    int from, where;
    unsigned long long len;

    for (int i = 0; i < N; ++i) {
        AdjList[i].Len = 0;
        AdjList[i].Vertex = i;
        AdjList[i].Next = NULL;
        AdjList[i].Using = 0;
    }

    for (int i = 0; i < M; ++i) {
        if (scanf("%d %d %llu", &from, &where, &len) != 3) {
            return 1    ;
        }

        char code = CheckSecondInput(from, where, N, len);
        if (code) { return code; }

        AddElementInList(AdjList, from - 1, len, where - 1);
        AddElementInList(AdjList, where - 1, len, from - 1);
    }

    return 0;
}

int FindIndex(int N, TVertexList* AdjList, char notFirst) {
    if (!notFirst) {
        return 0;
    }

    int min = INT_MAX, index = 0;
    for (int i = 0; i < N; ++i) {
        if (AdjList[i].Using) {
            if (AdjList[i].Next) {
                if (!AdjList[AdjList[i].Next->Vertex].Using) {
                    if (min > AdjList[i].Next->Len) {
                        min = AdjList[i].Next->Len;
                        index = i;
                    }
                }
            }
        }
    }

    return index;
}

void FreeElement(TVertexList* AdjList, int index, int num) {
    TList* Temp = AdjList[index].Next;
    if (num < 0) {
        AdjList[index].Next = AdjList[index].Next->Next;
    }
    else {
        if (Temp->Vertex == num) {
            AdjList[index].Next = AdjList[index].Next->Next;
        }
        else {
            TList* NewNext = Temp;
            while (NewNext->Next->Vertex != num) {
                NewNext = NewNext->Next;
            }
            Temp = NewNext->Next;
            NewNext->Next = NewNext->Next->Next;
        }
    }
    free(Temp);
}

int AlgorithmPrima(int N, int M, TVertexList* AdjList, int** answer, int* answerCount) {
    int code = GraphEntry(N, M, AdjList);
    if (code) { return code; }

    while (*answerCount != N - 1) {
        int index = FindIndex(N, AdjList, *answerCount);

        AdjList[index].Using = 1;
        AdjList[AdjList[index].Next->Vertex].Using = 1;
        answer[0][*answerCount] = AdjList[index].Vertex + 1;
        if (!AdjList[index].Next) {
            return 4;
        }
        answer[1][*answerCount] = AdjList[index].Next->Vertex + 1;

        FreeElement(AdjList, AdjList[index].Next->Vertex, AdjList[index].Vertex);
        FreeElement(AdjList, index, -1);

        ++(*answerCount);
    }

    return 0;
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
        FreeAll(answer, AdjList, N);
        return 0;
    }
    case(1): { printf("bad number of lines"); FreeAll(answer, AdjList, N); return 0; }
    case(2): { printf("bad vertex"); FreeAll(answer, AdjList, N); return 0; }
    case(3): { printf("bad length"); FreeAll(answer, AdjList, N); return 0; }
    case(4): { printf("no spanning tree"); FreeAll(answer, AdjList, N); return 0; }
    }
}
