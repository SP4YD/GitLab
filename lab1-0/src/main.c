#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int Check(bool done, int icheck, int lenPattern, unsigned char* pattern, unsigned char* text, int* imatrix, int index) {
	while (!done && icheck < lenPattern) {
		bool coincidence = 1;
		int i = 0;
		printf("%d ", icheck + index);
		//printf("\n ichek is %d, index is %d\n", icheck, index);
		if (text[icheck] != pattern[lenPattern - 1]) {
			coincidence = 0;
			if (icheck == lenPattern - 1) {
				done = 1;
			}
			else {
				icheck += imatrix[(int)text[icheck]];
				if (icheck > lenPattern - 1) {
					icheck = lenPattern - 1;
				}
			}
		}
		for (i = 1; i < lenPattern && coincidence; ++i) {
			printf("%d ", icheck + index - i);
			if (text[icheck - i] != pattern[lenPattern - 1 - i]) {
				coincidence = 0;
				if (icheck == lenPattern - 1) {
					done = 1;
				}
				else {
					icheck += imatrix[text[icheck]];
					if (icheck > lenPattern - 1) {
						icheck = lenPattern - 1;
					}
				}

			}
		}
		if (i == lenPattern && coincidence) {
			icheck += imatrix[pattern[lenPattern - 1]];
		}
	}
	return imatrix[text[icheck]];
}

int main(void) {
	int lenText = 0, lenPattern = 0, index = 1;
	unsigned char text[16], chr, pattern[16], input[16];
	while ((chr = getchar()) != '\n') {
		pattern[lenPattern] = chr;
		++lenPattern;
	}
	lenText = fread(text, 1, lenPattern, stdin);
	if (lenText == 0) { return 0; }
	int imatrix[256];
	for (int i = 0; i < 256; ++i) {
		imatrix[i] = lenPattern;
	}
	for (int i = lenPattern - 2; i >= 0; --i) {
		int PatternNow = pattern[i];
		if (imatrix[PatternNow] == lenPattern) {
			imatrix[PatternNow] = lenPattern - i - 1;
		}
	}
	index += Check (0, lenPattern - 1, lenPattern, pattern, text, imatrix, index) - 1;
	bool flagEOF = 0;
	int indexInput = index;
	while (!flagEOF) {
		printf("\nindexIn is %d\n", indexInput);
		lenText = fread(input, 1, indexInput, stdin);
		printf("\ninput is %s\n", input);
		printf("\nlenText is %d\n", lenText);
		if (lenText < indexInput || input[lenText - 1] == (unsigned int)EOF) {
			flagEOF = 1;
			if (lenText == 0){
				continue;
			}
		}
		for (int i = 0; i < lenPattern - lenText; ++i) {
			pattern[i] = pattern[i + lenText];
		}
		for (int i = lenPattern - lenText; i < lenPattern; ++i) {
			pattern[i] = input[i - (lenPattern - lenText)];
		}
		//printf("\n Начальный ленпатерн - 1 = %d\n", lenPattern - 1);
		indexInput = Check(0, lenPattern - 1, lenPattern, pattern, text, imatrix, index + 1) - 1;
		index += indexInput;
	}
	return 0;
}
