# CD Lab Week 4 — YACC Programs

This repository contains three YACC programs:

```text
q1simple.y   → Question 1: Simple arithmetic expression evaluator
q1complex.y  → Question 1: Arithmetic evaluator with detailed error handling
q2.y         → Question 2: String recognition using S → 0S | 01
```

These programs use **YACC/Bison only**. There are **no Lex/Flex files**.

The lexical analyzer is written manually using the `yylex()` function inside
each `.y` file.

---

# 1. Requirements

You need:

- GCC
- Bison

You do **not** need Flex for these programs.

## Ubuntu / WSL installation

```bash
sudo apt update
sudo apt install build-essential bison
```

Check:

```bash
g++ --version
bison --version
```

If `bison --version` prints a version such as `3.8.2`, Bison is installed.

---

# 2. How YACC Works in These Programs

Normally, a compiler-design program can use:

```text
Input
  ↓
Lex/Flex
  ↓
Tokens
  ↓
YACC
  ↓
Parser
  ↓
Output
```

Here we are not using Flex.

Instead:

```text
Input
  ↓
yylex() written inside .y file
  ↓
Tokens
  ↓
YACC parser
  ↓
Output
```

`yylex()` reads characters from the keyboard and returns tokens to
`yyparse()`.

---

# 3. General Structure of a YACC File

A YACC file has three major sections:

```text
C/C++ declarations
%%
Grammar rules
%%
C/C++ functions
```

The first:

```text
%%
```

separates the declarations from the grammar.

The second:

```text
%%
```

separates the grammar from the C/C++ functions.

---

# 4. Question 1 — q1simple.y

This is the basic arithmetic expression evaluator.

It supports:

```text
+
-
*
/
%
( )
```

Example:

```text
5+10*3
```

Output:

```text
Result = 35
```

## Complete code

```c
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/' '%'

%%

input:
      expr '\n'     { printf("Result = %d\n", $1); }
    ;

expr:
      expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | expr '%' expr { $$ = $1 % $3; }
    | '(' expr ')'  { $$ = $2; }
    | NUMBER        { $$ = $1; }
    ;

%%

int yylex(void)
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c >= '0' && c <= '9')
    {
        ungetc(c, stdin);
        scanf("%d", &yylval);
        return NUMBER;
    }

    return c;
}

void yyerror(const char *s)
{
    printf("Invalid expression\n");
}

int main(void)
{
    printf("Enter an arithmetic expression: ");
    yyparse();
    return 0;
}
```

---

# 5. q1simple.y — Line-by-Line Explanation

## C/C++ declaration section

```c
%{
```

Starts the C/C++ code section.

Everything between `%{` and `%}` is copied into the generated parser.

```c
#include <stdio.h>
```

Provides functions such as:

```c
printf()
getchar()
scanf()
```

```c
#include <stdlib.h>
```

Includes standard C library functionality.

```c
int yylex(void);
```

Declares the lexical analyzer.

YACC calls `yylex()` whenever it needs the next token.

```c
void yyerror(const char *s);
```

Declares the function used when a parsing error occurs.

```c
%}
```

Ends the C/C++ declaration section.

---

## Token declaration

```c
%token NUMBER
```

Creates a token called `NUMBER`.

When the lexer reads:

```text
25
```

it returns the token:

```text
NUMBER
```

and stores the actual value `25` in `yylval`.

---

## Operator precedence

```c
%left '+' '-'
```

Makes `+` and `-` left associative.

For example:

```text
10 - 3 - 2
```

is interpreted as:

```text
(10 - 3) - 2
```

```c
%left '*' '/' '%'
```

Makes `*`, `/`, and `%` left associative.

The precedence declarations are ordered from lower to higher precedence.

Therefore:

```text
+ -
```

have lower precedence than:

```text
* / %
```

So:

```text
5 + 10 * 3
```

is evaluated as:

```text
5 + (10 * 3)
```

giving:

```text
35
```

---

# 6. Grammar Section of q1simple.y

```c
%%
```

Marks the beginning of the grammar rules.

### Input rule

```c
input:
      expr '\n'     { printf("Result = %d\n", $1); }
    ;
```

The complete input must contain:

```text
expression + newline
```

After the expression is successfully recognized, the action runs:

```c
printf("Result = %d\n", $1);
```

`$1` contains the value of `expr`.

For example:

```text
5 + 3
```

produces:

```text
8
```

so `$1` is `8`.

---

## Addition

```c
expr '+' expr { $$ = $1 + $3; }
```

The grammar recognizes:

```text
expression + expression
```

Here:

```text
$1 = left expression
$3 = right expression
$$ = result
```

Therefore:

```c
$$ = $1 + $3;
```

calculates the addition.

---

## Subtraction

```c
| expr '-' expr { $$ = $1 - $3; }
```

Calculates:

```text
left expression - right expression
```

---

## Multiplication

```c
| expr '*' expr { $$ = $1 * $3; }
```

Calculates multiplication.

---

## Division

```c
| expr '/' expr { $$ = $1 / $3; }
```

Calculates integer division.

This simple version does not explicitly check for division by zero.

---

## Modulus

```c
| expr '%' expr { $$ = $1 % $3; }
```

Calculates the remainder.

For example:

```text
15 % 4
```

gives:

```text
3
```

---

## Parentheses

```c
| '(' expr ')' { $$ = $2; }
```

Allows expressions such as:

```text
(5 + 3) * 2
```

`$2` is the value of the expression inside the parentheses.

Therefore:

```c
$$ = $2;
```

---

## Number

```c
| NUMBER { $$ = $1; }
```

A number itself is an expression.

If the lexer reads `25`, then:

```text
$1 = 25
```

and:

```c
$$ = $1;
```

makes the expression's value `25`.

---

# 7. Lexer in q1simple.y

The second:

```text
%%
```

marks the end of the grammar.

After this we write the C function `yylex()`.

```c
int yylex(void)
```

This is the manually written lexical analyzer.

---

```c
int c;
```

Stores one input character.

---

```c
while ((c = getchar()) == ' ' || c == '\t')
    ;
```

Reads characters and ignores:

```text
spaces
tabs
```

The semicolon means the loop has an empty body.

---

```c
if (c >= '0' && c <= '9')
```

Checks whether the character is a digit.

---

```c
ungetc(c, stdin);
```

Puts the digit back into the input stream.

This is needed because the next line uses `scanf()` to read the complete number.

---

```c
scanf("%d", &yylval);
```

Reads the complete integer and stores it in `yylval`.

For example:

```text
123
```

causes:

```text
yylval = 123
```

---

```c
return NUMBER;
```

Tells YACC that the token is a `NUMBER`.

---

```c
return c;
```

If the character is an operator such as:

```text
+
-
*
/
%
(
)
```

the character itself is returned as the token.

---

# 8. Error Function in q1simple.y

```c
void yyerror(const char *s)
{
    printf("Invalid expression\n");
}
```

YACC calls `yyerror()` when the input does not match the grammar.

This version simply prints:

```text
Invalid expression
```

---

# 9. Main Function in q1simple.y

```c
int main(void)
```

Program execution starts here.

```c
printf("Enter an arithmetic expression: ");
```

Displays the input prompt.

```c
yyparse();
```

Starts the YACC parser.

The parser internally calls:

```text
yylex()
```

to obtain tokens.

```c
return 0;
```

Ends the program successfully.

---

# 10. Running q1simple.y

From the folder containing the file:

```bash
bison -d q1simple.y
```

This generates:

```text
q1simple.tab.c
q1simple.tab.h
```

Compile:

```bash
g++ q1simple.tab.c -o q1simple
```

Run:

```bash
./q1simple
```

Example:

```text
Enter an arithmetic expression: 5+10*3
Result = 35
```

---

# 11. Question 1 — q1complex.y

`q1complex.y` performs the same arithmetic evaluation but has better error handling.

It handles:

```text
Runtime Error: Division by zero
Runtime Error: Modulus by zero
Lexical Error: Invalid symbol '@'
Syntax Error: Incomplete expression
```

This is the recommended version if your lab requires specific error messages.

---

# 12. q1complex.y — Important Declarations

```c
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);

int error_flag = 0;
%}
```

The first two includes provide standard C functions.

```c
int yylex(void);
```

declares our lexer.

```c
void yyerror(const char *s);
```

declares the parser error function.

```c
int error_flag = 0;
```

stores whether an error has already occurred.

Initially:

```text
error_flag = 0
```

means no error.

---

## Token and precedence

```c
%token NUMBER
```

declares the `NUMBER` token.

```c
%left '+' '-'
%left '*' '/' '%'
```

sets associativity and precedence exactly like `q1simple.y`.

---

# 13. q1complex.y — Input Rule

```c
input:
      expr '\n'
        {
            if (!error_flag)
                printf("Result = %d\n", $1);
        }
    ;
```

The parser expects an expression followed by newline.

The important difference is:

```c
if (!error_flag)
```

The result is printed only if no runtime or lexical error occurred.

---

# 14. q1complex.y — Arithmetic Rules

Addition:

```c
expr '+' expr
{
    $$ = $1 + $3;
}
```

Subtraction:

```c
expr '-' expr
{
    $$ = $1 - $3;
}
```

Multiplication:

```c
expr '*' expr
{
    $$ = $1 * $3;
}
```

All work exactly like the simple version.

---

# 15. Division Error Handling

```c
expr '/' expr
```

recognizes division.

```c
if ($3 == 0)
```

checks whether the right operand is zero.

If it is:

```c
printf("Runtime Error: Division by zero\n");
```

prints the runtime error.

```c
error_flag = 1;
```

records that an error occurred.

```c
$$ = 0;
```

gives the expression a temporary value so parsing can continue.

Otherwise:

```c
$$ = $1 / $3;
```

performs the division.

---

# 16. Modulus Error Handling

```c
expr '%' expr
```

recognizes modulus.

```c
if ($3 == 0)
```

checks for modulo by zero.

If zero:

```c
printf("Runtime Error: Modulus by zero\n");
error_flag = 1;
$$ = 0;
```

Otherwise:

```c
$$ = $1 % $3;
```

calculates the remainder.

Example:

```text
15%4
```

produces:

```text
Result = 3
```

---

# 17. Parentheses and NUMBER

```c
| '(' expr ')'
{
    $$ = $2;
}
```

The value of `(expr)` is the value of the expression inside it.

```c
| NUMBER
{
    $$ = $1;
}
```

A number is an expression.

---

# 18. q1complex.y — Lexer

```c
int yylex(void)
{
    int c;
```

Creates the character variable `c`.

---

```c
while ((c = getchar()) == ' ' || c == '\t')
    ;
```

Ignores spaces and tabs.

---

## Reading a number

```c
if (c >= '0' && c <= '9')
```

checks for a digit.

```c
int num = 0;
```

initializes the number.

```c
do
{
    num = num * 10 + (c - '0');
    c = getchar();
}
while (c >= '0' && c <= '9');
```

Reads all consecutive digits.

For:

```text
123
```

it calculates:

```text
1
12
123
```

---

```c
ungetc(c, stdin);
```

puts the first non-digit character back into the input stream.

---

```c
yylval = num;
return NUMBER;
```

stores the number and returns the `NUMBER` token.

---

# 19. q1complex.y — Valid Symbols

```c
if (c == '+' || c == '-' ||
    c == '*' || c == '/' ||
    c == '%' || c == '(' ||
    c == ')' || c == '\n')
{
    return c;
}
```

These are the valid symbols.

They are returned directly to YACC.

---

# 20. q1complex.y — Lexical Error

```c
if (c != EOF)
{
    printf("Lexical Error: Invalid symbol '%c'\n", c);
```

If an unknown character occurs, a lexical error is printed.

For example:

```text
20@5
```

produces:

```text
Lexical Error: Invalid symbol '@'
```

---

```c
while ((c = getchar()) != '\n' && c != EOF)
    ;
```

Consumes the remaining characters on the input line.

This prevents the parser from processing the invalid input again.

---

```c
error_flag = 1;
return '\n';
```

records the error and returns newline to finish the current input.

---

# 21. q1complex.y — Syntax Error

```c
void yyerror(const char *s)
{
    if (!error_flag)
    {
        printf("Syntax Error: Incomplete expression\n");
        error_flag = 1;
    }
}
```

If YACC cannot match the grammar, `yyerror()` is called.

For:

```text
10+
```

the parser sees:

```text
NUMBER + newline
```

but the grammar requires another expression after `+`.

Therefore:

```text
Syntax Error: Incomplete expression
```

is printed.

The `if (!error_flag)` prevents another error message from being printed if a previous lexical/runtime error already occurred.

---

# 22. q1complex.y — Main

```c
int main(void)
{
    printf("Enter expression: ");

    yyparse();

    return 0;
}
```

Displays the prompt, starts the parser, and exits.

---

# 23. Running q1complex.y

Generate the parser:

```bash
bison -d q1complex.y
```

Compile:

```bash
g++ q1complex.tab.c -o q1complex
```

Run:

```bash
./q1complex
```

## Test cases

### TC1

Input:

```text
15%4
```

Output:

```text
Result = 3
```

### TC2

Input:

```text
5+10*3
```

Output:

```text
Result = 35
```

### TC3

Input:

```text
20%0
```

Output:

```text
Runtime Error: Modulus by zero
```

### TC4

Input:

```text
20@5
```

Output:

```text
Lexical Error: Invalid symbol '@'
```

### TC5

Input:

```text
10+
```

Output:

```text
Syntax Error: Incomplete expression
```

---

# 24. Question 2 — q2.y

The grammar is:

```text
S → 0S | 01
```

This grammar accepts:

```text
01
001
0001
00001
...
```

In other words:

```text
one or more 0s followed by exactly one 1
```

---

# 25. q2.y — Declaration Section

```c
%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}
```

```c
#include <stdio.h>
```

provides `printf()` and `getchar()`.

```c
int yylex(void);
```

declares the lexer.

```c
void yyerror(const char *s);
```

declares the error function.

---

# 26. q2.y — Grammar

```c
%%
```

starts the grammar.

```c
S:
      '0' S
    | '0' '1'
    ;
```

This is exactly:

```text
S → 0S | 01
```

The first rule:

```c
'0' S
```

means:

> A valid string can start with `0`, followed by another valid string.

The second rule:

```c
'0' '1'
```

is the base case.

Therefore:

```text
01
```

is valid.

---

# 27. How q2.y Accepts 0001

Starting with:

```text
S
```

Use:

```text
S → 0S
```

giving:

```text
0S
```

Again:

```text
S → 0S
```

giving:

```text
00S
```

Again:

```text
S → 0S
```

giving:

```text
000S
```

Finally use:

```text
S → 01
```

giving:

```text
0001
```

Therefore:

```text
0001
```

is accepted.

---

# 28. q2.y — Lexer

```c
int yylex(void)
{
    int c = getchar();
```

Reads one character.

---

```c
if(c=='\n')
    return 0;
```

When newline is reached, return `0` to indicate the end of input.

---

```c
if (c == '0' || c == '1')
    return c;
```

If the character is `0` or `1`, return that character as a token.

---

```c
return c;
```

Returns any other character.

Since the grammar only accepts `0` and `1`, unexpected characters cause parsing to fail.

---

# 29. q2.y — Error Function

```c
void yyerror(const char *s)
{
    /* Do nothing */
}
```

The error function does not print anything.

The decision to print `Accepted` or `Rejected` is handled in `main()`.

---

# 30. q2.y — Main

```c
int main(void)
{
    printf("Enter a string: ");
```

Displays the prompt.

```c
if (yyparse() == 0)
    printf("Accepted\n");
```

If `yyparse()` returns `0`, parsing was successful.

Therefore the string belongs to the grammar.

```c
else
    printf("Rejected\n");
```

If parsing fails, the string is rejected.

---

# 31. Running q2.y

Generate the parser:

```bash
bison -d q2.y
```

Compile:

```bash
g++ q2.tab.c -o q2
```

Run:

```bash
./q2
```

## Accepted examples

```text
01
```

Output:

```text
Accepted
```

```text
001
```

Output:

```text
Accepted
```

```text
0001
```

Output:

```text
Accepted
```

## Rejected examples

```text
1
```

```text
0
```

```text
0011
```

```text
100
```

```text
0101
```

Output:

```text
Rejected
```

---

# 32. Complete Compilation Commands

## q1simple

```bash
bison -d q1simple.y
g++ q1simple.tab.c -o q1simple
./q1simple
```

## q1complex

```bash
bison -d q1complex.y
g++ q1complex.tab.c -o q1complex
./q1complex
```

## q2

```bash
bison -d q2.y
g++ q2.tab.c -o q2
./q2
```

---

# 33. What Files Does Bison Generate?

When you run:

```bash
bison -d q1complex.y
```

Bison generates:

```text
q1complex.tab.c
q1complex.tab.h
```

Similarly:

```bash
bison -d q1simple.y
```

generates:

```text
q1simple.tab.c
q1simple.tab.h
```

and:

```bash
bison -d q2.y
```

generates:

```text
q2.tab.c
q2.tab.h
```

You do not need to manually write these files.

---

# 34. Cleaning Generated Files

To remove generated files and executables:

```bash
rm -f *.tab.c *.tab.h q1simple q1complex q2
```

Your original files remain:

```text
README.md
q1simple.y
q1complex.y
q2.y
```

---

# 35. Important YACC Terms for Viva

## `yyparse()`

Starts the parser generated by YACC/Bison.

```c
yyparse();
```

---

## `yylex()`

Returns the next token to the parser.

In these programs, we manually implement `yylex()` instead of using Flex.

---

## `yyerror()`

Called when the parser encounters an invalid input according to the grammar.

---

## `yylval`

Stores the semantic value of a token.

For example, for:

```text
123
```

the lexer does:

```c
yylval = 123;
return NUMBER;
```

---

## `NUMBER`

A token representing an integer.

Declared using:

```c
%token NUMBER
```

---

## `$1`, `$2`, `$3`

Refer to the values of symbols on the right side of a grammar rule.

For:

```c
expr '+' expr
```

we have:

```text
$1 → left expression
$2 → '+'
$3 → right expression
```

---

## `$$`

Represents the value produced by the complete grammar rule.

Example:

```c
expr '+' expr
{
    $$ = $1 + $3;
}
```

---

## `%left`

Specifies left associativity and operator precedence.

```c
%left '+' '-'
%left '*' '/' '%'
```

---

## `YYABORT`

Not used in the current `q1complex.y`; the program uses `error_flag`
instead so it can print the required error message cleanly.

---

# 36. Recommended File for Submission

If your teacher wants only one solution for Question 1, use:

```text
q1complex.y
```

because it handles:

- Arithmetic operations
- Operator precedence
- Associativity
- Parentheses
- Division by zero
- Modulus by zero
- Invalid symbols
- Incomplete expressions

For Question 2 use:

```text
q2.y
```

Final submission can therefore be:

```text
CD-Lab-Week4/
├── README.md
├── q1complex.y
├── q1simple.y
└── q2.y
```
