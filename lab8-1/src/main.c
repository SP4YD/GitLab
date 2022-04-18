#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct TList TList;
typedef struct TListElement TListElement;

struct TList {
    int Size;
    TListElement* Array;
};

struct TListElement {
    short Vertex1;
    short Vertex2;
    unsigned int Len;
};

short FindRoot(short* dsu, short v) {
    while (dsu[v] != v) {
        short temp = dsu[v];
        dsu[v] = dsu[dsu[v]];
        v = temp;
    }
    return v;
}

char UnionSet(short* dsu, short a, short b) {
    if (FindRoot(dsu, a) != FindRoot(dsu, b)) {
        dsu[FindRoot(dsu, a)] = FindRoot(dsu, b);
        return 1;
    }
    return 0;
}

int Comparator(const void* first, const void* second) {
    if (((TListElement*)first)->Len > ((TListElement*)second)->Len) {
        return 0;
    }
    else {
        return 1;
    }
}

void FreeAll(int** answer, TList AdjList) {
    free(answer[0]);
    free(answer[1]);
    free(answer);
    free(AdjList.Array);
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

int GraphEntry(int N, int M, TList* HeadArray) {
    unsigned long long len;
    int from, where;
    char check = 0;

    for (int i = 0; i < M; ++i) {
        if (scanf("%d %d %llu", &from, &where, &len) != 3) {
            return 1;
        }

        char code = CheckSecondInput(from, where, N, len);
        if (code) { 
            return code;
        }

        if (from != where) {
            HeadArray->Array[HeadArray->Size].Vertex1 = from;
            HeadArray->Array[HeadArray->Size].Vertex2 = where;
            HeadArray->Array[HeadArray->Size].Len = (unsigned int)len;
            ++(HeadArray->Size);
            check = 1;
        }
    }

    return check ? 0 : 4;
}

int AlgorithmKraskala(int N, int M, TList* HeadArray, int** answer, int* answerCount) {
    int code = GraphEntry(N, M, HeadArray);
    if (code) { 
        return code;
    }

    short* dsu = calloc(N + 1, sizeof(short));
    for (int i = 1; i <= N; ++i) {
        dsu[i] = i;
    }
    
    qsort(HeadArray->Array, HeadArray->Size, sizeof(TListElement), Comparator);

    while (*answerCount != N - 1 && HeadArray->Size > 0) {
        if (UnionSet(dsu, HeadArray->Array[HeadArray->Size - 1].Vertex1,
                    HeadArray->Array[HeadArray->Size - 1].Vertex2)) 
        {
            answer[0][*answerCount] = HeadArray->Array[HeadArray->Size - 1].Vertex1;
            answer[1][*answerCount] = HeadArray->Array[HeadArray->Size - 1].Vertex2;
            ++(*answerCount);
        }

        --(HeadArray->Size);
    }

    free(dsu);

    if (*answerCount != N - 1) {
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

    switch (CheckFirstInput (N, M)) {
        case(1):
        {
            printf ("bad number of vertices");
            return 0;
        }
        case(2):
        {
            printf ("bad number of edges");
            return 0;
        }
        case(3):
        {
            return 0;
        }
        case(4):
        {
            printf ("no spanning tree");
            return 0;
        }
    }

    TList HeadArray;
    HeadArray.Size = 0;
    HeadArray.Array = calloc(N * (N - 1), sizeof(TListElement));
    int** answer = calloc(2, sizeof(int*)), answerCount = 0;
    answer[0] = calloc(N - 1, sizeof(int));
    answer[1] = calloc(N - 1, sizeof(int));

    switch (AlgorithmKraskala(N, M, &HeadArray, answer, &answerCount)) {
        case(0): {
            for (int i = 0; i < answerCount; ++i) {
                printf("%d %d\n", answer[0][i], answer[1][i]);
            }
            break;
        }
        case(1):{
            printf ("bad number of lines");
            break;
        }
        case(2):{
            printf ("bad vertex");
            break;
        }
        case(3):{
            printf ("bad length");
            break;
        }
        case(4):{
            printf ("no spanning tree");
            break;
        }
    }

    FreeAll(answer, HeadArray);

    return 0;
}
