
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <cctype>
using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;

    Node(string v, Node* l = nullptr, Node* r = nullptr)
        : value(v), left(l), right(r) {}
};

class ExpressionTree {
    string expr;
    int pos = 0;

    // Construct an AST using operator precedence.
    Node* parseExpression() {
        Node* node = parseTerm();

        while (pos < expr.size() &&
               (expr[pos] == '+' || expr[pos] == '-')) {
            char op = expr[pos++];
            Node* right = parseTerm();
            node = new Node(string(1, op), node, right);
        }
        return node;
    }

    Node* parseTerm() {
        Node* node = parseFactor();

        while (pos < expr.size() &&
               (expr[pos] == '*' || expr[pos] == '/')) {
            char op = expr[pos++];
            Node* right = parseFactor();
            node = new Node(string(1, op), node, right);
        }
        return node;
    }

    Node* parseFactor() {
        if (pos >= expr.size())
            return nullptr;

        if (expr[pos] == '(') {
            pos++;
            Node* node = parseExpression();

            if (pos < expr.size() && expr[pos] == ')')
                pos++;

            return node;
        }

        if (isalnum(static_cast<unsigned char>(expr[pos]))) {
            string operand;

            while (pos < expr.size() &&
                   isalnum(static_cast<unsigned char>(expr[pos]))) {
                operand += expr[pos++];
            }

            return new Node(operand);
        }

        return nullptr;
    }

    // Print the tree/DAG sideways.
    
void printTree(Node* node, string prefix = "", bool isLast = true) {
    if (node == nullptr)
        return;

    cout << prefix;
    cout << (isLast ? "`-- " : "|-- ");
    cout << node->value << '\n';

    string newPrefix = prefix + (isLast ? "    " : "|   ");

    if (node->left && node->right) {
        printTree(node->left, newPrefix, false);
        printTree(node->right, newPrefix, true);
    }
    else if (node->left) {
        printTree(node->left, newPrefix, true);
    }
    else if (node->right) {
        printTree(node->right, newPrefix, true);
    }
}


    // Create a DAG by interning nodes with identical subexpressions.
    map<tuple<string, Node*, Node*>, Node*> shared;

    Node* makeDAG(Node* node) {
        if (!node) return nullptr;

        Node* left = makeDAG(node->left);
        Node* right = makeDAG(node->right);

        auto key = make_tuple(node->value, left, right);

        auto it = shared.find(key);
        if (it != shared.end())
            return it->second;

        Node* result = new Node(node->value, left, right);
        shared[key] = result;
        return result;
    }

public:
    explicit ExpressionTree(string s) : expr(s) {}

    void build() {
        pos = 0;
        Node* ast = parseExpression();

        if (pos != expr.size()) {
            cout << "Invalid expression near position "
                 << pos << '\n';
            return;
        }

        cout << "\nAbstract Syntax Tree (AST):\n";
        printTree(ast);

        shared.clear();
        Node* dag = makeDAG(ast);

        cout << "\nDirected Acyclic Graph (DAG):\n";
        printTree(dag);
    }
};

int main() {
    string expression;

    cout << "Enter an arithmetic expression: ";
    cin >> expression;

    ExpressionTree tree(expression);
    tree.build();

    return 0;
}
