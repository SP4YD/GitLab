//#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#define SIZE 256

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
	printf("%d\n", (int)('а'));
	printf("%d\n", (int)('А'));
	printf("%d\n", (int)('я'));
	printf("%d\n", (int)('Я'));
	return 0;
}
