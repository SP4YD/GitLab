#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define IsNotSymbol 256

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

int LetterCode (int symbol) {
    if (symbol < 0) {
        symbol += 256;
    }

    return symbol;
}

void PushQueue (TMainQueue* MainQueue, TTree* root) {
    TQueueElement* newElement = malloc (sizeof (TQueueElement));

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
    } 
    else {
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

void PrintCodeTree (TTree FullTree, int sizeTree) { //BFS
    TMainQueue MainQueue;
    int lenOutput = 0;
    char output[100000] = {'\0'};

    MainQueue.Head = MainQueue.Tail = NULL;
    PushQueue (&MainQueue, &FullTree);

    if (FullTree.Symbol == IsNotSymbol) {
        int countPrint = 0;
        output[lenOutput] = '0';
        ++lenOutput;

        while (MainQueue.Head && countPrint < sizeTree) {
            TTree* RootNow;
            PopQueue (&MainQueue, &RootNow);

            if (RootNow->Left != NULL) {
                if (RootNow->Left->Symbol != IsNotSymbol) {
                    output[lenOutput] = '1';
                    output[lenOutput + 1] = RootNow->Left->Symbol;
                    lenOutput += 2;
                    ++countPrint;
                } 
                else {
                    output[lenOutput] = '0';
                    ++lenOutput;
                }
                PushQueue (&MainQueue, RootNow->Left);
            } 
            else {
                output[lenOutput] = '0';
                ++lenOutput;
            }

            if (RootNow->Right != NULL) {
                if (RootNow->Right->Symbol != IsNotSymbol) {
                    output[lenOutput] = '1';
                    output[lenOutput + 1] = RootNow->Right->Symbol;
                    lenOutput += 2;
                    ++countPrint;
                } 
                else {
                    output[lenOutput] = '0';
                    ++lenOutput;
                }
                PushQueue (&MainQueue, RootNow->Right);
            } 
            else {
                output[lenOutput] = '0';
                ++lenOutput;
            }
        }
    } 
    else {
        output[lenOutput] = '1';
        output[lenOutput + 1] = FullTree.Symbol;
        lenOutput += 2;
    }

    printf ("%d %s", lenOutput, output);

    ClearingQueue (MainQueue);
}

void SearchMin (TTree* FullTree, int sizeTree, int* min1I, int* min2I) {
    int min1C = 100000;
    int min2C = 100000;

    for (int i = 0; i < sizeTree; ++i) {
        if (!FullTree[i].Used) {
            if (FullTree[i].Count < min1C) {
                min2C = min1C;
                *min2I = *min1I;
                min1C = FullTree[i].Count;
                *min1I = i;
            } 
            else if (FullTree[i].Count < min2C) {
                min2C = FullTree[i].Count;
                *min2I = i;
            }
        }
    }
}

TTree* CombiningTrees (TTree* First, TTree* Second) {
    TTree* NewTree = calloc (1, sizeof (TTree));

    NewTree->Count = First->Count + Second->Count;
    NewTree->Left = Second;
    NewTree->Right = First;
    NewTree->Used = 0;
    NewTree->Symbol = IsNotSymbol;

    return NewTree;
}

TTree AlgorithmHuffman (TTree* FullTree, int* sizeTree) {
    int min1Index = 0;
    int min2Index = 0;

    for (int i = 0, N = *sizeTree; i < N - 2; ++i) {
        SearchMin (FullTree, *sizeTree, &min1Index, &min2Index);

        FullTree[*sizeTree] = *CombiningTrees (&FullTree[min1Index], &FullTree[min2Index]);
        FullTree[min1Index].Used = FullTree[min2Index].Used = 1;

        *sizeTree += 1;
    }

    FullTree[*sizeTree] = *CombiningTrees (&FullTree[*sizeTree - 1], &FullTree[*sizeTree - 2]);

    return FullTree[*sizeTree];
}

TTree* CreateAllTree (int sizeTree, int* alphabet) {
    TTree* AlphabetTree = malloc ((2 * sizeTree) * sizeof (TTree));

    for (int i = 0, N = 0; i < 256; ++i) {
        if (alphabet[i]) {
            AlphabetTree[N].Left = AlphabetTree[N].Right = NULL;
            AlphabetTree[N].Count = alphabet[i];
            AlphabetTree[N].Used = 0;
            if (i > 127) {
                AlphabetTree[N].Symbol = i - 256;
            } 
            else {
                AlphabetTree[N].Symbol = i;
            }
            ++N;
        }
    }

    return AlphabetTree;
}

TTree* BuildingTree (char* codeTree) {
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

        if (RootNow != NULL) {
            RootNow->Count = RootNow->Used = 0;
            RootNow->Left = RootNow->Right = NULL;
            RootNow->Symbol = IsNotSymbol;

            if (codeTree[i] == '0') {
                RootNow->Left = calloc (1, sizeof (TTree));
                RootNow->Right = calloc (1, sizeof (TTree));
            } 
            else {
                ++i;
                RootNow->Symbol = LetterCode (codeTree[i]);
            }

            PushQueue (&MainQueue, RootNow->Left);
            PushQueue (&MainQueue, RootNow->Right);
        }
    }

    ClearingQueue (MainQueue);

    return FullTree;
}

void CalculatingTheCode (TTree TreeNow, TElementAlTree* Alphabet, TElementAlTree ElementNow) {
    if (TreeNow.Symbol != IsNotSymbol) {
        ElementNow.Code[ElementNow.LenCode] = '\0';
        Alphabet[TreeNow.Symbol] = ElementNow;

        return;
    }

    ++ElementNow.LenCode;

    ElementNow.Code[ElementNow.LenCode - 1] = '0';
    CalculatingTheCode (*TreeNow.Left, Alphabet, ElementNow);

    ElementNow.Code[ElementNow.LenCode - 1] = '1';
    CalculatingTheCode (*TreeNow.Right, Alphabet, ElementNow);

}

void FindAndPrintCodeSymbols (TTree* FullTree, char* input) {
    TTree* TreeNow = FullTree;

    for (int i = 0; input[i] != '\0'; ++i) {
        if (TreeNow->Symbol == IsNotSymbol) {
            if (input[i] == '0') {
                TreeNow = TreeNow->Left;
            } 
            else {
                TreeNow = TreeNow->Right;
            }
        } 
        else {
            printf ("%c", TreeNow->Symbol);
            TreeNow = FullTree;
            --i;
        }
    }

    if (TreeNow->Symbol != IsNotSymbol) {
        printf ("%c", TreeNow->Symbol);
    }
}

int main (void) {
    char str[100000] = {'\0'};
    char task = 0;

    if (scanf ("%c", &task) != 1) {
        return 1;
    }

    if (task == 'c') {
        int alphabet[256] = {0};
        int lenText;

        lenText = fread (str, sizeof (char), 100000, stdin);

        if (lenText == 0) {
            return 1;
        }

        for (int i = 0; str[i] != '\0'; ++i) {
            ++alphabet[LetterCode (str[i])];
        }

        int sizeTree = 0;
        TElementAlTree* AlphabetCodes = calloc (256, sizeof (TElementAlTree));
        TElementAlTree ElementNow;
        ElementNow.LenCode = 0;

        for (int i = 0; i < 256; ++i) {
            if (alphabet[i]) {
                ++sizeTree;
            }
        }

        TTree* AlphabetTree = CreateAllTree (sizeTree, alphabet);
        TTree FullTree;

        FullTree = AlgorithmHuffman (AlphabetTree, &sizeTree);

        CalculatingTheCode (FullTree, AlphabetCodes, ElementNow);

        PrintCodeTree (FullTree, sizeTree / 2);

        //printf ("\n\n");

        for (int i = 0; str[i] != '\0'; ++i) {
            printf ("%s", AlphabetCodes[(int)str[i]].Code);
        }

        //for (int i = 0; i < sizeTree; ++i) {
        //    free (FullTree[i]);
        //}
        free (AlphabetCodes);
        free (AlphabetTree);
    } 
    else {
        TTree* FullTree;
        int lenText;
        int lenInput = 0;

        if (scanf ("%d ", &lenInput) != 1) {
            return 1;
        }

        lenText = fread (str, sizeof (char), lenInput, stdin);

        if (lenText != lenInput) {
            return 1;
        }

        FullTree = BuildingTree (str);

        lenText = fread (str, sizeof (char), 100000, stdin);

        if (lenText == 0) {
            return 1;
        }

        FindAndPrintCodeSymbols (FullTree, str);

        free (FullTree);
    }

    return 0;
}
