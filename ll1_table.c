#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define EPSILON 'e'

char nonTerminals[MAX];
char terminals[MAX];
char productions[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
char table[MAX][MAX][MAX];

int n, t; // Number of non-terminals and terminals

int isTerminal(char symbol)
{
    return !isupper(symbol);
}

void findFirst(char result[], char symbol)
{
    for (int i = 0; i < n; i++)
    {
        if (productions[i][0] == symbol)
        {
            if (isTerminal(productions[i][2]) || productions[i][2] == EPSILON)
            {
                strncat(result, &productions[i][2], 1);
            }
            else
            {
                char temp[MAX] = "";
                findFirst(temp, productions[i][2]);
                strcat(result, temp);
            }
        }
    }
}

void findFollow(char result[], char symbol)
{
    if (symbol == productions[0][0])
    {
        strcat(result, "$");
    }

    for (int i = 0; i < n; i++)
    {
        char *pos = strchr(productions[i] + 2, symbol);
        if (pos != NULL)
        {
            if (*(pos + 1) != '\0')
            {
                char temp[MAX] = "";
                findFirst(temp, *(pos + 1));
                strcat(result, temp);
                if (strchr(temp, EPSILON) != NULL)
                {
                    char followTemp[MAX] = "";
                    findFollow(followTemp, productions[i][0]);
                    strcat(result, followTemp);
                }
            }
            else if (productions[i][0] != symbol)
            {
                char followTemp[MAX] = "";
                findFollow(followTemp, productions[i][0]);
                strcat(result, followTemp);
            }
        }
    }
}

void constructParsingTable()
{
    for (int i = 0; i < n; i++)
    {
        char firstSet[MAX] = "";
        findFirst(firstSet, productions[i][0]);

        for (int j = 0; j < strlen(firstSet); j++)
        {
            if (firstSet[j] != EPSILON)
            {
                int col = strchr(terminals, firstSet[j]) - terminals;
                strcpy(table[i][col], productions[i]);
            }
            else
            {
                char followSet[MAX] = "";
                findFollow(followSet, productions[i][0]);
                for (int k = 0; k < strlen(followSet); k++)
                {
                    int col = strchr(terminals, followSet[k]) - terminals;
                    strcpy(table[i][col], productions[i]);
                }
            }
        }
    }
}

void displayParsingTable()
{
    printf("\nParsing Table:\n");
    printf("   ");
    for (int i = 0; i < t; i++)
    {
        printf("%c  ", terminals[i]);
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%c  ", nonTerminals[i]);
        for (int j = 0; j < t; j++)
        {
            if (strlen(table[i][j]) > 0)
            {
                printf("%s ", table[i][j]);
            }
            else
            {
                printf("-  ");
            }
        }
        printf("\n");
    }
}

int main()
{
    strcpy(nonTerminals, "EATF");
    strcpy(terminals, "+*()i$");
    strcpy(productions[0], "E=TA");
    strcpy(productions[1], "A=+TA");
    strcpy(productions[2], "A=e");
    strcpy(productions[3], "T=FB");
    strcpy(productions[4], "B=*FB");
    strcpy(productions[5], "B=e");
    strcpy(productions[6], "F=(E)");
    strcpy(productions[7], "F=i");

    n = strlen(nonTerminals);
    t = strlen(terminals);

    constructParsingTable();
    displayParsingTable();

    return 0;
}