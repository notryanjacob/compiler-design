#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK 100
#define MAX_SYMBOLS 10

// Grammar:
// E -> E + T | T
// T -> T * F | F
// F -> ( E ) | id

// LR(0) Parsing Table
int action[12][6] = {
    {5, 0, 4, 0, 1, 2},    // State 0
    {0, 6, 0, 0, 0, 0},    // State 1
    {0, -2, 0, -2, 0, -2}, // State 2
    {0, -4, 0, -4, 0, -4}, // State 3
    {5, 0, 4, 0, 8, 2},    // State 4
    {0, -6, 0, -6, 0, -6}, // State 5
    {5, 0, 4, 0, 0, 9},    // State 6
    {5, 0, 4, 0, 0, 10},   // State 7
    {0, 6, 0, 11, 0, 0},   // State 8
    {0, -1, 0, -1, 0, -1}, // State 9
    {0, -3, 0, -3, 0, -3}, // State 10
    {0, -5, 0, -5, 0, -5}  // State 11
};

int goTo[12][3] = {
    {1, 2, 3},  // State 0
    {0, 0, 0},  // State 1
    {0, 0, 0},  // State 2
    {0, 0, 0},  // State 3
    {8, 2, 3},  // State 4
    {0, 0, 0},  // State 5
    {0, 9, 3},  // State 6
    {0, 0, 10}, // State 7
    {0, 0, 0},  // State 8
    {0, 0, 0},  // State 9
    {0, 0, 0},  // State 10
    {0, 0, 0}   // State 11
};

int getSymbolIndex(char symbol)
{
    switch (symbol)
    {
    case 'id':
        return 0;
    case '+':
        return 1;
    case '(':
        return 2;
    case ')':
        return 3;
    case '$':
        return 4;
    case '*':
        return 5;
    case 'E':
        return 0;
    case 'T':
        return 1;
    case 'F':
        return 2;
    default:
        return -1;
    }
}

void shift(int *stack, int *top, int state)
{
    stack[++(*top)] = state;
}

void reduce(int *stack, int *top, int rule)
{
    switch (rule)
    {
    case 1: // E -> E + T
    case 2: // E -> T
        *top -= (rule == 1) ? 6 : 2;
        stack[++(*top)] = goTo[stack[*top - 1]][0];
        break;
    case 3: // T -> T * F
    case 4: // T -> F
        *top -= (rule == 3) ? 6 : 2;
        stack[++(*top)] = goTo[stack[*top - 1]][1];
        break;
    case 5: // F -> ( E )
    case 6: // F -> id
        *top -= (rule == 5) ? 6 : 2;
        stack[++(*top)] = goTo[stack[*top - 1]][2];
        break;
    }
}

bool parse(char *input)
{
    int stack[MAX_STACK], top = -1;
    int index = 0;
    shift(stack, &top, 0);

    while (true)
    {
        int state = stack[top];
        char symbol = input[index];
        int action_val = action[state][getSymbolIndex(symbol)];

        if (action_val > 0)
        {
            shift(stack, &top, action_val);
            index++;
        }
        else if (action_val < 0)
        {
            reduce(stack, &top, -action_val);
        }
        else if (state == 1 && symbol == '$')
        {
            printf("Input accepted\n");
            return true;
        }
        else
        {
            printf("Error at position %d\n", index);
            return false;
        }
    }
}

int main()
{
    char input[MAX_SYMBOLS];
    printf("Enter input string (use 'id' for identifier, end with $): ");
    scanf("%s", input);
    parse(input);
    return 0;
}