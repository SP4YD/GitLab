//#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <local.h>
#define SIZE 256

/*int Rus(int a) {
	if (a < 0) {
		a += 256;
	}
	return a;
}*/

void Check(bool done, int icheck, int LenText, int LenPattern, char *pattern, char *text, int *imatrix){
	
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
				icheck += imatrix[text[icheck]];
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
	setlocale(LC_ALL, "Rus");
	char pattern[SIZE] = "\0", text[SIZE] = "\0", chr;
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
	//text[LenText - 1] = '\0';
	//--LenText;
	char cmatrix[SIZE];
	int imatrix[SIZE];
	for (int i = 0; i < 256; ++i) {
		cmatrix[i] = i;
		imatrix[i] = LenPattern;
	}
	for (int i = LenPattern - 2; i >= 0; --i) {
		int pattern_now = pattern[i];
		if (imatrix[pattern_now] == LenPattern) {
			imatrix[pattern_now] = LenPattern - i - 1;
		}
	}
	//Check(0, LenPattern - 1, LenText, LenPattern, pattern, text, imatrix);
	//for (int i = 65; i < 256; ++i) { printf("%c) %d\n", i, imatrix[i]); } 
	return 0;
}
