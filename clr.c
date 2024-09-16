#include <stdio.h>
#include <stdbool.h>

#define MAX_STACK 100
#define MAX_INPUT 100

// Grammar: S -> S + S | (S) | a

// LALR Parsing Table
int action[7][5] = {
    {3, 0, 2, 0, 0},      // State 0
    {0, 4, 0, 0, -1},     // State 1
    {3, 0, 2, 0, 0},      // State 2
    {-3, -3, -3, -3, -3}, // State 3
    {3, 0, 2, 0, 0},      // State 4
    {0, 4, 0, -2, -2},    // State 5
    {-1, -1, -1, -1, -1}  // State 6
};

int goTo[7][1] = {
    {1}, // State 0
    {0}, // State 1
    {5}, // State 2
    {0}, // State 3
    {6}, // State 4
    {0}, // State 5
    {0}  // State 6
};

int getSymbolIndex(char symbol)
{
    switch (symbol)
    {
    case 'a':
        return 0;
    case '+':
        return 1;
    case '(':
        return 2;
    case ')':
        return 3;
    case '$':
        return 4;
    default:
        return -1;
    }
}

bool parse(char *input)
{
    int stack[MAX_STACK], top = 0;
    stack[top] = 0;
    int index = 0;

    while (true)
    {
        int state = stack[top];
        char symbol = input[index];
        int act = action[state][getSymbolIndex(symbol)];

        if (act > 0)
        { // Shift
            stack[++top] = act;
            index++;
        }
        else if (act < 0)
        { // Reduce
            int rule = -act;
            if (rule == 1)
                top -= 3; // S -> S + S
            else if (rule == 2)
                top -= 3; // S -> (S)
            else
                top -= 1; // S -> a
            stack[++top] = goTo[stack[top - 1]][0];
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
    char input[MAX_INPUT];
    printf("Enter input string (end with $): ");
    scanf("%s", input);
    parse(input);
    return 0;
}