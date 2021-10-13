#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define SIZE 1000000

int FindWeight(int symbol, int n) { /////////////переделать
	int a = 1;
	for (int i = 0; i < n; ++i) {
		a *= 3;
	}
	return (((symbol) % 3) * a);
}

void Compare(int WeightPattern, int WeightPartText, int icheck, int LenPattern, unsigned char* pattern, unsigned char* text, int index) {
	if (WeightPattern == WeightPartText) {
		bool coincidence = 1;
		for (int i = 0; (i < LenPattern) && coincidence; ++i) {
			printf("%d ", icheck + i + index);
			if (pattern[i] != text[i + icheck]) {
				coincidence = 0;
			}
		}
	}
}

void Check(int LenText, int LenPattern, unsigned char* pattern, unsigned char* text, int WeightPattern, int index) {
	int WeightPartText = 0, icheck = 0;
	for (int i = 0; i < LenPattern; ++i) {
		WeightPartText += FindWeight(text[i], i);
	}
	while ((icheck + LenPattern) < LenText) {
		//printf("%d and %d\n", WeightPattern, WeightPartText);
		Compare(WeightPattern, WeightPartText, icheck, LenPattern, pattern, text, index);
		WeightPartText = (WeightPartText - FindWeight(text[icheck], 0)) / 3 + FindWeight(text[icheck + LenPattern], LenPattern - 1);
		++icheck;
	}
	Compare(WeightPattern, WeightPartText, icheck, LenPattern, pattern, text, index);
}

int main(void) {
	int LenText = 0, LenPattern = 0, WeightPattern = 0, index = 1;
	unsigned char text[SIZE], chr, pattern[16];
	while ((chr = getchar()) != '\n') {
		pattern[LenPattern] = chr;
		++LenPattern;
	}
	LenText = fread(text, 1, SIZE, stdin);
	if (LenText == 0) { printf("0"); return 0; }
	for (int i = 0; i < LenPattern; ++i) {
		WeightPattern += FindWeight(pattern[i], i);
	}
	printf("%d ", WeightPattern);
	Check(LenText, LenPattern, pattern, text, WeightPattern, index);
	if (LenText == SIZE){
	    for (int i = 0; LenText == SIZE; ++i){
	        index += LenText;
	        LenText = fread(text, 1, SIZE, stdin);
	        Check(LenText, LenPattern, pattern, text, WeightPattern, index);
	    }
	}
	return 0;
}
