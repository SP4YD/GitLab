#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 1000
#define Swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

int Chislo(unsigned char symbol) {
	return symbol - '0';
}

void Chek(unsigned char number[SIZE], int lenNum) {
	if (lenNum < 11) {
		int coincidence = 1;
		char repeat[10] = { 0 };
		for (int i = 0; i < lenNum; ++i) {
			if (!repeat[Chislo(number[i])] && number[i] <= '9' && number[i] >= '0') {
				repeat[Chislo(number[i])] = 1;
			}
			else {
				printf("bad input");
				exit(0);
			}
			if (i > 0 && number[i] > number[i - 1] && coincidence) {
				coincidence = 0;
			}
		}
		if (coincidence) {
			exit(0);
		}
	}
}

void PartialTransformations(unsigned char* number, int lenNum) {
	int i = lenNum - 2, j;
	while (number[i] > number[i + 1]) {
		--i;
	}
	unsigned int minNum = '9' + 1, mini = 0;
	j = i + 1;
	while (j < lenNum) {
		if ((number[j] < minNum) && (number[j] > number[i])) {
			minNum = number[j];
			mini = j;
		}
		++j;
	}
	Swap(number[i], number[mini]);
	for (int z = i + 1; z < (lenNum + i) / 2; ++z) {
		Swap(number[z], number[lenNum - z + i]);
	}
}

void GeneralTransformations(unsigned char* number, int lenNum, int numOfTurns) {
	for (int i = 0; i < numOfTurns; ++i) {
		PartialTransformations(number, lenNum);
		printf("%s", number);
		Chek(number, lenNum);
		if (i != numOfTurns - 1) {
			printf("\n");
		}
	}
}

int main(void) {
	unsigned char number[SIZE];
	unsigned int lenNum = 0, numOfTurns = 0, chr; // lenNOT = lenNumOfTurns
	while ((chr = getchar()) != '\n') {
		number[lenNum] = chr;
		++lenNum;
	}
	number[lenNum] = '\0';
	if (scanf("%u", &numOfTurns) != 1) { return 0; }
	Chek(number, lenNum);
	GeneralTransformations(number, lenNum, numOfTurns);
	return 0;
}
