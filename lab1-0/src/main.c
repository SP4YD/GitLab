#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Compare(int lenPattern, unsigned char* pattern, unsigned char* text, int weightPattern, int weightText, int index) {
	if (weightPattern == weightText) {
		for (int i = 0; i < lenPattern; ++i) {
			printf("%d ", i + index);
			if (pattern[i] != text[i]) {
				break;
			}
		}
	}
	return (weightText - (((text[0]) % 3) * 1)) / 3;
}

void Shift(unsigned char* text, int lenPattern, unsigned char symbol) {
	for (int i = 0; i < lenPattern - 1; ++i) {
		text[i] = text[i + 1];
	}
	text[lenPattern - 1] = symbol;
}

void Check(int lenPattern, int lenText, unsigned char* text, unsigned char* pattern) {
	int weightPattern = 0, index = 0, degree = 1, weightText = 0, degreeLastSymb = pow(3, lenPattern - 1);;
	for (int i = 0; i < lenPattern; ++i) {
		weightText += ((text[i]) % 3) * degree;
		weightPattern += ((pattern[i]) % 3) * degree;
		degree *= 3;
	}
	printf("%d ", weightPattern);
	weightText = Compare(lenPattern, pattern, text, weightPattern, weightText, index);
	unsigned char newText[1];
	lenText = fread(newText, 1, 1, stdin);
	while (lenText) {
		weightText += ((newText[0]) % 3) * degreeLastSymb;
		++index;
		Shift(text, lenPattern, newText[0]);
		weightText = Compare(lenPattern, pattern, text, weightPattern, weightText, index);
		lenText = fread(newText, 1, 1, stdin);
	}
}

int main(void) {
	int lenText, lenPattern = 0;
	unsigned char pattern[17];
	if (scanf("%16[^\n]s", pattern) != 1) { return 0; }
	for (int i = 0; pattern[i] != '\0'; ++i) {
		++lenPattern;
	}
	unsigned char text[17];
	lenText = fread(text, 1, lenPattern, stdin);
	if (lenText == 0 || lenText < lenPattern) { printf("0"); return 0; }
	Check(lenPattern, lenText, text, pattern);
	return 0;
}
