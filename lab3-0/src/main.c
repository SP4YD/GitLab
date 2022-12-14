#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

void ExchangeDown(int* iArray, int lenArray, int index) {
	if ((index * 2) < lenArray) {
		int max = index * 2;
		if ((max + 1) < lenArray && iArray[max] < iArray[max + 1]) {
			++max;
		}
		if (iArray[max] > iArray[index]) {
			swap(iArray[index], iArray[max]);
			ExchangeDown(iArray, lenArray, max);
		}
	}
}

void FirstChangeToMaxHeap(int* iArray, int lenArray) {
	for (int i = lenArray - 1; i > 0; --i) {
		ExchangeDown(iArray, lenArray, i);
	}
}

void HeapSort(int* iArray, int lenArray) {
	FirstChangeToMaxHeap(iArray, lenArray);
	while (lenArray > 2) {
		swap(iArray[1], iArray[lenArray - 1]);
		--lenArray;
		ExchangeDown(iArray, lenArray, 1);
	}
}

int main(void) {
	int lenArray;
	if (scanf("%d", &lenArray) != 1) { return 0; }
	++lenArray;
	int* iArray = (int*)malloc(sizeof(int) * lenArray);
	for (int i = 1; i < lenArray; ++i) {
		if (scanf("%d", &iArray[i]) != 1) { return 0; }
	}
	HeapSort(iArray, lenArray);
	for (int i = 1; i < lenArray; ++i) {
		printf("%d ", iArray[i]);
	}
	free(iArray);
	return 0;
}
