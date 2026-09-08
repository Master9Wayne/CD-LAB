%{
#include "y.tab.h"
#include <stdio.h>
%}

%option noyywrap yylineno

%%

"int"       { printf("INT      %s  line %d\n", yytext, yylineno); return INT; }
"float"     { printf("FLOAT    %s  line %d\n", yytext, yylineno); return FLOAT; }
"void"      { printf("VOID     %s  line %d\n", yytext, yylineno); return VOID; }
"return"    { printf("RETURN   %s  line %d\n", yytext, yylineno); return RETURN; }

[0-9]+\.[0-9]+ {
    printf("FLOATNUM %s  line %d\n", yytext, yylineno);
    return FNUM;
}

[0-9]+ {
    printf("NUMBER   %s  line %d\n", yytext, yylineno);
    return NUM;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("ID       %s  line %d\n", yytext, yylineno);
    return ID;
}

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }

"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
";"         { return ';'; }
","         { return ','; }

[ \t\r\n]+  ;

. {
    printf("Lexical error: '%s' at line %d\n", yytext, yylineno);
}

%%