#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int TopDFS(int N, char** graph, int indexG, char* color, int* answer, int* indexA) {
    if (color[indexG] == 1) {
        return 1;
    }
    if (color[indexG] == 2) {
        return 0;
    }

    color[indexG] = 1; //Gray

    for (int i = 0; i < N; ++i) {
        if (graph[indexG][i]) {
            if (TopDFS(N, graph, i, color, answer, indexA)) return 1;
        }
    }

    color[indexG] = 2; //Black

    answer[*indexA] = indexG + 1;
    *indexA += 1;

    return 0;

}
int CheckBadVertex(int N, int from, int wheree) {
    if ((from < 1 || from > N) || (wheree < 1 || wheree > N)) {
        return 1;
    }
    return 0;
}

int TopSort(int N, int M, int* answer) {
    char** graph = (char**)calloc(N, sizeof(char*));
    
    for (int i = 0; i < N; ++i) {
        graph[i] = (char*)calloc(N, sizeof(char));
    }

    int from, wheree;

    for (int i = 0; i < M; ++i) {
        if (scanf("%d %d", &from, &wheree) != 2) {
            return 1; 
        }
        if (CheckBadVertex(N, from, wheree)) return 2; 
        graph[from - 1][wheree - 1] = 1;
    }

    /*for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d", graph[i][j]);
        }
        printf("\n");
    }*/

    char* color = (char*)calloc(N, sizeof(char));
    int trash = 0;
    int* indexA = &trash;
    for (int i = 0; i < N; ++i) {
        if (TopDFS(N, graph, i, color, answer, indexA)) { return 3;}
    }
    
    for (int i = 0; i < N; ++i) {
        free(graph[i]);
    }

    free(graph);
    free(color);

    return 0;
}

int CheckInput(int N, int M) {
    if (N < 0 || N > 2000) {
        return 1;
    }

    if (M < 0 || M >(N * N - N) / 2) {
        return 2;
    }

    if (!M) {
        return 3;
    }

    return 0;
}

int main()
{
    int N, M;
    if (scanf("%d\n%d", &N, & M) != 2) {
        printf("bad number of lines");
        return 0;
    }

    int* answer = (int*)calloc(N, sizeof(int));

    switch (CheckInput(N, M)) {
    case(1): { printf("bad number of vertices"); free(answer); return 0; }
    case(2): { printf("bad number of edges"); free(answer); return 0; }
    case(3): {
        for (int i = 1; i <= N; ++i) {
            printf("%d ", i);
        }
        free(answer);
        return 0;
    }
    }

    switch (TopSort(N, M, answer)) {
    case(0): {
        for (int i = N - 1; i >= 0; --i) {
            printf("%d ", answer[i]);
        }
        free(answer);
        return 0;
    }
    case(1): { printf("bad number of lines"); free(answer); return 0; }
    case(2): { printf("bad vertex"); free(answer); return 0; }
    case(3): { printf("impossible to sort"); free(answer); return 0; }
    }

    
    
    return 0;
}
