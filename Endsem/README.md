# Compiler Design – 10 C++ Programs

All programs are standalone C++17 source files.

## Files
1. `01_left_recursion.cpp` – removes immediate left recursion.
2. `02_left_factoring.cpp` – factors a common prefix from alternatives.
3. `03_LL1_parser.cpp` – table-driven LL(1) parser demonstration.
4. `04_SLR1_parser.cpp` – table-driven SLR(1) parser demonstration.
5. `05_AST_DAG.cpp` – demonstrates AST/DAG sharing for `a+b*c+b*c`.
6. `06_FIRST_FOLLOW.cpp` – computes FIRST and FOLLOW sets.
7. `07_operator_precedence_demo.cpp` – validates expressions with operator precedence (`i`, `+`, `*`, parentheses).
8. `08_CLR1_parser.cpp` – CLR(1) table-driven parser demonstration.
9. `09_LALR1_parser.cpp` – LALR(1) table-driven parser demonstration.
10. `10_TAC_quadruples_triples.cpp` – generates TAC, quadruples and triples for assignment expressions.

## Compile and run

For any file:

```bash
g++ -std=c++17 filename.cpp -o program
./program
```

Example:

```bash
g++ -std=c++17 10_TAC_quadruples_triples.cpp -o tac
./tac
```

## Important notes

- The LL(1), SLR(1), CLR(1), and LALR(1) programs contain illustrative parsing tables. For a different grammar, construct and enter that grammar's corresponding ACTION/GOTO or LL(1) table.
- The operator-precedence demonstration recognizes arithmetic expressions with `i` as an operand, `*` higher precedence than `+`, and parentheses. It uses recursive precedence parsing rather than a general handle-reduction table implementation.
- The left-recursion and left-factoring programs handle one non-terminal and a single group of alternatives; they are intended as lab-level demonstrations, not a complete grammar transformation toolkit.
- In FIRST/FOLLOW input, enter alternatives separated by `|`, use `#` for epsilon, and use single-character grammar symbols. Example: `E=TR|#`.
