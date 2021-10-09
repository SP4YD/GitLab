#include <stdio.h>
#define SIZE 256

int FindWeight(int symbol, int n){
	int a = 1;
	for (int i = 0; i < n; ++i){
		a *= 3;
	}
	return ((symbol%3)*a);
}

int Compare(){
    
}

void Check(int LenText, int LenPattern, int* pattern, int* text, int WeightPattern) {
	int WeightPartText = 0, icheck = 0;
	for (int i = 0; i < LenPattern; ++i){
		WeightPattern += FindWeight(text[i], i);
	}	
	while (icheck < LenText){
		if (WeightPattern == WeightPartText){
			
		}
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
	for (int i = 0; i < LenPattern; ++i){
		WeightPattern += FindWeight(pattern[i], i);
	}
	printf("%d ", WeightPattern);
	return 0;
}
