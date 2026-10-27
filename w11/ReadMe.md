
# AST and DAG Construction in C++

## Aim
To construct an Abstract Syntax Tree (AST) for an arithmetic expression and convert it into a Directed Acyclic Graph (DAG) by identifying and sharing common subexpressions.

## Theory

### 1. Abstract Syntax Tree (AST)
An AST represents an arithmetic expression as a tree. Operators such as `+`, `-`, `*`, and `/` form internal nodes, while variables and constants form leaf nodes. The tree structure reflects operator precedence and parentheses.

### 2. Directed Acyclic Graph (DAG)
A DAG represents an expression as a directed graph without cycles. Unlike an AST, it allows multiple parent nodes to reference the same child node, enabling identical subexpressions to share a single representation.

### 3. Common Subexpression Elimination
Common subexpression elimination is a compiler optimization that avoids redundant computations by reusing identical expressions when it is semantically safe to do so.

## Implementation

The program uses recursive-descent parsing to construct the AST and then converts it into a DAG.

- **`parseExpression()`**: Parses addition and subtraction.
- **`parseTerm()`**: Parses multiplication and division, respecting operator precedence.
- **`parseFactor()`**: Handles operands and parenthesized expressions.
- **`makeDAG()`**: Recursively processes nodes and uses a map to identify structurally identical subexpressions. Existing nodes are reused whenever a match is found.
- **`printTree()`**: Displays the AST and DAG in a readable ASCII tree format.

Each node stores an operator or operand and pointers to its left and right children.

## Sample Input

```text
a+a*(b-c)+(b-c)*d
```

## Working

The input expression contains the repeated subexpression `b-c`.

In the AST, each occurrence of `b-c` is represented by a separate subtraction node. During DAG construction, the program identifies the identical subexpressions and makes both multiplication operations reference the same subtraction node.

This demonstrates structural sharing, which is a fundamental idea behind common subexpression elimination.

## Compilation and Execution

```bash
g++ q1w11.cpp -o ast_dag
./ast_dag
```

Replace `q1w11.cpp` with the name of your source file if necessary.

## Applications

- Compiler optimization and intermediate representations.
- Detection of repeated expressions.
- Reduction of redundant computations.
- Expression analysis and code generation.

## Conclusion

The program demonstrates how an arithmetic expression can be represented using an AST and transformed into a DAG. While the AST preserves the hierarchical structure of the expression, the DAG shares identical subexpressions, illustrating an important concept in compiler design and optimization.
