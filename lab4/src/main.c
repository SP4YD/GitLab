#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct TStack TStack;
typedef struct StackOper StackOper;
typedef struct StackNum StackNum;

struct TStack {
    struct StackOper {
        char Operation;
        int Priority;

        StackOper* Next;
    }* HeadOper;
    struct StackNum {
        int Number;

        StackNum* Next;
    }* HeadNum;
};

int ConvertToNum(char symbol) {
    return symbol - '0';
}

int CalcPriority(char symbol) {
    if (symbol == '/' || symbol == '*') {
        return 3;
    }
    if (symbol == '+' || symbol == '-') {
        return 2;
    }
    if (symbol == ')') {
        return 1;
    }
    if (symbol == '(') {
        return 0;
    }
    return 10;
}

int PushStackOper(TStack* stack, char operation) {
    if (!stack) {
        return 1;
    }
    StackOper* newElement = malloc(sizeof(StackOper));
    if (!newElement) {
        return 1;
    }

    newElement->Next = stack->HeadOper;
    stack->HeadOper = newElement;
    stack->HeadOper->Operation = operation;
    stack->HeadOper->Priority = CalcPriority(operation);

    return 0;
}

int PushStackNum(TStack* stack, int value) {
    if (!stack) {
        return 1;
    }
    StackNum* newElement = malloc(sizeof(StackNum));
    if (!newElement) {
        return 1;
    }

    newElement->Next = stack->HeadNum;
    stack->HeadNum = newElement;
    stack->HeadNum->Number = value;

    return 0;
}

int PopStackOper(TStack* stack, int* priority, char* operation) {
    if (!stack || !stack->HeadOper) {
        return 1;
    }

    *operation = stack->HeadOper->Operation;
    *priority = stack->HeadOper->Priority;
    StackOper* temp = stack->HeadOper;
    stack->HeadOper = stack->HeadOper->Next;
    free(temp);

    return 0;
}

int PopStackNum(TStack* stack, int* value) {
    if (!stack || !stack->HeadNum) {
        return 1;
    }

    *value = stack->HeadNum->Number;
    StackNum* temp = stack->HeadNum;
    stack->HeadNum = stack->HeadNum->Next;
    free(temp);

    return 0;
}

int CheckSymbol(char symbol) {
    char pattern[17] = "0123456789+-/*()";
    for (int i = 0; i < 17; ++i) {
        if (symbol == pattern[i]) {
            return 0;
        }
    }
    return 1;
}

int PerformingOperation(TStack* stack, char operation, int* value) {
    int num1, num2;
    if (PopStackNum(stack, &num1)) { return 1; }
    if (PopStackNum(stack, &num2)) { return 1; }

    if (operation == '/') {
        if (num1 == 0) {
            return 2;
        }
        *value = num2 / num1;
    }
    else if (operation == '*') {
        *value = num2 * num1;
    }
    else if (operation == '+') {
        *value = num2 + num1;
    }
    else if (operation == '-') {
        *value = num2 - num1;
    }
    else if (operation == '*') {
        *value = num2 * num1;
    }
    else {
        return 1;
    }

    return 0;
}

int CheckError(char* expression, int len) {
    int leftBracket = 1, rightBracket = 0;
    for (int i = 0; i < len; ++i) {
        if (CheckSymbol(expression[i])) {
            return 1;
        }
        if (expression[i] == '(') {
            ++leftBracket;
        }
        else if (expression[i] == ')') {
            ++rightBracket;
        }
    }

    if (leftBracket != rightBracket) {
        return 1;
    }

    return 0;
}

int MathematicalCalculations(char* expression, int len, int* value) {
    TStack stack;
    stack.HeadNum = NULL;
    stack.HeadOper = NULL;
    if (PushStackOper(&stack, '(')) { return 1; }
    int priorityLast, priorityCurrent, tempValue, code, number = 0;
    for (int i = 0; i < len; ++i) {
        int success = 0, numberOfOperations = 0;
        char lastOperation;
        if (expression[i] >= '0' && expression[i] <= '9') {
            number += ConvertToNum(expression[i]);
            if (i < len - 1 && expression[i + 1] >= '0' && expression[i + 1] <= '9') {
                number *= 10;
            }
            else {
                if (PushStackNum(&stack, number)) { return 1; }
                number = 0;
            }
        }
        else {
            priorityCurrent = CalcPriority(expression[i]);
            while (!success) {
                if (PopStackOper(&stack, &priorityLast, &lastOperation)) { return 1; }
                if (priorityLast >= priorityCurrent && priorityCurrent != 0) {
                    code = PerformingOperation(&stack, lastOperation, &tempValue);
                    if (code == 1) { return 1; }
                    if (code == 2) { return 2; }
                    if (PushStackNum(&stack, tempValue)) { return 1; }
                }
                else {
                    if (priorityCurrent != 1 || priorityLast != 0) {
                        if (PushStackOper(&stack, lastOperation)) { return 1; }
                        if (PushStackOper(&stack, expression[i])) { return 1; }
                    }
                    else if (!numberOfOperations) {
                        return 1;
                    }
                    success = 1;
                }
                ++numberOfOperations;
            }
        }
    }
    if (PopStackNum(&stack, value)) { return 1; }
    if (stack.HeadNum != NULL) {
        return 1;
    }
    return 0;
}

int main() {
    char expression [1000];

    if (scanf("%s", expression) != 1) { return 0; }

    int len = strlen(expression) + 1, answer = 0;
    expression[len - 1] = ')'; expression[len] = '\0';

    if (CheckError(expression, len)) {
        printf("syntax error");
        return 0;
    }

    int code = MathematicalCalculations(expression, len, &answer);

    if (code == 0) {
        printf("%d\n", answer);
    }
    else if (code == 1) {
        printf("syntax error");
    }
    else if (code == 2) {
        printf("division by zero");
    }

    return 0;
}
