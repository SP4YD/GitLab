#include <stdio.h>
#include <stdbool.h>
#define SIZE 256

int FindWeight(int symbol, int n) {
	int a = 1;
	for (int i = 0; i < n; ++i) {
		a *= 3;
	}
	return ((symbol % 3) * a);
}

void Compare(int icheck, int LenPattern, int* pattern, int* text) {
	bool coincidence = 1;
	for (int i = 0; (i < LenPattern) && coincidence; ++i) {
		printf("%d ", icheck + i + 1);
		if (pattern[i] != text[i + icheck]) {
			coincidence = 0;
		}
	}
}

void Check(int LenText, int LenPattern, int* pattern, int* text, int WeightPattern) {
	int WeightPartText = 0, icheck = 0;
	for (int i = 0; i < LenPattern; ++i) {
		WeightPartText += FindWeight(text[i], i);
	}
	while ((icheck + LenPattern) < LenText) {
		if (WeightPattern == WeightPartText) {
			Compare(icheck, LenPattern, pattern, text);
		}
		WeightPartText = (WeightPartText - FindWeight(text[icheck], 0)) / 3 + FindWeight(text[icheck + LenPattern], LenPattern - 1);
		++icheck;
	}
}

int main(void) {
	int pattern[SIZE], text[SIZE], chr, LenText = 0, LenPattern = 0, WeightPattern = 0;
	while ((chr = getc(stdin)) != '\n') {
		pattern[LenPattern] = chr;
		++LenPattern;
	}
	while ((chr = getc(stdin)) != EOF) {
		text[LenText] = chr;
		++LenText;
	}
	if (LenText == 0 && chr == EOF) { return 0; }
	for (int i = 0; i < LenPattern; ++i) {
		WeightPattern += FindWeight(pattern[i], i);
	}
	printf("\n%d ", WeightPattern);
	Check(LenText, LenPattern, pattern, text, WeightPattern);
	return 0;
}
