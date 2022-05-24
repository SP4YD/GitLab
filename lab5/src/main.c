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

void LengthCheckAndPlusOne(int* lenCode, unsigned char* code) {
    ++(*lenCode);
    if (*lenCode > 7) {
        fprintf(stdout, "%c", *code);
        *code = 0;
        *lenCode = 0;
    }
}

void CheckSizeCodeAndPlusOne(char* codeSize, unsigned char* code) {
    ++(*codeSize);
    if (*codeSize > 7) {
        *codeSize = 0;
        if (fscanf(stdin, "%c", code) != 1) {
            return;
        }
    }
}

void CodingSymbol(int* lenCode, unsigned char* code, unsigned char symbol) {
    for (int i = 0; i < 8; ++i) {
        if (symbol & 1 << (7 - i)) {
            *code |= 1 << (7 - *lenCode);
        }
        LengthCheckAndPlusOne(lenCode, code);
    }
}

void PushQueue(TMainQueue* MainQueue, TTree* root) {
    TQueueElement* newElement = calloc(1, sizeof(TQueueElement));

    newElement->Root = root;
    newElement->Next = NULL;

    if (MainQueue->Head) {
        if (!MainQueue->Tail) {
            TQueueElement* temp;
            temp = MainQueue->Head;

            while (temp->Next) {
                temp = temp->Next;
            }
        }
        MainQueue->Tail->Next = newElement;
    } else {
        MainQueue->Head = newElement;
    }

    MainQueue->Tail = newElement;
}

void PopQueue(TMainQueue* MainQueue, TTree** root) {
    if (MainQueue->Head == NULL) {
        return;
    }

    TQueueElement* element = MainQueue->Head->Next;
    *root = MainQueue->Head->Root;
    free(MainQueue->Head);
    MainQueue->Head = element;
}

void ClearingQueue(TMainQueue MainQueue) {
    while (MainQueue.Head) {
        TTree* RootNow;
        PopQueue(&MainQueue, &RootNow);
    }
}

void FreeTree(TTree* FullTree) {
    if (FullTree) {
        if (FullTree->Left) {
            FreeTree(FullTree->Left);
        }

        if (FullTree->Right) {
            FreeTree(FullTree->Right);
        }
        free(FullTree);
    }
}

void PrintCodeTree(TTree FullTree, int sizeTree) { //BFS
    TMainQueue MainQueue;
    int lenCode = 0;
    unsigned char code = 0;
    fprintf(stdout, "%c", sizeTree - 1);

    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue(&MainQueue, &FullTree);

    if (FullTree.Symbol == IsNotSymbol) {
        int lenOutput = 0;
        ++lenCode;

        while (lenOutput < sizeTree) {
            TTree* RootNow = NULL;
            PopQueue(&MainQueue, &RootNow);

            if (RootNow) {
                if (RootNow->Left) {
                    if (RootNow->Left->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        LengthCheckAndPlusOne(&lenCode, &code);
                        CodingSymbol(&lenCode, &code, (unsigned char)RootNow->Left->Symbol);
                        ++lenOutput;
                    } else {
                        LengthCheckAndPlusOne(&lenCode, &code);
                    }
                    PushQueue(&MainQueue, RootNow->Left);
                }
            }

            if (RootNow) {
                if (RootNow->Right && lenOutput < sizeTree) {
                    if (RootNow->Right->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        LengthCheckAndPlusOne(&lenCode, &code);
                        CodingSymbol(&lenCode, &code, (unsigned char)RootNow->Right->Symbol);
                        ++lenOutput;
                    } else {
                        LengthCheckAndPlusOne(&lenCode, &code);
                    }
                    PushQueue(&MainQueue, RootNow->Right);
                }
            }
        }
    } else {
        code |= 1 << (7 - lenCode);
        LengthCheckAndPlusOne(&lenCode, &code);
        CodingSymbol(&lenCode, &code, (unsigned char)FullTree.Symbol);
    }

    if (lenCode) {
        fprintf(stdout, "%c", code);
    }

    ClearingQueue(MainQueue);
}

void SearchMin(TTree* FullTree[512], int sizeTree, int* min1I, int* min2I) {
    int min1C = INT_MAX;
    int min2C = INT_MAX;

    for (int i = 0; i < sizeTree; ++i) {
        if (!FullTree[i]->Used) {
            if (FullTree[i]->Count < min1C) {
                min2C = min1C;
                *min2I = *min1I;
                min1C = FullTree[i]->Count;
                *min1I = i;
            } else if (FullTree[i]->Count < min2C) {
                min2C = FullTree[i]->Count;
                *min2I = i;
            }
        }
    }
}

TTree* CombiningTrees(TTree* First, TTree* Second) {
    TTree* NewTree = calloc(1, sizeof(TTree));
    NewTree->Count = First->Count + Second->Count;
    NewTree->Left = First;
    NewTree->Right = Second;
    NewTree->Used = 0;
    NewTree->Symbol = IsNotSymbol;

    return NewTree;
}

void CreateAllTree(int* alphabet, TTree** AlphabetTree) {

    for (int i = 0, N = 0; i < 256; ++i) {
        if (alphabet[i]) {
            AlphabetTree[N] = calloc(1, sizeof(TTree));
            AlphabetTree[N]->Count = alphabet[i];
            AlphabetTree[N]->Symbol = i;
            AlphabetTree[N]->Left = AlphabetTree[N]->Right = NULL;
            AlphabetTree[N]->Used = 0;
            ++N;
        }
    }
}

TTree* AlgorithmHuffman(int sizeTree, int* alphabet) {
    TTree* AlphabetTree[512];
    CreateAllTree(alphabet, AlphabetTree);

    int min1Index = 0;
    int min2Index = 0;

    for (int i = 0, N = sizeTree; i < N - 1; ++i) {
        SearchMin(AlphabetTree, sizeTree, &min1Index, &min2Index);

        AlphabetTree[sizeTree] = CombiningTrees(AlphabetTree[min1Index], AlphabetTree[min2Index]);
        AlphabetTree[min1Index]->Used = AlphabetTree[min2Index]->Used = 1;

        sizeTree += 1;
    }

    return AlphabetTree[sizeTree - 1];
}

void BuildingTree(char bit, TMainQueue* MainQueue, char symbol) {
    TTree* RootNow = NULL;
    PopQueue(MainQueue, &RootNow);

    if (RootNow) {
        RootNow->Count = RootNow->Used = 0;
        RootNow->Left = RootNow->Right = NULL;
        RootNow->Symbol = IsNotSymbol;

        if (bit == 0) {
            RootNow->Left = calloc(1, sizeof(TTree));
            RootNow->Right = calloc(1, sizeof(TTree));
            PushQueue(MainQueue, RootNow->Left);
            PushQueue(MainQueue, RootNow->Right);
        } else {
            RootNow->Symbol = symbol;
        }
    }
}

TTree* ScanForBuildingTree(int lenInput) {
    TTree* FullTree = calloc(1, sizeof(TTree));
    FullTree->Count = FullTree->Used = 0;
    FullTree->Left = FullTree->Right = NULL;
    FullTree->Symbol = IsNotSymbol;

    TMainQueue MainQueue;
    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue(&MainQueue, FullTree);

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
            BuildingTree(0, &MainQueue, 0);
            CheckSizeCodeAndPlusOne(&codeSize, &code);
        }

        CheckSizeCodeAndPlusOne(&codeSize, &code);

        while (j < 8) {
            if (code & 1 << (7 - codeSize)) {
                symbol |= 1 << (7 - j);
            }

            ++j;
            CheckSizeCodeAndPlusOne(&codeSize, &code);
        }

        BuildingTree(1, &MainQueue, symbol);

        ++lenText;
    }

    ClearingQueue(MainQueue);

    return FullTree;
}

void CalculatingTheCode(TTree TreeNow, TElementAlTree* Alphabet, TElementAlTree ElementNow) {
    ++ElementNow.LenCode;

    if (TreeNow.Left) {
        ElementNow.Code[ElementNow.LenCode - 1] = '0';
        CalculatingTheCode(*TreeNow.Left, Alphabet, ElementNow);
    }

    if (TreeNow.Right) {
        ElementNow.Code[ElementNow.LenCode - 1] = '1';
        CalculatingTheCode(*TreeNow.Right, Alphabet, ElementNow);
    }

    if (TreeNow.Symbol < IsNotSymbol) {

        ElementNow.Code[ElementNow.LenCode - 1] = '\0';
        Alphabet[TreeNow.Symbol] = ElementNow;

        return;
    }
}

void FindAndPrintCodeSymbols(TTree* FullTree) {
    TTree* TreeNow = FullTree;
    int twoInDegree;
    unsigned char countBitInLast;
    unsigned char code;

    int placeNow = ftell(stdin);
    fseek(stdin, -1, SEEK_END);
    int lastByte = ftell(stdin);

    if (fscanf(stdin, "%c", &countBitInLast) != 1) {
        return;
    }

    fseek (stdin, placeNow, SEEK_SET);

    while (ftell(stdin) + 1 < lastByte) {
        twoInDegree = 7;

        if (fscanf(stdin, "%c", &code) != 1) {
            return;
        }

        while (twoInDegree >= 0) {
            if (TreeNow->Symbol < IsNotSymbol) {
                printf("%c", TreeNow->Symbol);

                TreeNow = FullTree;
            }

            if ((code & 1 << (twoInDegree)) == 0) {
                TreeNow = TreeNow->Left;
            } else {
                TreeNow = TreeNow->Right;
            }

            --twoInDegree;
        }
    }

    twoInDegree = 7;

    if (fscanf(stdin, "%c", &code) != 1) {
        return;
    }

    while (twoInDegree >= 8 - countBitInLast) {
        if (TreeNow->Symbol < IsNotSymbol) {
            printf("%c", TreeNow->Symbol);
            TreeNow = FullTree;
        }

        if ((code & 1 << (twoInDegree)) == 0) {
            TreeNow = TreeNow->Left;
        } else {
            TreeNow = TreeNow->Right;
        }

        --twoInDegree;
    }

    if (TreeNow->Symbol < IsNotSymbol) {
        printf("%c", TreeNow->Symbol);
    }
}

/// ////////////////////////////// Заменить main на две функции
////////////////////////////////// переменные с маленькой
////////////////////////////////// УБрать string из декодировки

int main(void) {
    unsigned char inputSymbol;
    unsigned char task = 0;

    //freopen("in.txt", "rb", stdin); freopen("out.txt", "wb", stdout);
    //char a = 0, b = 1; fprintf (stdout, "c"); 
    //fprintf (stdout, "%c", '\n');
    //fprintf (stdout, "%c", '\r');
    //fprintf (stdout, "%c", '\n');
    //fprintf (stdout, "%c", '\r'); return 0;

    //fprintf (stdout, "c");
    //for (int i = 0; i < 256; ++i) {
    //    fprintf (stdout, "%c", i);
    //} return 0;

    if (fscanf(stdin, "%c", &task) != 1) {
        return 1;
    }

    if (task == 'c') {
        int alphabet[256] = {0};
        int lenText = 0;

        while (fscanf(stdin, "%c", &inputSymbol) == 1) {
            ++alphabet[inputSymbol];
            ++lenText;
        }

        if (lenText == 0) {
            return 0;
        }

        TElementAlTree* AlphabetCodes = calloc(256, sizeof(TElementAlTree));
        TElementAlTree ElementNow;
        int sizeTree = 0;
        int singleSymbol = 0;
        ElementNow.LenCode = 0;

        for (int i = 0; i < 256; ++i) {
            if (alphabet[i]) {
                ++sizeTree;
                singleSymbol = i;
            }
        }

        if (sizeTree < 2) {
            fprintf(stdout, "%c", sizeTree - 1);

            fprintf(stdout, "%c", singleSymbol);

            fprintf(stdout, "%d", lenText);

        } else {
            TTree* FullTree = AlgorithmHuffman(sizeTree, alphabet);

            PrintCodeTree(*FullTree, sizeTree);

            CalculatingTheCode(*FullTree, AlphabetCodes, ElementNow);

            fseek(stdin, 1, SEEK_SET);

            int lenNow = 0;
            unsigned char code = 0;
            char j = 0;

            while (lenNow < lenText) {
                if (fscanf(stdin, "%c", &inputSymbol) != 1) {
                    return 0;
                }
                ++lenNow;

                char* strCodeNow = AlphabetCodes[(int)inputSymbol].Code;
                int z = 0;

                while (strCodeNow[z] != '\0') {
                    if (strCodeNow[z] == '1') {
                        code |= 128 >> j;
                    }

                    ++j;
                    ++z;

                    if (j > 7) {

                        printf("%c", code);

                        j = 0;
                        code = 0;
                    }
                }
            }

            printf("%c%c", code, j);

            FreeTree(FullTree);
        }

        free(AlphabetCodes);

    } else {
        int lenInput = 0;
        unsigned char code = 0;

        if (fscanf(stdin, "%c", &code) != 1) { //Не получается считать char в int
            return 0;
        }
        lenInput = code + 1;

        if (lenInput < 2) {
            int countPrint = 0;

            if (fscanf (stdin, "%c", &code) != 1) {
                return 0;
            }

            if (fscanf(stdin, "%d", &countPrint) != 1) {
                return 0;
            }

            for (int i = 0; i < countPrint; ++i) {
                fprintf(stdout, "%c", code);
            }
        } else {
            TTree* FullTree;

            FullTree = ScanForBuildingTree (lenInput);

            FindAndPrintCodeSymbols(FullTree);

            FreeTree(FullTree);
        }
    }
    
    return 0;
}
