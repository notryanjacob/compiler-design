#include <stdio.h>
#include <string.h>

struct Symbol
{
    char name[50];
    char type[50];
    int location;
};

struct Symbol symbolTable[100];
int symbolCount = 0;

void addSymbol(const char *name, const char *type, int location)
{
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolTable[symbolCount].location = location;
    symbolCount++;
}

void printTable()
{
    printf("Symbol Table : \n");
    printf("Name\tType\tLocation\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%s\t%s\t%d\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].location);
    }
}

int main()
{
    addSymbol("x", "int", 1000);
    addSymbol("y", "int", 1001);
    addSymbol("address", "string", 1002);
    addSymbol("sum", "float", 1003);
    printTable();
    return 0;
}