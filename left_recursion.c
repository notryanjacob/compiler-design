// Original Production: A → βα | Aα
// After Eliminating Left Recursion:
// A  → βA'
// A' → αA' | ε

// Original Production: B → αβ | αγ
// After Left Factoring:
// B  → αB'
// B' → β | γ
#include <stdio.h>
#include <string.h>

int isLeftRecursive(const char *nonTerminal, const char *production)
{
    return production[0] == nonTerminal[0];
}

void eliminateLeftRecursion(const char *nonTerminal, const char production[][100], int numProductions)
{
    char alphas[100][100];
    char betas[100][100];
    int alphaCount = 0, betaCount = 0;

    for (int i = 0; i < numProductions; i++)
    {
        if (isLeftRecursive(nonTerminal, production[i]))
        {
            strcpy(alphas[alphaCount++], production[i] + 1); // skip the first char (left recursive part)
        }
        else
        {
            strcpy(betas[betaCount++], production[i]);
        }
    }

    // if there are no left recursions, there's nothing to eliminate
    if (alphaCount == 0)
    {
        printf("No left recursions to eliminate\n");
        return;
    }

    // Create new non terminal (A')
    char newNonTerminal[10];
    sprintf(newNonTerminal, "%s\'", nonTerminal);

    printf("After eliminating left recursion \n");

    // Print the prodns of A -> bA'
    for (int i = 0; i < betaCount; i++)
    {
        printf("%s -> %s%s\n", nonTerminal, betas[i], newNonTerminal);
    }
    // print the prodns of A' -> aA' | e
    for (int i = 0; i < alphaCount; i++)
    {
        printf("%s -> %s%s\n", newNonTerminal, alphas[i], newNonTerminal);
    }
    printf("%s -> e", newNonTerminal);
}

int main()
{
    const char nonTerminal[] = "A";
    const char productions[5][100] = {"Aa", "Ab", "c", "d"};
    int numProductions = 4;

    printf("Original Grammar\n");
    for (int i = 0; i < numProductions; i++)
    {
        printf("%s -> %s\n", nonTerminal, productions[i]);
    }
    eliminateLeftRecursion(nonTerminal, productions, numProductions);
}