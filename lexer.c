#include <stdio.h>
#include <string.h>
#include <ctype.h>

void analyze(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (isspace(str[i]))
        {
            i++;
            continue;
        }
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=')
        {
            printf("Operator : %c \n", str[i]);
        }
        if (isdigit(str[i]))
        {
            printf("Number : ");
            while (isdigit(str[i]))
                printf("%c", str[i++]);
            printf("\n");
            continue;
        }
        if (isalpha(str[i]))
        {
            char buffer[100];
            int j = 0;
            while (isalpha(str[i]))
                buffer[j++] = str[i++];
            buffer[j] = '\0';
            if (strcmp(buffer, "int") == 0 || strcmp(buffer, "return") == 0)
            {
                printf("Keyword : %s \n", buffer);
            }
            else
            {
                printf("Identifier : %s\n", buffer);
            }
            continue;
        }
        if (str[i] == ';')
        {
            printf("Delimeter : %c\n", str[i++]);
            continue;
        }

        i++;
    }
}

int main()
{
    const char *code = "int x = 10 + y; return x;";
    analyze(code);
    return 0;
}