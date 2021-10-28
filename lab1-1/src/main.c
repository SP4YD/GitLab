#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int FindWeight(int symbol, int degree) {
	return (((symbol) % 3) * degree);
}

void Compare(int weightPattern, int weightPartText, int lenPattern, unsigned char* pattern, unsigned char* text, int index) {
	if (weightPattern == weightPartText) {
		for (int i = 0; i < lenPattern; ++i) {
			printf("%d ", i + index);
			if (pattern[i] != text[i]) {
				return;
			}
		}
	}
}

int Check(int lenPattern, unsigned char* pattern, unsigned char* text, int weightPattern, int weightText, int index) {
	Compare(weightPattern, weightText, lenPattern, pattern, text, index);
	weightText = (weightText - FindWeight(text[0], 1)) / 3;
	return weightText;
}

void Shift(unsigned char* text, int lenPattern, unsigned char symbol) {
	for (int i = 0; i < lenPattern - 1; ++i) {
		text[i] = text[i + 1];
	}
	text[lenPattern - 1] = symbol;
}

int main(void) {
	int lenText = 0, lenPattern = 0, weightPattern = 0, index = 0, degree = 1, weightText = 0;
	unsigned char pattern[17];
	if (scanf("%16[^\n]s", pattern) != 1) { return 0; }
	for (int i = 0; pattern[i] != '\0'; ++i) {
		++lenPattern;
	}
	int degreeLastSymb = pow(3, lenPattern - 1);
	unsigned char text[17];
	lenText = fread(text, 1, lenPattern, stdin);
	if (lenText == 0 || lenText < lenPattern) { printf("0"); return 0; }
	for (int i = 0; i < lenPattern; ++i) {
		weightText += FindWeight(text[i], degree);
		weightPattern += FindWeight(pattern[i], degree);
		degree *= 3;
	}
	printf("%d ", weightPattern);
	weightText = Check(lenPattern, pattern, text, weightPattern, weightText, index);
	unsigned char newText[1];
	lenText = fread(newText, 1, 1, stdin);
	if (lenText) {
		while (lenText) {
			Shift(text, lenPattern, newText[0]);
			weightText += FindWeight(newText[0], degreeLastSymb);
			index += 1;
			weightText = Check(lenPattern, pattern, text, weightPattern, weightText, index);
			lenText = fread(newText, 1, 1, stdin);
		}
	}
	return 0;
}
