#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 100000

void Check(bool done, int icheck, int lenText, int lenPattern, unsigned char* pattern, unsigned char* text, int* imatrix, int index) {

	while (!done && icheck < lenText) {
		bool coincidence = 1;
		int i = 0;
		printf("%d ", icheck + index);
		if (text[icheck] != pattern[lenPattern - 1]) {
			coincidence = 0;
			if (icheck == lenText - 1) {
				done = 1;
			}
			else {
				icheck += imatrix[(int)text[icheck]];
				if (icheck > lenText - 1) {
					icheck = lenText - 1;
				}
			}
		}
		for (i = 1; i < lenPattern && coincidence; ++i) {
			printf("%d ", icheck + index - i);
			if (text[icheck - i] != pattern[lenPattern - 1 - i]) {
				coincidence = 0;
				if (icheck == lenText - 1) {
					done = 1;
				}
				else {
					icheck += imatrix[text[icheck]];
					if (icheck > lenText - 1) {
						icheck = lenText - 1;
					}
				}

			}
		}
		if (i == lenPattern && coincidence) {
			icheck += imatrix[pattern[lenPattern - 1]];
		}
	}
}

int main(void) {
	int lenText = 0, lenPattern = 0, index = 1;
	unsigned char text[SIZE], chr, pattern[16];
	while ((chr = getchar()) != '\n') {
		pattern[lenPattern] = chr;
		++lenPattern;
	}
	lenText = fread(text, 1, SIZE, stdin);
	if (lenText == 0) { return 0; }
	int imatrix[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		imatrix[i] = lenPattern;
	}
	for (int i = lenPattern - 2; i >= 0; --i) {
		int PatternNow = pattern[i];
		if (imatrix[PatternNow] == lenPattern) {
			imatrix[PatternNow] = lenPattern - i - 1;
		}
	}
	Check(0, lenPattern - 1, lenText, lenPattern, pattern, text, imatrix, index);
	if (lenText == SIZE) {
		while (lenText == SIZE) {
			index += lenText;
			lenText = fread(text, 1, SIZE, stdin);
			Check(0, lenPattern - 1, lenText, lenPattern, pattern, text, imatrix, index);
		}
	}
	return 0;
}
