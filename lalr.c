#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_PRODUCTIONS 100
#define MAX_STATES 200
#define MAX_STACK 1000

typedef struct
{
    char le;
    char right[10];
} Production;

typedef struct
{
    int dot_position;
    int production_index;
    char lookahead;
} Item;

typedef struct
{
    Item items[MAX_PRODUCTIONS];
    int num_items;
} State;

Production productions[MAX_PRODUCTIONS];
int num_productions = 0;

State states[MAX_STATES];
int num_states = 0;

char symbols[MAX_SYMBOLS];
int num_symbols = 0;

int goto_table[MAX_STATES][MAX_SYMBOLS];
char action_table[MAX_STATES][MAX_SYMBOLS];

void add_production(char le, const char *right)
{
    Production p = {le, ""};
    strcpy(p.right, right);
    productions[num_productions++] = p;
}

void add_symbol(char symbol)
{
    for (int i = 0; i < num_symbols; i++)
    {
        if (symbols[i] == symbol)
            return;
    }
    symbols[num_symbols++] = symbol;
}

void init_grammar()
{
    // Example Grammar:
    // S -> AB | BS | ab
    add_production('S', "AB");
    add_production('S', "BS");
    add_production('S', "ab");

    // A -> abc | b
    add_production('A', "abc");
    add_production('A', "b");

    // B -> c
    add_production('B', "c");

    // Add symbols
    add_symbol('S');
    add_symbol('A');
    add_symbol('B');
    add_symbol('a');
    add_symbol('b');
    add_symbol('c');
    add_symbol('*'); // Added to handle the input string
    add_symbol('+'); // Added to handle the input string
    add_symbol('$'); // End of input
}

void compute_closure(State *state)
{
    int changed;
    do
    {
        changed = 0;
        for (int i = 0; i < state->num_items; i++)
        {
            Item *item = &state->items[i];
            char next_symbol = productions[item->production_index].right[item->dot_position];

            if (next_symbol >= 'A' && next_symbol <= 'Z')
            {
                for (int j = 0; j < num_productions; j++)
                {
                    if (productions[j].le == next_symbol)
                    {
                        Item new_item = {0, j, item->lookahead};
                        int exists = 0;

                        for (int k = 0; k < state->num_items; k++)
                        {
                            if (memcmp(&state->items[k], &new_item, sizeof(Item)) == 0)
                            {
                                exists = 1;
                                break;
                            }
                        }

                        if (!exists)
                        {
                            state->items[state->num_items++] = new_item;
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while (changed);
}

int find_or_create_state(State *new_state)
{
    for (int i = 0; i < num_states; i++)
    {
        if (memcmp(&states[i], new_state, sizeof(State)) == 0)
        {
            return i;
        }
    }
    states[num_states] = *new_state;
    return num_states++;
}

void build_lalr_automaton()
{
    State initial_state = {{{0, 0, '$'}}, 1};
    compute_closure(&initial_state);
    find_or_create_state(&initial_state);

    for (int i = 0; i < num_states; i++)
    {
        for (int j = 0; j < num_symbols; j++)
        {
            State new_state = {{}, 0};
            for (int k = 0; k < states[i].num_items; k++)
            {
                Item *item = &states[i].items[k];

                char next_symbol = productions[item->production_index].right[item->dot_position];
                if (next_symbol == symbols[j])
                {
                    Item new_item = *item;
                    new_item.dot_position++;
                    new_state.items[new_state.num_items++] = new_item;
                }
            }
            if (new_state.num_items > 0)
            {
                compute_closure(&new_state);
                int new_state_index = find_or_create_state(&new_state);
                goto_table[i][j] = new_state_index;
                if (isupper(symbols[j]))
                {
                    action_table[i][j] = 's';
                }
                else
                {
                    action_table[i][j] = 'r';
                }
            }
        }
    }
}

void print_lalr_tables()
{
    printf("LALR Parsing Tables:\n\n");

    printf("Goto Table:\n");
    printf("   ");
    for (int i = 0; i < num_symbols; i++)
    {
        printf("%c  ", symbols[i]);
    }
    printf("\n");
    for (int i = 0; i < num_states; i++)
    {
        printf("%2d ", i);
        for (int j = 0; j < num_symbols; j++)
        {
            if (goto_table[i][j] != 0)
            {
                printf("%2d ", goto_table[i][j]);
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }

    printf("\nAction Table:\n");
    printf("   ");
    for (int i = 0; i < num_symbols; i++)
    {
        printf("%c  ", symbols[i]);
    }
    printf("\n");
    for (int i = 0; i < num_states; i++)
    {
        printf("%2d ", i);
        for (int j = 0; j < num_symbols; j++)
        {
            if (action_table[i][j] != 0)
            {
                printf("%c  ", action_table[i][j]);
            }
            else
            {
                printf("   ");
            }
        }
        printf("\n");
    }
}

int parse_input(const char *input)
{
    int stack[MAX_STACK];
    int top = 0;
    stack[top] = 0;

    int input_pos = 0;
    char current_symbol = input[input_pos];

    while (1)
    {
        int current_state = stack[top];
        int symbol_index = -1;
        for (int i = 0; i < num_symbols; i++)
        {
            if (symbols[i] == current_symbol)
            {
                symbol_index = i;
                break;
            }
        }

        if (symbol_index == -1)
        {
            printf("Invalid input symbol: %c\n", current_symbol);
            return 0;
        }

        char action = action_table[current_state][symbol_index];
        int next_state = goto_table[current_state][symbol_index];

        if (action == 's')
        {
            stack[++top] = next_state;
            current_symbol = input[++input_pos];
        }
        else if (action == 'r')
        {
            int production_index = next_state;
            int rhs_length = strlen(productions[production_index].right);

            top -= rhs_length;
            int goto_state = goto_table[stack[top]][productions[production_index].le - 'A'];
            stack[++top] = goto_state;

            if (production_index == 0 && current_symbol == '$')
            {
                printf("Input accepted\n");
                return 1;
            }
        }
        else
        {
            printf("Parsing error at position %d\n", input_pos);
            return 0;
        }
    }
}

int main()
{
    init_grammar();
    build_lalr_automaton();
    print_lalr_tables();

    const char *input = "c*abcc+$"; // The string to parse
    printf("\nParsing input string: %s\n", input);

    if (parse_input(input))
    {
        printf("Parsing successful\n");
    }
    else
    {
        printf("Parsing failed\n");
    }

    return 0;
}
