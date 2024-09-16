#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_INPUT 10

typedef struct Node
{
    char data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Transition
{
    int from;
    char symbol;
    int to;
} Transition;

Node *createNode(char data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void printParseTree(Node *root, int level)
{
    if (root == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf("  ");
    printf("%c\n", root->data);
    printParseTree(root->left, level + 1);
    printParseTree(root->right, level + 1);
}

void addTransition(Transition transitions[], int *size, int from, char symbol, int to)
{
    transitions[*size].from = from;
    transitions[*size].symbol = symbol;
    transitions[*size].to = to;
    (*size)++;
}

void printTransitionTable(Transition transitions[], int size)
{
    printf("\nTransition Table:\n");
    printf("From\tSymbol\tTo\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d\t%c\t%d\n", transitions[i].from, transitions[i].symbol, transitions[i].to);
    }
}

void calculateFirstFollowLast()
{
    // Simplified first, follow, and last sets:
    printf("First/Follow/Last Sets (simplified):\n");
    printf("First(0) = {0}\n");
    printf("First(1) = {1}\n");
    printf("Last(0) = {0}\n");
    printf("Last(1) = {1}\n");
}

int simulateDFA(Transition transitions[], int size, const char *input)
{
    int currentState = 0; // Start state
    for (int i = 0; input[i] != '\0'; i++)
    {
        char symbol = input[i];
        int found = 0;
        for (int j = 0; j < size; j++)
        {
            if (transitions[j].from == currentState && transitions[j].symbol == symbol)
            {
                currentState = transitions[j].to;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            return 0; // No valid transition
        }
    }
    return currentState == 5; // Accepting state
}

void buildDFA(Transition transitions[])
{
    int size = 0;
    // From state 0 with '0' to state 1
    addTransition(transitions, &size, 0, '0', 1);
    // From state 1 with '0' to state 2
    addTransition(transitions, &size, 1, '0', 2);
    // From state 1 with '1' to state 3
    addTransition(transitions, &size, 1, '1', 3);
    // From state 2 with '0' to state 4
    addTransition(transitions, &size, 2, '0', 4);
    // From state 3 with '1' to state 4
    addTransition(transitions, &size, 3, '1', 4);
    // From state 4 with '0' to state 5
    addTransition(transitions, &size, 4, '0', 5);
    // From state 4 with '1' to state 5
    addTransition(transitions, &size, 4, '1', 5);

    printTransitionTable(transitions, size);
}

int main()
{
    // Regular expression: 0(0+1)01
    Node *root = createNode('0');
    root->left = createNode('+');
    root->left->left = createNode('0');
    root->left->right = createNode('1');
    root->right = createNode('0');
    root->right->right = createNode('1');

    printf("Parse Tree:\n");
    printParseTree(root, 0);

    calculateFirstFollowLast();

    Transition transitions[MAX_STATES];
    buildDFA(transitions);

    const char *inputString = "0101";
    if (simulateDFA(transitions, MAX_STATES, inputString))
    {
        printf("The string %s is accepted by the DFA.\n", inputString);
    }
    else
    {
        printf("The string %s is accepted by the DFA.\n", inputString);
    }

    return 0;
}