
# AST and DAG Construction in C++

## 1. Concepts

### Abstract Syntax Tree (AST)

An Abstract Syntax Tree (AST) is a hierarchical representation of an arithmetic expression. Each internal node represents an operator, such as `+`, `-`, `*`, or `/`, while leaf nodes represent operands, such as variables or constants.

The structure of an AST reflects the precedence and associativity of operators, as well as the grouping specified by parentheses. For example, `a+b*c` is represented as `a+(b*c)` because multiplication has higher precedence than addition.

An AST provides a structured representation of an expression that can be used for semantic analysis, optimization, and intermediate code generation.

### Directed Acyclic Graph (DAG)

A Directed Acyclic Graph (DAG) represents an expression as a directed graph containing no cycles. Unlike an AST, where each non-root node has exactly one parent, a DAG allows multiple parent nodes to reference the same child node.

This property enables identical subexpressions to share a single representation, reducing duplicated nodes and making common subexpressions easier to identify.

### Common Subexpression Elimination

Common Subexpression Elimination (CSE) is a compiler optimization technique that identifies repeated computations and reuses their results instead of evaluating the same expression multiple times, provided that doing so preserves program semantics.

For example, in `a*(b-c)+(b-c)*d`, the expression `b-c` occurs twice. A DAG can represent this subtraction using one shared node referenced by both multiplication nodes.

## 2. Code Explanation

### Node Structure

The `Node` structure represents an individual node in the expression tree or DAG.

Each node contains:
- `value`: The operator or operand stored in the node.
- `left`: A pointer to the left child.
- `right`: A pointer to the right child.

Operator nodes generally have two children, whereas operand nodes are leaves without children. The constructor initializes the node's value and child pointers.

### Recursive-Descent Parsing

The program uses recursive-descent parsing to construct the AST. It processes the expression from left to right using three functions that correspond to different levels of operator precedence.

**`parseExpression()`**

This function handles addition and subtraction. It first parses a term and then repeatedly processes `+` or `-` operators. For each operator, it creates a new node whose left child is the previously constructed expression and whose right child is the next parsed term.

**`parseTerm()`**

This function handles multiplication and division. It parses a factor and repeatedly processes `*` or `/` operators, creating operator nodes for the corresponding subexpressions.

Because `parseExpression()` calls `parseTerm()`, multiplication and division receive higher precedence than addition and subtraction.

**`parseFactor()`**

This function handles operands and parenthesized expressions. If it encounters an opening parenthesis, it recursively calls `parseExpression()` to parse the enclosed expression and then advances past the closing parenthesis. Otherwise, it reads an alphanumeric operand and creates a leaf node.

Together, these functions construct an AST that respects operator precedence and parentheses.

### AST Printing

The `printTree()` function displays the AST in a readable ASCII format. It recursively traverses the tree, printing each node with an appropriate prefix.

The `prefix` string maintains indentation, while `isLast` determines whether a branch is printed using `` `-- `` or `|--`. This makes parent-child relationships and subtree boundaries easier to understand.

### DAG Construction Using `makeDAG()`

The `makeDAG()` function recursively transforms the AST into a DAG.

For each node, it first processes the left and right subtrees. It then creates a key using a tuple containing the node's value and the pointers to its converted children.

The `shared` map stores previously created DAG nodes using these keys.

- If the key already exists, the function returns the existing node.
- Otherwise, it creates a new node, stores it in the map, and returns it.

Consequently, nodes representing identical operators with identical children are shared. This structural sharing eliminates duplicate representations of common subexpressions.

### DAG Node Identification

The `printDAG()` function assigns a unique integer ID to each distinct DAG node. The `ids` map associates each node pointer with its assigned ID.

Before assigning an ID, the function checks whether the node has already been visited. This prevents shared nodes from receiving multiple IDs and ensures that each distinct node is identified only once.

### DAG Display Using `showDAGOnce()`

The `showDAGOnce()` function prints each DAG node once, displaying its ID, value, and the IDs of its left and right children.

The `shown` map tracks nodes that have already been displayed. When a shared node is encountered again, the function skips it, avoiding duplicate output.

This representation makes shared relationships visible through node references rather than printing the DAG as an ordinary tree.

## 3. Example and Working

**Input:**

```text
a+a*(b-c)+(b-c)*d
```

The expression contains the repeated subexpression `b-c`.

During AST construction, each occurrence of `b-c` is represented by a separate subtraction node. During DAG construction, `makeDAG()` recognizes that both subtraction nodes have the same value and corresponding children.

The existing subtraction node is reused, allowing both multiplication nodes to reference the same node. The resulting DAG therefore shares the representation of `b-c`.

## 4. Conclusion

The program demonstrates recursive-descent parsing, AST construction, and DAG generation. The AST captures the hierarchical structure of an arithmetic expression, while the DAG identifies and shares structurally identical subexpressions. These concepts form an important foundation for compiler optimization and intermediate representation.


