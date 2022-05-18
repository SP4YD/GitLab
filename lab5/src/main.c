#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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
    char Code[1000];
    int LenCode;
};

struct TMainQueue {
    struct TQueueElement {
        TTree* Root;
        TQueueElement* Next;
    }*Head;
    TQueueElement* Tail;
};

unsigned char LengthCheckAndPlusOne (int* lenCode, unsigned char code) {
    ++(*lenCode);
    if (*lenCode > 7) {
        fprintf (stdout, "%c", code);
        code = 0;
        *lenCode = 0;
    }

    return code;
}

unsigned char CheckSizeCodeAndPlusOne (int* codeSize, unsigned char code) {
    ++(*codeSize);
    if (*codeSize > 7) {
        *codeSize = 0;
        if (fscanf (stdin, "%c", &code) != 1) {
            return 0;
        }
    }

    return code;
}

unsigned char CodingSymbol (int* lenCode, unsigned char code, char symbol) {
    for (int i = 0; i < 8; ++i) {
        if (symbol & 1 << (7 - i)) {
            code |= 1 << (7 - *lenCode);
        }
        code = LengthCheckAndPlusOne (lenCode, code);
    }

    return code;
}

void PushQueue (TMainQueue* MainQueue, TTree* root) {
    TQueueElement* newElement = calloc (1, sizeof (TQueueElement));

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

void PopQueue (TMainQueue* MainQueue, TTree** root) {
    if (MainQueue->Head == NULL) {
        return;
    }

    TQueueElement* element = MainQueue->Head->Next;
    *root = MainQueue->Head->Root;
    free (MainQueue->Head);
    MainQueue->Head = element;
}

void ClearingQueue (TMainQueue MainQueue) {
    while (MainQueue.Head) {
        TTree* RootNow;
        PopQueue (&MainQueue, &RootNow);
    }
}

void FreeTree (TTree* FullTree) {
    if (FullTree) {
        if (FullTree->Left) {
            FreeTree (FullTree->Left);
        }

        if (FullTree->Right) {
            FreeTree (FullTree->Right);
        }
        free (FullTree);
    }
}

void PrintCodeTree (TTree FullTree, int sizeTree) { //BFS
    TMainQueue MainQueue;
    int lenCode = 0;
    unsigned char code = 0;
    fprintf (stdout, "%c", sizeTree);

    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue (&MainQueue, &FullTree);

    if (FullTree.Symbol == IsNotSymbol) {
        int lenOutput = 0;
        ++lenCode;

        while (lenOutput < sizeTree) {
            TTree* RootNow = NULL;
            PopQueue (&MainQueue, &RootNow);

            if (RootNow) {
                if (RootNow->Left) {
                    if (RootNow->Left->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        code = LengthCheckAndPlusOne (&lenCode, code);
                        code = CodingSymbol (&lenCode, code, RootNow->Left->Symbol);
                        ++lenOutput;
                    } else {
                        code = LengthCheckAndPlusOne (&lenCode, code);
                    }
                } else {
                    code = LengthCheckAndPlusOne (&lenCode, code);
                }
                PushQueue (&MainQueue, RootNow->Left);
            }

            if (RootNow) {
                if (RootNow->Right && lenOutput < sizeTree) {
                    if (RootNow->Right->Symbol < IsNotSymbol) {
                        code |= 1 << (7 - lenCode);
                        code = LengthCheckAndPlusOne (&lenCode, code);
                        code = CodingSymbol (&lenCode, code, RootNow->Right->Symbol);
                        ++lenOutput;
                    } else {
                        code = LengthCheckAndPlusOne (&lenCode, code);
                    }
                } else {
                    code = LengthCheckAndPlusOne (&lenCode, code);
                }
                PushQueue (&MainQueue, RootNow->Right);
            }
        }
    } else {
        code |= 1 << (7 - lenCode);
        code = LengthCheckAndPlusOne (&lenCode, code);
        code = CodingSymbol (&lenCode, code, FullTree.Symbol);
    }

    if (lenCode) {
        fprintf (stdout, "%c", code);
    }

    ClearingQueue (MainQueue);
}

void SearchMin (TTree* FullTree[512], int sizeTree, int* min1I, int* min2I) {
    int min1C = 100000;
    int min2C = 100000;

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

TTree* CombiningTrees (TTree* First, TTree* Second) {
    TTree* NewTree = calloc (1, sizeof (TTree));
    NewTree->Count = First->Count + Second->Count;
    NewTree->Left = First;
    NewTree->Right = Second;
    NewTree->Used = 0;
    NewTree->Symbol = IsNotSymbol;

    return NewTree;
}

void CreateAllTree (int* alphabet, TTree** AlphabetTree) {

    for (int i = 0, N = 0; i < 256; ++i) {
        if (alphabet[i]) {
            AlphabetTree[N] = calloc (1, sizeof (TTree));
            AlphabetTree[N]->Count = alphabet[i];
            AlphabetTree[N]->Symbol = i;
            AlphabetTree[N]->Left = AlphabetTree[N]->Right = NULL;
            AlphabetTree[N]->Used = 0;
            ++N;
        }
    }
}

TTree* AlgorithmHuffman (int sizeTree, int* alphabet) {
    TTree* AlphabetTree[512];
    CreateAllTree (alphabet, AlphabetTree);

    int min1Index = 0;
    int min2Index = 0;

    for (int i = 0, N = sizeTree; i < N - 1; ++i) {
        SearchMin (AlphabetTree, sizeTree, &min1Index, &min2Index);

        AlphabetTree[sizeTree] = CombiningTrees (AlphabetTree[min1Index], AlphabetTree[min2Index]);
        AlphabetTree[min1Index]->Used = AlphabetTree[min2Index]->Used = 1;

        sizeTree += 1;
    }

    return AlphabetTree[sizeTree - 1];
}

TTree* BuildingTree (unsigned char* codeTree) {
    TTree* FullTree = calloc (1, sizeof (TTree));
    FullTree->Count = FullTree->Used = 0;
    FullTree->Left = FullTree->Right = NULL;
    FullTree->Symbol = IsNotSymbol;

    TMainQueue MainQueue;
    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue (&MainQueue, FullTree);

    for (int i = 0; codeTree[i] != '\0'; ++i) {
        TTree* RootNow = NULL;
        PopQueue (&MainQueue, &RootNow);

        if (RootNow) {
            RootNow->Count = RootNow->Used = 0;
            RootNow->Left = RootNow->Right = NULL;
            RootNow->Symbol = IsNotSymbol;

            if (codeTree[i] == '0') {
                RootNow->Left = calloc (1, sizeof (TTree));
                RootNow->Right = calloc (1, sizeof (TTree));
            } else {
                ++i;
                RootNow->Symbol = codeTree[i];
            }

            PushQueue (&MainQueue, RootNow->Left);
            PushQueue (&MainQueue, RootNow->Right);
        }
    }

    ClearingQueue (MainQueue);

    return FullTree;
}

void CalculatingTheCode (TTree TreeNow, TElementAlTree* Alphabet, TElementAlTree ElementNow) {
    ++ElementNow.LenCode;

    if (TreeNow.Left) {
        ElementNow.Code[ElementNow.LenCode - 1] = '0';
        CalculatingTheCode (*TreeNow.Left, Alphabet, ElementNow);
    }

    if (TreeNow.Right) {
        ElementNow.Code[ElementNow.LenCode - 1] = '1';
        CalculatingTheCode (*TreeNow.Right, Alphabet, ElementNow);
    }

    if (TreeNow.Symbol < IsNotSymbol) {

        ElementNow.Code[ElementNow.LenCode - 1] = '\0';
        Alphabet[TreeNow.Symbol] = ElementNow;

        return;
    }
}

void FindAndPrintCodeSymbols (TTree* FullTree, unsigned char* input, int lenText) {
    TTree* TreeNow = FullTree;
    unsigned char countBitInLast = input[lenText - 1];
    int twoInDegree;

    for (int i = 0; i < lenText - 2; ++i) {
        twoInDegree = 7;

        while (twoInDegree >= 0) {
            if (TreeNow->Symbol < IsNotSymbol) {
                printf ("%c", TreeNow->Symbol);
                TreeNow = FullTree;
            }

            if ((input[i] & 1 << (twoInDegree)) == 0) {
                TreeNow = TreeNow->Left;
            } else {
                TreeNow = TreeNow->Right;
            }

            --twoInDegree;
        }
    }

    twoInDegree = 7;

    while (twoInDegree >= 8 - countBitInLast) {
        if (TreeNow->Symbol < IsNotSymbol) {
            printf ("%c", TreeNow->Symbol);
            TreeNow = FullTree;
        }

        if ((input[lenText - 2] & 1 << (twoInDegree)) == 0) {
            TreeNow = TreeNow->Left;
        } else {
            TreeNow = TreeNow->Right;
        }

        --twoInDegree;
    }

    if (TreeNow->Symbol < IsNotSymbol) {
        printf ("%c", TreeNow->Symbol);
    }
}

/// ////////////////////////////// Заменить main на две функции
//////////////////// Переделать code на void

int main (void) {
    unsigned char str[100000] = {'\0'};
    unsigned char task = 0;

    //freopen ("in.txt", "rb", stdin); freopen ("out.txt", "wb", stdout);
    //char a = 0, b = 1; fprintf (stdout, "c"); 
    //fprintf (stdout, "%c", '\n');
    //fprintf (stdout, "%c", '\r');
    //fprintf (stdout, "%c", '\n');
    //fprintf (stdout, "%c", '\r'); return 0;

    if (fscanf (stdin, "%c", &task) != 1) {
        return 1;
    }

    if (task == 'c') {
        int alphabet[256] = {0};
        int lenText = 0;

        while (fscanf (stdin, "%c", &str[lenText]) == 1) {
            ++lenText;
        }

        if (lenText == 0) {
            return 0;
        }

        for (int i = 0; i < lenText; ++i) {
            ++alphabet[str[i]];
        }

        int sizeTree = 0;
        int singleSymbol = 0;
        TElementAlTree* AlphabetCodes = calloc (256, sizeof (TElementAlTree));
        TElementAlTree ElementNow;
        ElementNow.LenCode = 0;

        for (int i = 0; i < 256; ++i) {
            if (alphabet[i]) {
                ++sizeTree;
                singleSymbol = i;
            }
        }

        if (sizeTree < 2) {
            unsigned char code = 0;
            int lenCode = 0;
            fprintf (stdout, "%c", sizeTree);
            code |= 1 << (7 - lenCode);
            code = LengthCheckAndPlusOne (&lenCode, code);
            code = CodingSymbol (&lenCode, code, singleSymbol);
            if (lenCode) {
                fprintf (stdout, "%c", code);
            }
            fprintf (stdout, "%c", lenText);

        } else {
            TTree* FullTree = AlgorithmHuffman (sizeTree, alphabet);

            PrintCodeTree (*FullTree, sizeTree);

            CalculatingTheCode (*FullTree, AlphabetCodes, ElementNow);

            unsigned char code = 0;
            char j = 0;
            for (int i = 0; i < lenText; ++i) {
                char* strCodeNow = AlphabetCodes[(int)str[i]].Code;
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

            FreeTree (FullTree);
        }

        free (AlphabetCodes);
    } else {
        int lenText = 0;
        int sizeTreeNow = 0;
        int codeSize = 0;
        unsigned char sizeTree = 0;
        unsigned char code = 0;

        if (fscanf (stdin, "%c", &sizeTree) != 1) {
            return 0;
        }

        if (fscanf (stdin, "%c", &code) != 1) {
            return 0;
        }

        while (lenText < sizeTree) {
            char j = 0;
            unsigned char symbol = 0;

            while ((code & 1 << (7 - codeSize)) < 1) {
                str[sizeTreeNow] = '0';
                ++sizeTreeNow;
                code = CheckSizeCodeAndPlusOne (&codeSize, code);
            }

            str[sizeTreeNow] = '1';
            ++sizeTreeNow;
            code = CheckSizeCodeAndPlusOne (&codeSize, code);

            while (j < 8) {
                if (code & 1 << (7 - codeSize)) {
                    symbol |= 1 << (7 - j);
                }

                ++j;
                code = CheckSizeCodeAndPlusOne (&codeSize, code);
            }

            str[sizeTreeNow] = symbol;
            ++sizeTreeNow;
            ++lenText;
        }

        if (str[0] == '1') {
            unsigned char countPrint = 0;
            if (fscanf (stdin, "%c", &countPrint) != 1) {
                return 0;
            }

            for (int i = 0; i < countPrint; ++i) {
                printf ("%c", str[1]);
            }
        } else {
            TTree* FullTree;

            FullTree = BuildingTree (str);

            lenText = 0;

            while (fscanf (stdin, "%c", &str[lenText]) == 1) {
                ++lenText;
            }

            if (lenText == 0) {
                return 0;
            }

            str[lenText] = '\0';

            FindAndPrintCodeSymbols (FullTree, str, lenText);

            FreeTree (FullTree);
        }
    }

    return 0;
}
