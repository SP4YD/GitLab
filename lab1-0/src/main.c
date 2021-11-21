#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int Check(
	int lenPattern,
	unsigned char* pattern,
	unsigned char* text,
	int* imatrix,
	int index
) {
	int icheck = lenPattern - 1;
	printf("%d ", icheck + index);
	if (text[icheck] != pattern[icheck]) {
		return imatrix[(int)text[icheck]];
	}
	for (int i = lenPattern - 2; i >= 0; --i) {
		printf("%d ", i + index);
		if (text[i] != pattern[i]) {
			return imatrix[(int)text[icheck]];
		}
	}
	return imatrix[pattern[lenPattern - 1]];
}

void Shift(unsigned char* text, int shift, int len) {
	memmove(text, text + (shift * sizeof(unsigned char)), len - shift);
}

int Upadte(unsigned char* text, int shift, int len) {
	if (shift != len) {
		Shift(text, shift, len);
	}
	return fread(
		text + len - (shift * sizeof(unsigned char)),
		sizeof(unsigned char),
		shift,
		stdin
	);
}

int main(void) {
	int lenPattern = 0, index = 1;
	unsigned char chr, pattern[16], text[16];

	while ((chr = getchar()) != '\n') {
		pattern[lenPattern] = chr;
		++lenPattern;
	}

	int lenText = fread(text, sizeof(unsigned char), lenPattern, stdin);

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

	int shift = Check(lenPattern, pattern, text, imatrix, index);
	int lenRead = Upadte(text, shift, lenPattern);

	while (lenRead == shift) {
		index += lenRead;
		shift = Check(lenPattern, pattern, text, imatrix, index);
		lenRead = Upadte(text, shift, lenPattern);
	}

	return 0;
}
