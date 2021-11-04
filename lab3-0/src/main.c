#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

void ExchangeDown(int* iArray, int lenArray, int index);

void ExchangeUp(int* iArray, int lenArray, int index) {
	if ((index / 2) > 0 && iArray[index] > iArray[index / 2]) {
		swap(iArray[index], iArray[index / 2]);
		ExchangeDown(iArray, lenArray, index);
		ExchangeUp(iArray, lenArray, index / 2);
	}
}

void ExchangeDown(int* iArray, int lenArray, int index) {
	if ((index * 2) < lenArray) {
		int max = index * 2;
		if ((max + 1) < lenArray && iArray[max] < iArray[max + 1]) {
			++max;
		}
		if (iArray[max] > iArray[index]) {
			swap(iArray[index], iArray[max]);
			ExchangeDown(iArray, lenArray, max);
			ExchangeUp(iArray, lenArray, index);
		}
	}
}

void ChangeToMaxHeap(int* iArray, int lenArray) {
	for (int i = 1; i * 2 < lenArray; ++i) {
		ExchangeDown(iArray, lenArray, i);
	}
}

void HeapSort(int* iArray, int lenArray) {
	while (lenArray > 2) {
		swap(iArray[1], iArray[lenArray - 1]);
		--lenArray;
		ChangeToMaxHeap(iArray, lenArray);
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
	ChangeToMaxHeap(iArray, lenArray);
	HeapSort(iArray, lenArray);
	for (int i = 1; i < lenArray; ++i) {
		printf("%d ", iArray[i]);
	}
}
