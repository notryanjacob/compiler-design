#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK 100
#define MAX_INPUT 100

// Simple grammar:
// S -> aB
// B -> b | ε

// Dummy parsing table (2D matrix)
char parsingTable[2][3][3] = {
    {"aB", "", ""}, // S
    {"", "b", "e"}  // B
};

// Function to get the index for terminals
int getTerminalIndex(char c)
{
    switch (c)
    {
    case 'a':
        return 0;
    case 'b':
        return 1;
    case '$':
        return 2;
    default:
        return -1;
    }
}

// Function to get the index for non-terminals
int getNonTerminalIndex(char c)
{
    switch (c)
    {
    case 'S':
        return 0;
    case 'B':
        return 1;
    default:
        return -1;
    }
}

// Function to parse the input string
bool parse(char *input)
{
    char stack[MAX_STACK];
    int top = 0;
    int inputIndex = 0;

    // Initialize stack with start symbol and end marker
    stack[top++] = '$';
    stack[top++] = 'S';

    printf("Parsing Steps:\n");
    printf("Stack\tInput\tAction\n");

    while (top > 0)
    {
        // Print current state
        printf("%s\t%s\t", stack, input + inputIndex);

        char stackTop = stack[--top];
        char currentInput = input[inputIndex];

        if (stackTop == '$' && currentInput == '$')
        {
            printf("Accept\n");
            return true;
        }

        if (stackTop == currentInput)
        {
            printf("Match %c\n", currentInput);
            inputIndex++;
        }
        else if (stackTop >= 'A' && stackTop <= 'Z')
        {
            int row = getNonTerminalIndex(stackTop);
            int col = getTerminalIndex(currentInput);

            if (row == -1 || col == -1)
            {
                printf("Error\n");
                return false;
            }

            char *production = parsingTable[row][col];
            if (strlen(production) == 0)
            {
                printf("Error\n");
                return false;
            }

            printf("Apply %c -> %s\n", stackTop, production);

            // Push production onto stack in reverse order
            for (int i = strlen(production) - 1; i >= 0; i--)
            {
                if (production[i] != 'e')
                {
                    stack[top++] = production[i];
                }
            }
        }
        else
        {
            printf("Error\n");
            return false;
        }
    }

    return false;
}

int main()
{
    char input[MAX_INPUT];
    printf("Enter input string (end with $): ");
    scanf("%s", input);

    if (parse(input))
    {
        printf("Input accepted\n");
    }
    else
    {
        printf("Input rejected\n");
    }

    return 0;
}