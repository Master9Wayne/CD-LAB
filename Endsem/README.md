COMPILER DESIGN - 10 C++ PROGRAMS
================================

Files
-----
01_left_recursion_factoring.cpp
    Remove immediate left recursion and perform left factoring.

02_LL1_parser.cpp
    Table-driven LL(1) parser using a predefined parsing table.

03_SLR1_parser.cpp
    SLR(1) parser using supplied ACTION/GOTO tables.

04_AST_DAG.cpp
    Construct an Abstract Syntax Tree (AST) and Directed Acyclic Graph (DAG).

05_FIRST_FOLLOW.cpp
    Compute FIRST and FOLLOW sets for a CFG.

06_operator_precedence_parser.cpp
    Table-driven operator-precedence parser. Enter terminals, precedence
    relations, grammar productions, start symbol, and input string.
    This replaces the earlier precedence demo with a stack-based
    handle-reduction implementation. It supports single-character symbols,
    epsilon-free operator-precedence grammars, and no adjacent non-terminals
    on a production RHS.

07_CLR1_parser.cpp
    Canonical LR(1) parser using supplied ACTION/GOTO tables.

08_LALR1_parser.cpp
    LALR(1) parser using supplied ACTION/GOTO tables.

09_RDP.cpp
    Recursive-descent parser with backtracking for a user-entered grammar.

10_TAC_quadruples_triples.cpp
    Generate three-address code, quadruples, and triples from an assignment.

Compile any program
-------------------
g++ -std=c++17 filename.cpp -o program
./program

Example:
g++ -std=c++17 06_operator_precedence_parser.cpp -o op_parser
./op_parser

Notes
-----
- The LR/SLR/LALR programs contain demonstration parsing tables. For a new
  grammar, the productions and corresponding ACTION/GOTO tables must be
  updated consistently.
- The LL(1) program likewise uses a hardcoded parsing table; update its table
  and start symbol for a different grammar.
- The operator-precedence parser asks for the table and productions at runtime.
- All programs were checked for C++17 syntax. The operator-precedence parser
  was also tested on i+i*i and (i+i)*i with the example table in its comments.
