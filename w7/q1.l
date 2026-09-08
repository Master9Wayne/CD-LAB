%{
#include "y.tab.h"
#include <stdio.h>
%}

%option noyywrap yylineno

%%

"for"       { printf("FOR      %s  line %d\n", yytext, yylineno); return FOR; }
"while"     { printf("WHILE    %s  line %d\n", yytext, yylineno); return WHILE; }
"int"       { printf("INT      %s  line %d\n", yytext, yylineno); return INT; }
"float"     { printf("FLOAT    %s  line %d\n", yytext, yylineno); return FLOAT; }

"<="        { printf("LE       %s  line %d\n", yytext, yylineno); return LE; }
">="        { printf("GE       %s  line %d\n", yytext, yylineno); return GE; }
"=="        { printf("EQ       %s  line %d\n", yytext, yylineno); return EQ; }
"!="        { printf("NE       %s  line %d\n", yytext, yylineno); return NE; }

"++"        { printf("INC      %s  line %d\n", yytext, yylineno); return INC; }
"--"        { printf("DEC      %s  line %d\n", yytext, yylineno); return DEC; }

[0-9]+\.[0-9]+ { printf("FLOATNUM %s  line %d\n", yytext, yylineno); return FNUM; }
[0-9]+         { printf("NUMBER   %s  line %d\n", yytext, yylineno); return NUM; }

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("ID       %s  line %d\n", yytext, yylineno);
    return ID;
}

"+"         { printf("PLUS     %s  line %d\n", yytext, yylineno); return PLUS; }
"-"         { printf("MINUS    %s  line %d\n", yytext, yylineno); return MINUS; }
"*"         { printf("MUL      %s  line %d\n", yytext, yylineno); return MUL; }
"/"         { printf("DIV      %s  line %d\n", yytext, yylineno); return DIV; }

"<"         { printf("LT       %s  line %d\n", yytext, yylineno); return LT; }
">"         { printf("GT       %s  line %d\n", yytext, yylineno); return GT; }

"="         { printf("ASSIGN   %s  line %d\n", yytext, yylineno); return ASSIGN; }

"("         { printf("LPAREN   %s  line %d\n", yytext, yylineno); return '('; }
")"         { printf("RPAREN   %s  line %d\n", yytext, yylineno); return ')'; }
"{"         { printf("LBRACE   %s  line %d\n", yytext, yylineno); return '{'; }
"}"         { printf("RBRACE   %s  line %d\n", yytext, yylineno); return '}'; }
";"         { printf("SEMICOLON %s line %d\n", yytext, yylineno); return ';'; }

[ \t\r\n]+  ;

. {
    printf("Lexical error: %s at line %d\n", yytext, yylineno);
}

%%