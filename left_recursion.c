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
// Function to find the longest common prefix between two strings
void longestCommonPrefix(char *result, const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
    {
        result[i] = str1[i];
        i++;
    }
    result[i] = '\0'; // Null terminate the string
}
// Left factoring function
void leftFactoring(const char *nonTerminal, const char production[][100], int numProductions)
{
    char commonPrefix[100] = "";
    int i, j;

    // Finding the common prefix across all productions
    strcpy(commonPrefix, production[0]);
    for (i = 1; i < numProductions; i++)
    {
        char tempPrefix[100];
        longestCommonPrefix(tempPrefix, commonPrefix, production[i]);
        strcpy(commonPrefix, tempPrefix);
        if (strlen(commonPrefix) == 0)
            break; // No common prefix, exit loop
    }

    if (strlen(commonPrefix) == 0)
    {
        printf("No common prefix found, no need for left factoring\n");
        return;
    }

    // New non-terminal for factored productions
    char newNonTerminal[10];
    sprintf(newNonTerminal, "%s\'", nonTerminal);

    printf("After left factoring:\n");
    printf("%s -> %s%s\n", nonTerminal, commonPrefix, newNonTerminal);

    // Now print the rest of the productions
    for (i = 0; i < numProductions; i++)
    {
        if (strncmp(production[i], commonPrefix, strlen(commonPrefix)) == 0)
        {
            // Print the remaining part of the production after the common prefix
            if (strlen(production[i]) == strlen(commonPrefix))
                printf("%s -> e\n", newNonTerminal); // epsilon production
            else
                printf("%s -> %s\n", newNonTerminal, production[i] + strlen(commonPrefix));
        }
        else
        {
            printf("%s -> %s\n", nonTerminal, production[i]);
        }
    }
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
    leftFactoring(nonTerminal, productions, numProductions);
    eliminateLeftRecursion(nonTerminal, productions, numProductions);
}