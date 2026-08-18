%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%%

S:
      '0' S '1'       /* CHANGED: was '0' S */
    | '0' '1'
    ;

%%

int yylex(void)
{
    int c = getchar();

    if(c=='\n')
        return 0;

    if (c == '0' || c == '1')
        return c;

    return c;
}

void yyerror(const char *s)
{
    /* Do nothing */
}

int main(void)
{
    printf("Enter a string: ");

    if (yyparse() == 0)
        printf("Accepted\n");
    else
        printf("Rejected\n");

    return 0;
}
