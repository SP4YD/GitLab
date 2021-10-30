#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int FindWeight(int symbol, int degree) {
    return (((symbol) % 3) * degree);
}

void Compare(int lenPattern, unsigned char* pattern, unsigned char* text, int weightPattern, int weightText, int index) {
    if (weightPattern == weightText) {
        for (int i = 0; i < lenPattern; ++i) {
            printf("%d ", i + index);
            if (pattern[i] != text[i]) {
                break;
            }
        }
    }
}

void UpadteWeightText(int* weightText, unsigned char* text, int degreeLastSymb, int lenPattern) {
    *weightText = (*weightText - FindWeight(text[0], 0)) / 3 + FindWeight(text[lenPattern - 1], degreeLastSymb);
}

void Shift(unsigned char* text, int lenPattern) {
    memcpy(text, text + sizeof(unsigned char), lenPattern - 1);
}

void Check(int lenPattern, int lenText, unsigned char* text, unsigned char* pattern) {
    int weightPattern = 0, index = 1, weightText = 0, degreeLastSymb = pow(3, lenPattern - 1);;
    for (int i = 0, degree = 1; i < lenPattern; ++i) {
        weightText += FindWeight(text[i], degree);
        weightPattern += FindWeight(pattern[i], degree);
        degree *= 3;
    }
    printf("%d ", weightPattern);
    Compare(lenPattern, pattern, text, weightPattern, weightText, index);
    unsigned char newText[1];
    Shift(text, lenPattern);
    lenText = fread(&text[lenPattern-1], sizeof(unsigned char), 1, stdin);
    while (lenText) {
        ++index;
        UpadteWeightText(&weightText, text, degreeLastSymb, lenPattern);
        Compare(lenPattern, pattern, text, weightPattern, weightText, index);
        Shift(text, lenPattern);
        lenText = fread(&text[lenPattern - 1], sizeof(unsigned char), 1, stdin);
    }
}

int main(void) {
    int lenText, lenPattern = 0;
    unsigned char pattern[17], text[17];
    if (scanf("%16[^\n]s", pattern) != 1) { return 0; }
    for (int i = 0; pattern[i] != '\0'; ++i) {
        ++lenPattern;
    }
    lenText = fread(text, sizeof(unsigned char), 1, stdin); //считываение лишнего \n между pattern и text
    lenText = fread(text, sizeof(unsigned char), lenPattern, stdin);
    if (lenText == 0 || lenText < lenPattern) { printf("0"); return 0; }
    Check(lenPattern, lenText, text, pattern);
    return 0;
}
