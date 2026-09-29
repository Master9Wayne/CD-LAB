%{
#include <stdio.h>
%}

%%
[0-9]+[a-zA-Z_][a-zA-Z0-9_]*   { printf("Invalid Identifier: %s\n", yytext); }

[0-9]+\.[0-9]+\.[0-9]+         { printf("Invalid Numeric Constant: %s\n", yytext); }

[@#$%^&~`]                     { printf("Illegal Character: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*         ;   /* Valid identifier */

[0-9]+(\.[0-9]+)?             ;   /* Valid number */

[ \t\n]                       ;   /* Ignore spaces */

.                              ;   /* Ignore other valid symbols */
%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter C++ program:\n");
    yylex();
    return 0;
}
