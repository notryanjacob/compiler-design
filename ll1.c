// ll1 uses parsing table. recursive descent parser user if/else to determine parsing
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char lookahead;
char input[100];
int position = 0;

void E();
void E_prime();
void T();
void T_prime();
void F();

// fn to match lookahead char with expected char
void match(char expected)
{
    if (lookahead == expected)
    {
        position++;
        lookahead = input[position];
    }
    else
    {
        printf("Error: Expected %c, but got %c", expected, lookahead);
        exit(1);
    }
}
// fn to E ->TE'
void E()
{
    T();
    E_prime();
}
// fn to E' -> +TE' | e
void E_prime()
{
    if (lookahead == '+')
    {
        match('+');
        T();
        E_prime();
    }
    // E' -> e (do nothing)
}
// fn T -> FT'
void T()
{
    F();
    T_prime();
}
// fn T' -> *FT' | e
void T_prime()
{
    if (lookahead == '*')
    {
        match('*');
        F();
        T_prime();
    }
    // else do nothing T -> e
}
// F -> (E) | id
void F()
{
    if (lookahead == '(')
    {
        match('(');
        E();
        match(')');
    }
    else if (lookahead == 'i')
    { // i short for id
        match('i');
    }
    else
    {
        printf("Error : Unexpected symbol %c\n", lookahead);
        exit(1);
    }
}

int main()
{
    printf("Enter input string (ending with $) : ");
    scanf("%s", input);
    // E ->TE'
    // E'->+TE' | e
    // T -> FT'
    // T' -> *FT' | e
    // F ->(E) | id

    lookahead = input[position];

    E(); // start parsing at start symbol

    if (lookahead == '$')
    {
        printf("String successfully parsed \n");
    }
    else
    {
        printf("Error : Unexpected end on input");
    }
    return 0;
}