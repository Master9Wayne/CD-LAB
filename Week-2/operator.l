%{
#include <stdio.h>

int arithmetic = 0, relational = 0, logical = 0;
int assignment = 0, bitwise = 0, incdec = 0, ternary = 0;
%}

%%
"++"|"--"                  { incdec++; }

"+"|"-"|"*"|"/"|"%"        { arithmetic++; }

"=="|"!="|">="|"<="|">"|"<" { relational++; }

"&&"|"||"|"!"              { logical++; }

"="|"+="|"-="|"*="|"/="|"%=" { assignment++; }

"&"|"|"|"^"|"~"|"<<"|">>"  { bitwise++; }

"?"|":"                    { ternary++; }

.|\n                       ;
%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter C++ program:\n");
    yylex();

    printf("\nOperator Counts:\n");
    printf("Arithmetic      : %d\n", arithmetic);
    printf("Relational      : %d\n", relational);
    printf("Logical         : %d\n", logical);
    printf("Assignment      : %d\n", assignment);
    printf("Bitwise         : %d\n", bitwise);
    printf("Increment/Decrement : %d\n", incdec);
    printf("Ternary         : %d\n", ternary);

    return 0;
}
