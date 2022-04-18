#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct TTree TTree;
    
struct TTree {
    int Number;
    unsigned char Height;
    TTree* Right;
    TTree* Left;
};

int Max(int a, int b) {
    return a > b ? a : b;
}

int GetHeight(TTree* Leaf) { 
    if (Leaf != NULL) {
        return Leaf->Height;
    }
    return 0;
}

int EchoNum = 0;
TTree* EchoMemory;

TTree* EchoLocator(int num) {
    EchoMemory[EchoNum].Height = 1;
    EchoMemory[EchoNum].Left = EchoMemory[EchoNum].Right = NULL;
    EchoMemory[EchoNum].Number = num;

    return &EchoMemory[EchoNum++];
}

TTree* SmallLeftRotation(TTree* Knot) {
    TTree* NeedTree = Knot->Right;
    Knot->Right = NeedTree->Left;
    NeedTree->Left = Knot;
    NeedTree->Left->Height = Max(GetHeight(NeedTree->Left->Left), GetHeight(NeedTree->Left->Right)) + 1;
    NeedTree->Height = Max(GetHeight(NeedTree->Left), GetHeight(NeedTree->Right)) + 1;

    return NeedTree;
}

TTree* SmallRightRotation(TTree* Knot) {
    TTree* NeedTree = Knot->Left;
    Knot->Left = NeedTree->Right;
    NeedTree->Right = Knot;
    NeedTree->Right->Height = Max(GetHeight(NeedTree->Right->Left), GetHeight(NeedTree->Right->Right)) + 1;
    NeedTree->Height = Max(GetHeight(NeedTree->Left), GetHeight(NeedTree->Right)) + 1;

    return NeedTree;
}

TTree* LargeLeftRotation(TTree* Knot) {
    Knot->Right = SmallRightRotation(Knot->Right);

    return SmallLeftRotation(Knot);
}

TTree* LargeRightRotation(TTree* Knot) {
    Knot->Left = SmallLeftRotation(Knot->Left);

    return SmallRightRotation(Knot);
}

TTree* CreateNewLeaf(TTree* Temp, int num) {
    char isNull = 0;

    if (Temp == NULL) {
        Temp = EchoLocator(num);
        return Temp;
    }

    if (num > Temp->Number) {
        if (Temp->Right != NULL) {
            Temp->Right = CreateNewLeaf(Temp->Right, num);
        }
        else {
            Temp->Right = EchoLocator(num);
            isNull = 1;
        }
    }
    else {
        if (Temp->Left != NULL) {
            Temp->Left = CreateNewLeaf(Temp->Left, num);
        }
        else {
            TTree* Next = Temp->Left;
            Temp->Left = EchoLocator(num);
            isNull = 1;
        }
    }

    if (!isNull && abs(GetHeight(Temp->Left) - GetHeight(Temp->Right)) > 1) {
        if (GetHeight(Temp->Right) > GetHeight(Temp->Left)) {
            if (GetHeight(Temp->Right->Left) <= GetHeight(Temp->Right->Right)) {
                Temp = SmallLeftRotation(Temp);
            }
            else {
                Temp = LargeLeftRotation(Temp);
            }
        }
        else {
            if (GetHeight(Temp->Left->Right) <= GetHeight(Temp->Left->Left)) {
                Temp = SmallRightRotation(Temp);
            }
            else {
                Temp = LargeRightRotation(Temp);
            }
        }
    }

    Temp->Height = Max(GetHeight(Temp->Left), GetHeight(Temp->Right)) + 1;

    return Temp;
}

int main() {
    TTree* HeadTree = NULL;
    int counElements;
    if (scanf ("%d", &counElements) != 1) {
        return 1;
    }

    EchoMemory = malloc(sizeof(TTree) * counElements);

    if (!counElements) {
        printf("0");
        return 0;
    }

    for (int i = 0; i < counElements; ++i) {
        int temp;
        if (scanf ("%d", &temp) != 1) {
            return 1;
        }
        HeadTree = CreateNewLeaf(HeadTree, temp);
    }

    printf("%d", HeadTree->Height);

    return 0;
}
