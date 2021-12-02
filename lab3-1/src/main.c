#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

void QuickSort(int* iArray, int left, int right) {
	if (left < right) {
		int valuePivot = iArray[(right + left) / 2];
		int i = left, j = right;

		while (i != j) {
			if (iArray[i] >= valuePivot) {
				while (iArray[j] > valuePivot && i != j) {
					--j;
				}
				if (i == j) {
					break;
				}
				swap(iArray[i], iArray[j]);
			}
			++i;
		}

		QuickSort(iArray, left, i - 1);
		if (i != left) {
			QuickSort(iArray, i, right);
		}
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
