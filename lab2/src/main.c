#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE 1000
#define Swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

void ConvertInChislo(unsigned int* number, int lenNum) {
    for (int i = 0; i < lenNum; ++i) {
        number[i] -= '0';
    }
}

unsigned int ConvertNOT (unsigned int numOfTurns[SIZE], int lenNOT) {
    int ConvertNumOfTurns = 0;
    for (int i = lenNOT - 1, j = 1; i >= 0; --i, j *= 10) {
        ConvertNumOfTurns += numOfTurns[i] * j;
    }
    return ConvertNumOfTurns;
}

void Chek(unsigned int number[SIZE], int lenNum) {
    int coincidence = 1;
    for (int i = 0; i < lenNum; ++i) {
        if (number[i] > 9) {
            printf("bad input");
            exit(0);
        }
        for (int j = i + 1; j < lenNum; ++j) {
            if (number[i] == number[j]) {
                printf("bad input");
                exit(0);
            }
        }
        if (i > 0 && number[i] > number[i - 1] && coincidence) {
            coincidence = 0;
        }
    }
    if (coincidence) {
        exit(0);
    }
}

void PartialTransformations(unsigned int *number, int lenNum) {
    int i = lenNum - 2, j;
    while (number[i] > number[i + 1]) {
        --i;
    }
    unsigned int minNum = 10, mini = 0;
    j = i + 1;
    while (j < lenNum) {
        //printf("i = %d; number[i] = %d; j = %d; number[j] = %d\n", i, number[i], j, number[j]);
        if ((number[j] < minNum) && (number[j] > number[i])) {
            minNum = number[j];
            mini = j;
        }
        ++j;
    }
    //printf("minNum is %d\n", minNum);
    Swap(number[i], number[mini]);
    for (int z = i + 1; z < lenNum - z + i; ++z) {
        Swap(number[z], number[lenNum - z + i]);
    }
}

void GeneralTransformations(unsigned int* number, int lenNum, int numOfTurns) {
    for (int i = 0; i < numOfTurns; ++i) {
        PartialTransformations(number, lenNum);
        for (int i = 0; i < lenNum; ++i) { printf("%u", number[i]); }
        Chek(number, lenNum);
        if (i != numOfTurns - 1) {
            printf("\n");
        }
    }
}

int main(void) {
    unsigned int number[SIZE], stringNumOfTurns[SIZE];
    int chr, lenNum = 0, lenNOT = 0; // lenNOT = lenNumOfTurns
    while ((chr = getchar()) != '\n') {
        number[lenNum] = chr;
        ++lenNum;
    }
    while ((chr = getchar()) != EOF) {
        stringNumOfTurns[lenNOT] = chr;
        ++lenNOT;
    }
    --lenNOT;
    ConvertInChislo(stringNumOfTurns, lenNOT);
    int numOfTurns = ConvertNOT(stringNumOfTurns, lenNOT);
    //printf("NumOfTurns is %d\n", numOfTurns);
    ConvertInChislo(number, lenNum);
    Chek(number, lenNum);
    GeneralTransformations(number, lenNum, numOfTurns);
    return 0;
}
