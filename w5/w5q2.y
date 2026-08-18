%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%%

S:
      'a' S          /* CHANGED */
    | 'a' B          /* CHANGED */
    ;

B:
      'b' B          /* ADDED */
    | 'b' C          /* ADDED */
    ;

C:
      'c'            /* ADDED */
    ;

%%

int yylex(void)
{
    int c = getchar();

    if(c=='\n')
        return 0;

    if (c == 'a' || c == 'b' || c == 'c')   /* CHANGED */
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
