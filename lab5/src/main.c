#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define IsNotSymbol 777

typedef struct TTree TTree;
typedef struct TElementAlTree TElementAlTree;
typedef struct TMainQueue TMainQueue;
typedef struct TQueueElement TQueueElement;

struct TTree {
    TTree* Left;
    TTree* Right;
    short Symbol;
    char Used;
    int Count;
};

struct TElementAlTree {
    char Code[256];
    int LenCode;
};

struct TMainQueue {
    struct TQueueElement {
        TTree* Root;
        TQueueElement* Next;
    }*Head;
    TQueueElement* Tail;
};

void LengthCheckAndPlusOne (int* lenCode, unsigned char* code) {
    ++(*lenCode);
    if (*lenCode > 7) {
        fprintf (stdout, "%c", *code);
        *code = 0;
        *lenCode = 0;
    }
}

void CheckSizeCodeAndPlusOne (char* codeSize, unsigned char* code) {
    ++(*codeSize);
    if (*codeSize > 7) {
        *codeSize = 0;
        if (fscanf (stdin, "%c", code) != 1) {
            return;
        }
    }
}

void CodingSymbol (int* lenCode, unsigned char* code, unsigned char symbol) {
    for (int i = 0; i < 8; ++i) {
        if (symbol & 1 << (7 - i)) {
            *code |= 1 << (7 - *lenCode);
        }
        LengthCheckAndPlusOne (lenCode, code);
    }
}

void PushQueue (TMainQueue* mainQueue, TTree* root) {
    TQueueElement* newElement = calloc (1, sizeof (TQueueElement));

    newElement->Root = root;
    newElement->Next = NULL;

    if (mainQueue->Head) {
        if (!mainQueue->Tail) {
            TQueueElement* temp;
            temp = mainQueue->Head;

            while (temp->Next) {
                temp = temp->Next;
            }
        }
        mainQueue->Tail->Next = newElement;
    } 
    else {
        mainQueue->Head = newElement;
    }

    mainQueue->Tail = newElement;
}

void PopQueue (TMainQueue* mainQueue, TTree** root) {
    if (mainQueue->Head == NULL) {
        return;
    }

    TQueueElement* element = mainQueue->Head->Next;
    *root = mainQueue->Head->Root;
    free (mainQueue->Head);
    mainQueue->Head = element;
}

void ClearingQueue (TMainQueue mainQueue) {
    while (mainQueue.Head) {
        TTree* rootNow;
        PopQueue (&mainQueue, &rootNow);
    }
}

void FreeTree (TTree* fullTree) {
    if (fullTree) {
        if (fullTree->Left) {
            FreeTree (fullTree->Left);
        }

        if (fullTree->Right) {
            FreeTree (fullTree->Right);
        }
        free (fullTree);
    }
}

void PrintCodeTree (TTree fullTree, int sizeTree) { //BFS
    TMainQueue mainQueue;
    int lenCode = 0;
    unsigned char code = 0;
    fprintf (stdout, "%c", sizeTree - 1);

    mainQueue.Head = mainQueue.Tail = NULL;
    PushQueue (&mainQueue, &fullTree);

    if (fullTree.Symbol == IsNotSymbol) {
        int lenOutput = 0;
        ++lenCode;

        while (lenOutput < sizeTree) {
            TTree* rootNow = NULL;
            PopQueue (&mainQueue, &rootNow);

            if (rootNow) {
                if (rootNow->Left) {
                    if (rootNow->Left->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        LengthCheckAndPlusOne (&lenCode, &code);
                        CodingSymbol (&lenCode, &code, (unsigned char)rootNow->Left->Symbol);
                        ++lenOutput;
                    } 
                    else {
                        LengthCheckAndPlusOne (&lenCode, &code);
                    }
                    PushQueue (&mainQueue, rootNow->Left);
                }
            }

            if (rootNow) {
                if (rootNow->Right && lenOutput < sizeTree) {
                    if (rootNow->Right->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        LengthCheckAndPlusOne (&lenCode, &code);
                        CodingSymbol (&lenCode, &code, (unsigned char)rootNow->Right->Symbol);
                        ++lenOutput;
                    } 
                    else {
                        LengthCheckAndPlusOne (&lenCode, &code);
                    }
                    PushQueue (&mainQueue, rootNow->Right);
                }
            }
        }
    } 
    else {
        code |= 1 << (7 - lenCode);
        LengthCheckAndPlusOne (&lenCode, &code);
        CodingSymbol (&lenCode, &code, (unsigned char)fullTree.Symbol);
    }

    if (lenCode) {
        fprintf (stdout, "%c", code);
    }

    ClearingQueue (mainQueue);
}

void SearchMin (TTree* fullTree[512], int sizeTree, int* min1I, int* min2I) {
    int min1C = INT_MAX;
    int min2C = INT_MAX;

    for (int i = 0; i < sizeTree; ++i) {
        if (!fullTree[i]->Used) {
            if (fullTree[i]->Count < min1C) {
                min2C = min1C;
                *min2I = *min1I;
                min1C = fullTree[i]->Count;
                *min1I = i;
            } 
            else if (fullTree[i]->Count < min2C) {
                min2C = fullTree[i]->Count;
                *min2I = i;
            }
        }
    }
}

TTree* CombiningTrees (TTree* first, TTree* second) {
    TTree* NewTree = calloc (1, sizeof (TTree));
    NewTree->Count = first->Count + second->Count;
    NewTree->Left = first;
    NewTree->Right = second;
    NewTree->Used = 0;
    NewTree->Symbol = IsNotSymbol;

    return NewTree;
}

void CreateAllTree (int* alphabet, TTree** alphabetTree) {

    for (int i = 0, N = 0; i < 256; ++i) {
        if (alphabet[i]) {
            alphabetTree[N] = calloc (1, sizeof (TTree));
            alphabetTree[N]->Count = alphabet[i];
            alphabetTree[N]->Symbol = i;
            alphabetTree[N]->Left = alphabetTree[N]->Right = NULL;
            alphabetTree[N]->Used = 0;
            ++N;
        }
    }
}

TTree* AlgorithmHuffman (int sizeTree, int* alphabet) {
    TTree* alphabetTree[512];
    CreateAllTree (alphabet, alphabetTree);

    int min1Index = 0;
    int min2Index = 0;

    for (int i = 0, N = sizeTree; i < N - 1; ++i) {
        SearchMin (alphabetTree, sizeTree, &min1Index, &min2Index);

        alphabetTree[sizeTree] = CombiningTrees (alphabetTree[min1Index], alphabetTree[min2Index]);
        alphabetTree[min1Index]->Used = alphabetTree[min2Index]->Used = 1;

        sizeTree += 1;
    }

    return alphabetTree[sizeTree - 1];
}

void BuildingTree (char bit, TMainQueue* mainQueue, char symbol) {
    TTree* rootNow = NULL;
    PopQueue (mainQueue, &rootNow);

    if (rootNow) {
        rootNow->Count = rootNow->Used = 0;
        rootNow->Left = rootNow->Right = NULL;
        rootNow->Symbol = IsNotSymbol;

        if (bit == 0) {
            rootNow->Left = calloc (1, sizeof (TTree));
            rootNow->Right = calloc (1, sizeof (TTree));
            PushQueue (mainQueue, rootNow->Left);
            PushQueue (mainQueue, rootNow->Right);
        } 
        else {
            rootNow->Symbol = symbol;
        }
    }
}

TTree* ScanForBuildingTree (int lenInput) {
    TTree* fullTree = calloc (1, sizeof (TTree));
    fullTree->Count = fullTree->Used = 0;
    fullTree->Left = fullTree->Right = NULL;
    fullTree->Symbol = IsNotSymbol;

    TMainQueue mainQueue;
    mainQueue.Head = mainQueue.Tail = NULL;
    PushQueue (&mainQueue, fullTree);

    int lenText = 0;
    char codeSize = 0;
    unsigned char code;

    if (fscanf (stdin, "%c", &code) != 1) {
        return 0;
    }

    while (lenText < lenInput) {
        char j = 0;
        unsigned char symbol = 0;

        while ((code & 1 << (7 - codeSize)) < 1) {
            BuildingTree (0, &mainQueue, 0);
            CheckSizeCodeAndPlusOne (&codeSize, &code);
        }

        CheckSizeCodeAndPlusOne (&codeSize, &code);

        while (j < 8) {
            if (code & 1 << (7 - codeSize)) {
                symbol |= 1 << (7 - j);
            }

            ++j;
            CheckSizeCodeAndPlusOne (&codeSize, &code);
        }

        BuildingTree (1, &mainQueue, symbol);

        ++lenText;
    }

    ClearingQueue (mainQueue);

    return fullTree;
}

void CalculatingTheCode (TTree treeNow, TElementAlTree* alphabet, TElementAlTree elementNow) {
    ++elementNow.LenCode;

    if (treeNow.Left) {
        elementNow.Code[elementNow.LenCode - 1] = '0';
        CalculatingTheCode (*treeNow.Left, alphabet, elementNow);
    }

    if (treeNow.Right) {
        elementNow.Code[elementNow.LenCode - 1] = '1';
        CalculatingTheCode (*treeNow.Right, alphabet, elementNow);
    }

    if (treeNow.Symbol < IsNotSymbol) {

        elementNow.Code[elementNow.LenCode - 1] = '\0';
        alphabet[treeNow.Symbol] = elementNow;

        return;
    }
}

void FindAndPrintCodeSymbols (TTree* fullTree) {
    TTree* treeNow = fullTree;
    int twoInDegree;
    unsigned char countBitInLast;
    unsigned char code;

    int placeNow = ftell (stdin);
    fseek (stdin, -1, SEEK_END);
    int lastByte = ftell (stdin);

    if (fscanf (stdin, "%c", &countBitInLast) != 1) {
        return;
    }

    fseek (stdin, placeNow, SEEK_SET);

    while (ftell (stdin) + 1 < lastByte) {
        twoInDegree = 7;

        if (fscanf (stdin, "%c", &code) != 1) {
            return;
        }

        while (twoInDegree >= 0) {
            if (treeNow->Symbol < IsNotSymbol) {
                printf ("%c", treeNow->Symbol);

                treeNow = fullTree;
            }

            if ((code & 1 << (twoInDegree)) == 0) {
                treeNow = treeNow->Left;
            } 
            else {
                treeNow = treeNow->Right;
            }

            --twoInDegree;
        }
    }

    twoInDegree = 7;

    if (fscanf (stdin, "%c", &code) != 1) {
        return;
    }

    while (twoInDegree >= 8 - countBitInLast) {
        if (treeNow->Symbol < IsNotSymbol) {
            printf ("%c", treeNow->Symbol);
            treeNow = fullTree;
        }

        if ((code & 1 << (twoInDegree)) == 0) {
            treeNow = treeNow->Left;
        } 
        else {
            treeNow = treeNow->Right;
        }

        --twoInDegree;
    }

    if (treeNow->Symbol < IsNotSymbol) {
        printf ("%c", treeNow->Symbol);
    }
}

void Coding (void) {
    unsigned char inputSymbol;
    int alphabet[256] = {0};
    int lenText = 0;

    while (fscanf (stdin, "%c", &inputSymbol) == 1) {
        ++alphabet[inputSymbol];
        ++lenText;
    }

    if (lenText == 0) {
        return;
    }

    TElementAlTree* alphabetCodes = calloc (256, sizeof (TElementAlTree));
    TElementAlTree elementNow;
    int sizeTree = 0;
    int singleSymbol = 0;
    elementNow.LenCode = 0;

    for (int i = 0; i < 256; ++i) {
        if (alphabet[i]) {
            ++sizeTree;
            singleSymbol = i;
        }
    }

    if (sizeTree < 2) {
        fprintf (stdout, "%c", sizeTree - 1);

        fprintf (stdout, "%c", singleSymbol);

        fprintf (stdout, "%d", lenText);
    } else {
        TTree* fullTree = AlgorithmHuffman (sizeTree, alphabet);

        PrintCodeTree (*fullTree, sizeTree);

        CalculatingTheCode (*fullTree, alphabetCodes, elementNow);

        fseek (stdin, 1, SEEK_SET);

        int lenNow = 0;
        unsigned char code = 0;
        char j = 0;

        while (lenNow < lenText) {
            if (fscanf (stdin, "%c", &inputSymbol) != 1) {
                return;
            }
            ++lenNow;

            char* strCodeNow = alphabetCodes[(int)inputSymbol].Code;
            int z = 0;

            while (strCodeNow[z] != '\0') {
                if (strCodeNow[z] == '1') {
                    code |= 128 >> j;
                }

                ++j;
                ++z;

                if (j > 7) {

                    printf ("%c", code);

                    j = 0;
                    code = 0;
                }
            }
        }

        printf ("%c%c", code, j);

        FreeTree (fullTree);
    }

    free (alphabetCodes);
}

void Decoding (void) {
    int lenInput = 0;
    unsigned char code = 0;

    if (fscanf (stdin, "%c", &code) != 1) { //Не получается считать char в int
        return;
    }
    lenInput = code + 1;

    if (lenInput < 2) {
        int countPrint = 0;

        if (fscanf (stdin, "%c", &code) != 1) {
            return;
        }

        if (fscanf (stdin, "%d", &countPrint) != 1) {
            return;
        }

        for (int i = 0; i < countPrint; ++i) {
            fprintf (stdout, "%c", code);
        }
    } else {
        TTree* fullTree;

        fullTree = ScanForBuildingTree (lenInput);

        FindAndPrintCodeSymbols (fullTree);

        FreeTree (fullTree);
    }
}

int main (void) {
    unsigned char task = 0;

    if (fscanf (stdin, "%c", &task) != 1) {
        return 1;
    }

    if (task == 'c') {
        Coding ();
    } 
    else {
        Decoding();
    }

    return 0;
}
