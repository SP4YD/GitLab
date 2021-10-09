#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 256

void Check(bool done, int icheck, int LenText, int LenPattern, int* pattern, int* text, int* imatrix) {

	while (!done && icheck < LenText) {
		bool coincidence = 1;
		int i = 0;
		printf("%d ", icheck + 1);
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
			printf("%d ", icheck + 1 - i);
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
	int pattern[SIZE], text[SIZE], chr;
	int LenText = 0, LenPattern = 0;
	while ((chr = getc(stdin)) != '\n') {
		pattern[LenPattern] = chr;
		++LenPattern;
	}
	while ((chr = getc(stdin)) != EOF) {
		text[LenText] = chr;
		++LenText;
	}
	if (LenText == 0 && chr == EOF) { return 0; }
	int imatrix[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		imatrix[i] = LenPattern;
	}
	for (int i = LenPattern - 2; i >= 0; --i) {
		int pattern_now = pattern[i];
		if (imatrix[pattern_now] == LenPattern) {
			imatrix[pattern_now] = LenPattern - i - 1;
		}
	}
	Check(0, LenPattern - 1, LenText, LenPattern, pattern, text, imatrix);
	return 0;
}
