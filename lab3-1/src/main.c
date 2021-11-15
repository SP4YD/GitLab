#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

void QuickSort(int* iArray, int left , int right) {
	if (left < right) {

		int iPivot = right;
		int wall = left;

		for (int i = left; i <= right; ++i) {
			if (iArray[i] < iArray[iPivot]) {
				if (i != wall) {
					swap(iArray[i], iArray[wall]);
				}
				++wall;
			}
		}

		if (iPivot != wall) {
			swap(iArray[iPivot], iArray[wall]);
		}

		QuickSort(iArray, left, wall - 1);
		QuickSort(iArray, wall + 1, right);
	}
}

int main(void) {
	int lenArray;

	if (scanf("%d", &lenArray) != 1) { return 0; }

	int* iArray = (int*)malloc(sizeof(int) * lenArray);

	for (int i = 0; i < lenArray; ++i) {
		if (scanf("%d", &iArray[i]) != 1) { return 0; }
	}

	QuickSort(iArray, 0, lenArray - 1);

	for (int i = 0; i < lenArray; ++i) {
		printf("%d ", iArray[i]);
	}

	free(iArray);

	return 0;
}
