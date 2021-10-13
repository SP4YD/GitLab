#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define SIZE 256

int Convert (int symbol){
    if (symbol < 0){
        return (32 + symbol + '\xE0');
    }
    return symbol;
}

int FindWeight(int symbol, int n) { /////////////переделать
	int a = 1;
	for (int i = 0; i < n; ++i) {
		a *= 3;
	}
	if (symbol < 0){
	printf("symbol is %c(%d) a is %d == %d\n", symbol, symbol, a, ((symbol % 3) * a));}
	return abs((Convert(symbol) % 3) * a);
}

void Compare(int WeightPattern, int WeightPartText, int icheck, int LenPattern, int* pattern, unsigned char* text) {
	if (WeightPattern == WeightPartText) {
		bool coincidence = 1;
		for (int i = 0; (i < LenPattern) && coincidence; ++i) {
			printf("%d ", icheck + i + 1);
			if (pattern[i] != text[i + icheck]) {
				coincidence = 0;
			}
		}
	}
}

void Check(int LenText, int LenPattern, int* pattern, unsigned char* text, int WeightPattern) {
	int WeightPartText = 0, icheck = 0;
	for (int i = 0; i < LenPattern; ++i) {
		WeightPartText += FindWeight(text[i], i);
	}
	while ((icheck + LenPattern) < LenText) {
		//printf("%d and %d\n", WeightPattern, WeightPartText);
		Compare(WeightPattern, WeightPartText, icheck, LenPattern, pattern, text);
		WeightPartText = (WeightPartText - FindWeight(text[icheck], 0)) / 3 + FindWeight(text[icheck + LenPattern], LenPattern - 1);
		++icheck;
	}
	Compare(WeightPattern, WeightPartText, icheck, LenPattern, pattern, text);
}

int main(void) {
	int pattern[SIZE], LenText = 0, LenPattern = 0, WeightPattern = 0;
	char chr;
	while ((chr = getchar()) != '\n') {
		pattern[LenPattern] = chr;
		++LenPattern;
	}
	unsigned char text[500];
	LenText = fread(text, 1, 200, stdin);
	/*for (int i = 0; i < LenText; ++i){
		text[i] = input[i];
	}*/
	//printf("text is %s\n", text);
	//printf("LenText is %d\n",LenText);
	if (LenText == 0) { printf("0"); return 0; }
	for (int i = 0; i < LenPattern; ++i) {
		WeightPattern += FindWeight(pattern[i], i);
	}
	printf("%d ", WeightPattern);
	Check(LenText, LenPattern, pattern, text, WeightPattern);
	return 0;
}
