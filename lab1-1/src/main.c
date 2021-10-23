#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define SIZE 1000000


int FindWeight(int symbol, int degree) {
	return (((symbol) % 3) * degree);
}

void Compare(int weightPattern, int weightPartText, int icheck, int lenPattern, unsigned char* pattern, unsigned char* text, int index) {
	if (weightPattern == weightPartText) {
		for (int i = 0; i < lenPattern; ++i) {
			printf("%d ", icheck + i + index);
			if (pattern[i] != text[i + icheck]) {
				return;
			}
		}
	}
}

void Check(int lenText, int lenPattern, unsigned char* pattern, unsigned char* text, int weightPattern, int index) {
	int weightPartText = 0, icheck = 0, degreeFirstSymb = 1, degreeLastSymb = pow(3, lenPattern - 1);
	for (int i = 0, degree = 1; i < lenPattern; ++i) {
		weightPartText += FindWeight(text[i], degree);
		degree *= 3;
	}
	while ((icheck + lenPattern) < lenText) {
		Compare(weightPattern, weightPartText, icheck, lenPattern, pattern, text, index);
		weightPartText = (weightPartText - FindWeight(text[icheck], degreeFirstSymb)) / 3 + FindWeight(text[icheck + lenPattern], degreeLastSymb);
		++icheck;
	}
	Compare(weightPattern, weightPartText, icheck, lenPattern, pattern, text, index);
}

int main(void) {
	int lenText = 0, lenPattern = 0, weightPattern = 0, index = 0, degree = 1;
	unsigned char text[SIZE], pattern[17];
	if (scanf("%16[^\n]s", pattern) != 1) { return 0; }
	for (int i = 0; pattern[i] != '\0'; ++i) {
		++lenPattern;
	}
	lenText = fread(text, 1, SIZE, stdin);
	if (lenText == 0) { printf("0"); return 0; }
	for (int i = 0; i < lenPattern; ++i) {
		weightPattern += FindWeight(pattern[i], degree);
		degree *= 3;
	}
	printf("%d ", weightPattern);
	Check(lenText, lenPattern, pattern, text, weightPattern, index);
	if (lenText == SIZE) {
		while (lenText == SIZE) {
			unsigned char newText[SIZE];
			index += lenText;
			lenText = fread(newText, 1, SIZE, stdin);
			Check(lenText, lenPattern, pattern, newText, weightPattern, index);
		}
	}
	return 0;
}
