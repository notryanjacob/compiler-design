#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define EPSILON 'e'

// E=TA
// A=+TA
// A=e
// T=FB
// B=*FB
// B=e
// F=(E)
// F=i

char productions[MAX][MAX]; // Store production rules

// Function to check if a symbol is a terminal
int isTerminal(char symbol)
{
    return !isupper(symbol);
}

// Function to find FIRST set of a symbol
void findFirst(char result[], char symbol, int numProductions)
{
    // Check if it's a terminal
    if (isTerminal(symbol))
    {
        strncat(result, &symbol, 1);
        return;
    }

    // Search through the productions
    for (int i = 0; i < numProductions; i++)
    {
        if (productions[i][0] == symbol)
        { // Match non-terminal
            if (isTerminal(productions[i][2]) || productions[i][2] == EPSILON)
            {
                // If terminal or epsilon, add to result
                strncat(result, &productions[i][2], 1);
            }
            else
            {
                // Recursively find FIRST of the next symbol
                char temp[MAX] = "";
                findFirst(temp, productions[i][2], numProductions);
                strcat(result, temp);
            }
        }
    }
}

// Function to find FOLLOW set of a symbol
void findFollow(char result[], char symbol, int numProductions)
{
    // Add '$' to follow of the start symbol
    if (symbol == productions[0][0])
    {
        strcat(result, "$");
    }

    // Search for the symbol in the RHS of productions
    for (int i = 0; i < numProductions; i++)
    {
        char *pos = strchr(productions[i], symbol);
        if (pos != NULL && pos != productions[i])
        { // Ensure not on LHS
            // If there's a next symbol, find its FIRST
            if (*(pos + 1) != '\0')
            {
                char temp[MAX] = "";
                findFirst(temp, *(pos + 1), numProductions);
                strcat(result, temp);

                // If epsilon is in FIRST, add FOLLOW of LHS
                if (strchr(temp, EPSILON))
                {
                    char followTemp[MAX] = "";
                    findFollow(followTemp, productions[i][0], numProductions);
                    strcat(result, followTemp);
                }
            }
            else
            {
                // If at the end, add FOLLOW of the LHS
                if (productions[i][0] != symbol)
                {
                    char followTemp[MAX] = "";
                    findFollow(followTemp, productions[i][0], numProductions);
                    strcat(result, followTemp);
                }
            }
        }
    }
}

int main()
{
    int numProductions;

    printf("Enter number of productions: ");
    scanf("%d", &numProductions);

    printf("Enter the productions in the format (e.g., E=TE'):\n");
    for (int i = 0; i < numProductions; i++)
    {
        scanf("%s", productions[i]);
    }

    // Compute FIRST sets
    for (int i = 0; i < numProductions; i++)
    {
        printf("FIRST(%c): ", productions[i][0]);
        char result[MAX] = "";
        findFirst(result, productions[i][0], numProductions);
        printf("{ %s }\n", result);
    }

    // Compute FOLLOW sets
    for (int i = 0; i < numProductions; i++)
    {
        printf("FOLLOW(%c): ", productions[i][0]);
        char result[MAX] = "";
        findFollow(result, productions[i][0], numProductions);
        printf("{ %s }\n", result);
    }

    return 0;
}
