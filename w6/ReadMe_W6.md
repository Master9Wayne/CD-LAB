# CD Week 6 - Lex and Yacc

## Files

w6q1.l
w6q1.y
w6q2.l
w6q2.y

The programs use Lex for lexical analysis and Yacc for syntax analysis.

==================================================
Q1 - CONDITIONAL STATEMENTS
==================================================

Supported grammar:

if (condition) statement

A statement can be:

identifier = expression;

or another nested if statement.

Supported relational operators:

<
>
<=
>=
==
!=

Supported expressions:

identifier
number

Examples of accepted input:

if (a > b) x = 10;

if (a < 10) x = 5;

if (a == b) x = y;

Nested if:

if (a > b) if (c < d) x = 10;

Examples of rejected input:

if a > b x = 10;

if (a > b) x = 10

if (a > b x = 10;

C-style blocks are NOT supported:

if (a > b) {
    x = 10;
}


==================================================
Q2 - FOR STATEMENTS
==================================================

Supported grammar:

for (initialization; condition; increment) statement

Initialization:

identifier = expression

Condition operators:

<
>
<=
>=
==
!=

Increment:

identifier = expression

Supported expressions:

identifier
number
expression + expression
expression - expression

Examples of accepted input:

for (i = 0; i < 10; i = i + 1) x = x + i;

for (i = 10; i > 0; i = i - 1) x = 5;

for (i = 0; i <= 10; i = i + 1) x = 10;

Nested for:

for (i = 0; i < 10; i = i + 1) for (j = 0; j < 5; j = j + 1) x = 5;

Examples of rejected input:

for (int i = 0; i < 10; i = i + 1) x = 5;

for (i = 0; i < 10; i++) x = 5;

for (i = 0; i < 10; i = i + 1) {
    x = x + i;
}


==================================================
HOW TO RUN
==================================================

Make sure Lex, Yacc and GCC are installed.

Check installation:

lex --version
yacc --version
gcc --version


--------------------------------------------------
RUN Q1
--------------------------------------------------

lex w6q1.l
yacc -d w6q1.y
gcc lex.yy.c y.tab.c -o w6q1
./w6q1

Example input:

if (a > b) x = 10;

Expected output:

Accepted


--------------------------------------------------
RUN Q2
--------------------------------------------------

Remove the generated files first:

rm -f lex.yy.c y.tab.c y.tab.h

Then run:

lex w6q2.l
yacc -d w6q2.y
gcc lex.yy.c y.tab.c -o w6q2
./w6q2

Example input:

for (i = 0; i < 10; i = i + 1) x = x + i;

Expected output:

Accepted


==================================================
NOTE
==================================================

The programs only check whether the input follows
the specified grammar. They do not execute the
if or for statements.
