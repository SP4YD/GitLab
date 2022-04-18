#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Tree Tree;

struct Tree {
	int Number;
	unsigned char Height;
	Tree* Right;
	Tree* Left;
};

int Max(int a, int b) {
	return a > b ? a : b;
}

int Exi(Tree* Leaf) { //Existence == Exi
	if (Leaf != NULL) {
		return Leaf->Height;
	}
	return 0;
}

int EchoNum = 0;
Tree* EchoMemory;

Tree* EchoLocator() {
	return &EchoMemory[EchoNum++];
}

Tree* SmallLeftRotation(Tree* Knot) {
	Tree* NeedTree = Knot->Right;
	Knot->Right = NeedTree->Left;
	NeedTree->Left = Knot;
	NeedTree->Left->Height = Max(Exi(NeedTree->Left->Left), Exi(NeedTree->Left->Right)) + 1;
	NeedTree->Height = Max(Exi(NeedTree->Left), Exi(NeedTree->Right)) + 1;

	return NeedTree;
}

Tree* SmallRightRotation(Tree* Knot) {
	Tree* NeedTree = Knot->Left;
	Knot->Left = NeedTree->Right;
	NeedTree->Right = Knot;
	NeedTree->Right->Height = Max(Exi(NeedTree->Right->Left), Exi(NeedTree->Right->Right)) + 1;
	NeedTree->Height = Max(Exi(NeedTree->Left), Exi(NeedTree->Right)) + 1;

	return NeedTree;
}

Tree* LargeLeftRotation(Tree* Knot) {
	Knot->Right = SmallRightRotation(Knot->Right);
	return SmallLeftRotation(Knot);
}

Tree* LargeRightRotation(Tree* Knot) {
	Knot->Left = SmallLeftRotation(Knot->Left);
	return SmallRightRotation(Knot);
}

Tree* CreateNewLeaf(Tree* Temp, int num) {
	char null = 0;
	
	if (Temp == NULL) {
		Temp = EchoLocator();
		Temp->Height = 1;
		Temp->Left = Temp->Right = NULL;
		Temp->Number = num;
		return Temp;
	}

	if (num > Temp->Number) {
		if (Temp->Right != NULL) {
			Temp->Right = CreateNewLeaf(Temp->Right, num);
		}
		else {
			Temp->Right = EchoLocator();
			Temp->Right->Height = 1;
			Temp->Right->Left = Temp->Right->Right = NULL;
			Temp->Right->Number = num;
			null = 1;
		}
	}
	else {
		if (Temp->Left != NULL) {
			Temp->Left = CreateNewLeaf(Temp->Left, num);
		}
		else {
			Temp->Left = EchoLocator();
			Temp->Left->Height = 1;
			Temp->Left->Left = Temp->Left->Right = NULL;
			Temp->Left->Number = num;
			null = 1;
		}
	}

	if (!null && abs(Exi(Temp->Left) - Exi(Temp->Right)) > 1) {
		if (Exi(Temp->Right) > Exi(Temp->Left)) {
			if (Exi(Temp->Right->Left) <= Exi(Temp->Right->Right)) {
				Temp = SmallLeftRotation(Temp);
			}
			else {
				Temp = LargeLeftRotation(Temp);
			}
		}
		else {
			if (Exi(Temp->Left->Right) <= Exi(Temp->Left->Left)) {
				Temp = SmallRightRotation(Temp);
			}
			else {
				Temp = LargeRightRotation(Temp);
			}
		}
	}

	Temp->Height = Max(Exi(Temp->Left), Exi(Temp->Right)) + 1;

	return Temp;
}

int main() {
	Tree* HeadTree = NULL;
	int counElements;
	if (scanf("%d", &counElements) != 1) return 1;

	EchoMemory = malloc(sizeof(Tree) * counElements);

	if (!counElements) {
		printf("0");
		return 0;
	}
	
	for (int i = 0; i < counElements; ++i) {
		int temp;
		if (scanf("%d", &temp) != 1) return 1;
		HeadTree = CreateNewLeaf(HeadTree, temp);
	}

	printf("%d", HeadTree->Height);

	return 0;
}
