#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 1000000

void Check(bool done, int icheck, int LenText, int LenPattern, unsigned char* pattern, unsigned char* text, int* imatrix, int index) {

	while (!done && icheck < LenText) {
		bool coincidence = 1;
		int i = 0;
		printf("%d ", icheck + index);
		if (text[icheck] != pattern[LenPattern - 1]) {
			coincidence = 0;
			if (icheck == LenText - 1) {
				done = 1;
			}
			else {
				icheck += imatrix[(int)text[icheck]];
				if (icheck > LenText - 1) {
					icheck = LenText - 1;
				}
			}
		}
		for (i = 1; i < LenPattern && coincidence; ++i) {
			printf("%d ", icheck + index - i);
			if (text[icheck - i] != pattern[LenPattern - 1 - i]) {
				coincidence = 0;
				if (icheck == LenText - 1) {
					done = 1;
				}
				else {
					icheck += imatrix[text[icheck]];
					if (icheck > LenText - 1) {
						icheck = LenText - 1;
					}
				}

			}
		}
		if (i == LenPattern && coincidence) {
			icheck += imatrix[pattern[LenPattern - 1]];
		}
	}
}

int main(void) {
	int LenText = 0, LenPattern = 0, index = 1;
	unsigned char text[SIZE], chr, pattern[16];
	while ((chr = getchar()) != '\n') {
		pattern[LenPattern] = chr;
		++LenPattern;
	}
	LenText = fread(text, 1, SIZE, stdin);
	if (LenText == 0) { return 0; }
	int imatrix[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		imatrix[i] = LenPattern;
	}
	for (int i = LenPattern - 2; i >= 0; --i) {
		int PatternNow = pattern[i];
		if (imatrix[PatternNow] == LenPattern) {
			imatrix[PatternNow] = LenPattern - i - 1;
		}
	}
	printf("\n");
	Check(0, LenPattern - 1, LenText, LenPattern, pattern, text, imatrix, index);
	if (LenText == SIZE) {
		while (LenText == SIZE) {
			index += LenText;
			LenText = fread(text, 1, SIZE, stdin);
			Check(0, LenPattern - 1, LenText, LenPattern, pattern, text, imatrix, index);
		}
	}
	return 0;
}
