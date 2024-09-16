#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RULES 10
#define MAX_STATE_ITEMS 100
#define MAX_STATES 100
#define MAX_STACK 1000

// Structure to represent a rule in the grammar
typedef struct
{
    char lhs;
    char rhs[100];
} Rule;

// Structure to represent an LR(1) item
typedef struct
{
    int ruleIndex;
    int dotPosition;
    char lookahead;
} LR1Item;

// Structure to represent a state in the parser
typedef struct
{
    LR1Item items[MAX_STATE_ITEMS];
    int numItems;
} State;

// Global variables
Rule grammar[MAX_RULES];
int numRules = 0;
State states[MAX_STATES];
int numStates = 0;
int actionTable[MAX_STATES][128];
int gotoTable[MAX_STATES][128];

// Function to add a rule to the grammar
void addRule(char lhs, char *rhs)
{
    grammar[numRules].lhs = lhs;
    strcpy(grammar[numRules].rhs, rhs);
    numRules++;
}

// Function to print the grammar
void printGrammar()
{
    printf("RYAN JACOB 22BCT0356\tGrammar:\n");
    for (int i = 0; i < numRules; i++)
    {
        printf("RYAN JACOB 22BCT0356\t%c -> %s\n", grammar[i].lhs, grammar[i].rhs);
    }
}

// Function to check if a state already contains an item
int containsItem(State *state, LR1Item item)
{
    for (int i = 0; i < state->numItems; i++)
    {
        if (state->items[i].ruleIndex == item.ruleIndex &&
            state->items[i].dotPosition == item.dotPosition &&
            state->items[i].lookahead == item.lookahead)
        {
            return 1;
        }
    }
    return 0;
}

// Function to add an item to a state
void addItem(State *state, LR1Item item)
{
    if (!containsItem(state, item))
    {
        state->items[state->numItems++] = item;
    }
}

// Function to get the first set of a symbol
char *first(char symbol)
{
    static char result[10];
    int index = 0;

    if (symbol >= 'a' && symbol <= 'z')
    {
        result[index++] = symbol;
    }
    else
    {
        for (int i = 0; i < numRules; i++)
        {
            if (grammar[i].lhs == symbol)
            {
                if (grammar[i].rhs[0] >= 'a' && grammar[i].rhs[0] <= 'z')
                {
                    result[index++] = grammar[i].rhs[0];
                }
                else
                {
                    char *subFirst = first(grammar[i].rhs[0]);
                    for (int j = 0; subFirst[j] != '\0'; j++)
                    {
                        result[index++] = subFirst[j];
                    }
                }
            }
        }
    }
    result[index] = '\0';
    return result;
}

// Closure function for LR(1) item set construction
void closure(State *state)
{
    int added = 1;
    while (added)
    {
        added = 0;
        for (int i = 0; i < state->numItems; i++)
        {
            LR1Item item = state->items[i];
            if (item.dotPosition < strlen(grammar[item.ruleIndex].rhs))
            {
                char nextSymbol = grammar[item.ruleIndex].rhs[item.dotPosition];
                if (nextSymbol >= 'A' && nextSymbol <= 'Z')
                { // Non-terminal
                    char *firstSet = first(grammar[item.ruleIndex].rhs[item.dotPosition + 1]);
                    for (int j = 0; firstSet[j] != '\0'; j++)
                    {
                        LR1Item newItem = {item.ruleIndex, item.dotPosition + 1, firstSet[j]};
                        if (!containsItem(state, newItem))
                        {
                            addItem(state, newItem);
                            added = 1;
                        }
                    }
                }
            }
        }
    }
}

// Function to calculate the GOTO state for a given symbol
State gotoState(State *state, char symbol)
{
    State newState;
    newState.numItems = 0;

    for (int i = 0; i < state->numItems; i++)
    {
        LR1Item item = state->items[i];
        if (item.dotPosition < strlen(grammar[item.ruleIndex].rhs))
        {
            char nextSymbol = grammar[item.ruleIndex].rhs[item.dotPosition];
            if (nextSymbol == symbol)
            {
                LR1Item newItem = {item.ruleIndex, item.dotPosition + 1, item.lookahead};
                addItem(&newState, newItem);
            }
        }
    }

    closure(&newState);
    return newState;
}

// Function to check if two states are equal
int equalStates(State a, State b)
{
    if (a.numItems != b.numItems)
        return 0;
    for (int i = 0; i < a.numItems; i++)
    {
        if (!containsItem(&b, a.items[i]))
            return 0;
    }
    return 1;
}

// Function to check if a state is already in the list of states
int containsState(State *states, int numStates, State state)
{
    for (int i = 0; i < numStates; i++)
    {
        if (equalStates(states[i], state))
            return i;
    }
    return -1;
}

// Function to print LR(1) items in a state
void printState(State *state)
{
    printf("RYAN JACOB 22BCT0356\t");
    for (int i = 0; i < state->numItems; i++)
    {
        LR1Item item = state->items[i];
        printf("[%c -> ", grammar[item.ruleIndex].lhs);
        for (int j = 0; j < strlen(grammar[item.ruleIndex].rhs); j++)
        {
            if (j == item.dotPosition)
            {
                printf(".");
            }
            printf("%c", grammar[item.ruleIndex].rhs[j]);
        }
        if (item.dotPosition == strlen(grammar[item.ruleIndex].rhs))
        {
            printf(".");
        }
        printf(", %c]\n", item.lookahead);
    }
}

// Function to generate the CLR parsing tables (ACTION and GOTO)
void generateParsingTables()
{
    // Initial state with augmented grammar rule
    State initialState;
    initialState.numItems = 0;
    LR1Item startItem = {0, 0, '$'};
    addItem(&initialState, startItem);
    closure(&initialState);
    states[numStates++] = initialState;

    // Perform item set construction (canonical collection of LR(1) sets)
    for (int i = 0; i < numStates; i++)
    {
        State currentState = states[i];
        printf("\nRYAN JACOB 22BCT0356\tState %d:\n", i);
        printState(&currentState);

        // Perform GOTO transitions for each symbol in the grammar
        for (char symbol = 'A'; symbol <= 'Z'; symbol++)
        {
            State newState = gotoState(&currentState, symbol);
            if (newState.numItems > 0)
            {
                int existingStateIndex = containsState(states, numStates, newState);
                if (existingStateIndex == -1)
                {
                    states[numStates++] = newState;
                }
            }
        }
        for (char symbol = 'a'; symbol <= 'z'; symbol++)
        {
            State newState = gotoState(&currentState, symbol);
            if (newState.numItems > 0)
            {
                int existingStateIndex = containsState(states, numStates, newState);
                if (existingStateIndex == -1)
                {
                    states[numStates++] = newState;
                }
            }
        }
    }
}

// Function to generate the ACTION table
void generateActionTable()
{
    printf("\nRYAN JACOB 22BCT0356\tACTION Table:\n");
    for (int i = 0; i < numStates; i++)
    {
        State currentState = states[i];
        printf("RYAN JACOB 22BCT0356\tState %d:\n", i);

        for (int j = 0; j < 128; j++)
        {
            actionTable[i][j] = -1; // Initialize ACTION table with -1 (invalid action)
        }

        for (int j = 0; j < currentState.numItems; j++)
        {
            LR1Item item = currentState.items[j];
            if (item.dotPosition < strlen(grammar[item.ruleIndex].rhs))
            {
                char nextSymbol = grammar[item.ruleIndex].rhs[item.dotPosition];
                if (nextSymbol >= 'a' && nextSymbol <= 'z')
                {
                    State newState = gotoState(&currentState, nextSymbol);
                    int gotoStateIndex = containsState(states, numStates, newState);
                    if (gotoStateIndex != -1)
                    {
                        printf("RYAN JACOB 22BCT0356\tACTION[%d, %c] = Shift %d\n", i, nextSymbol, gotoStateIndex);
                        actionTable[i][nextSymbol] = gotoStateIndex + 1; // Shift action (positive)
                    }
                }
            }
            else
            {
                if (item.ruleIndex == 0 && item.lookahead == '$')
                {
                    printf("RYAN JACOB 22BCT0356\tACTION[%d, $] = Accept\n", i);
                    actionTable[i]['$'] = 0; // Accept action
                }
                else
                {
                    printf("RYAN JACOB 22BCT0356\tACTION[%d, %c] = Reduce %d\n", i, item.lookahead, item.ruleIndex);
                    actionTable[i][item.lookahead] = -item.ruleIndex; // Reduce action (negative)
                }
            }
        }
    }
}

// Function to generate the GOTO table
void generateGotoTable()
{
    printf("\nRYAN JACOB 22BCT0356\tGOTO Table:\n");
    for (int i = 0; i < numStates; i++)
    {
        State currentState = states[i];
        printf("RYAN JACOB 22BCT0356\tState %d:\n", i);

        for (int j = 0; j < 128; j++)
        {
            gotoTable[i][j] = -1; // Initialize GOTO table with -1 (invalid state)
        }

        for (char symbol = 'A'; symbol <= 'Z'; symbol++)
        {
            State newState = gotoState(&currentState, symbol);
            if (newState.numItems > 0)
            {
                int gotoStateIndex = containsState(states, numStates, newState);
                if (gotoStateIndex != -1)
                {
                    printf("RYAN JACOB 22BCT0356\tGOTO[%d, %c] = %d\n", i, symbol, gotoStateIndex);
                    gotoTable[i][symbol] = gotoStateIndex; // GOTO transition
                }
            }
        }
    }
}

// Function to parse an input string using the generated tables
void parseString(char *input)
{
    int stack[MAX_STACK];
    int top = -1;
    stack[++top] = 0; // Push initial state (0)

    int inputLen = strlen(input);
    input[inputLen] = '$'; // End of input marker
    int inputIndex = 0;

    while (1)
    {
        int state = stack[top];
        char symbol = input[inputIndex];

        if (actionTable[state][symbol] > 0)
        { // Shift
            int nextState = actionTable[state][symbol] - 1;
            printf("RYAN JACOB 22BCT0356\tShift to state %d\n", nextState);
            stack[++top] = nextState;
            inputIndex++;
        }
        else if (actionTable[state][symbol] < 0)
        { // Reduce
            int ruleIndex = -actionTable[state][symbol];
            printf("RYAN JACOB 22BCT0356\tReduce: %c -> %s\n", grammar[ruleIndex].lhs, grammar[ruleIndex].rhs);
            top -= strlen(grammar[ruleIndex].rhs);                   // Pop stack for the length of RHS
            state = stack[top];                                      // New state after reduction
            stack[++top] = gotoTable[state][grammar[ruleIndex].lhs]; // GOTO action
        }
        else if (actionTable[state][symbol] == 0)
        { // Accept
            printf("RYAN JACOB 22BCT0356\tInput string is accepted.\n");
            break;
        }
        else
        {
            printf("RYAN JACOB 22BCT0356\tSyntax error.\n");
            break;
        }
    }
}

int main()
{
    // Define your grammar here
    addRule('S', "CC");
    addRule('C', "cC");
    addRule('C', "d");

    // Print the grammar
    printGrammar();

    // Generate the parsing tables
    generateParsingTables();
    generateActionTable();
    generateGotoTable();

    // Test parsing
    char input[100];
    printf("RYAN JACOB 22BCT0356\tEnter a string to parse: ");
    scanf("%s", input);

    parseString(input);

    return 0;
}