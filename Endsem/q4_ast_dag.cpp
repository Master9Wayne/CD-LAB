#include <iostream>
#include <string>
#include <map>
#include <tuple>
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
    int pos = 0, id = 0;

    map<tuple<string, Node*, Node*>, Node*> shared;
    map<Node*, int> ids;
    map<Node*, bool> shown;

    Node* parseExpression() {
        Node* node = parseTerm();

        while (pos < expr.size() &&
               (expr[pos] == '+' || expr[pos] == '-')) {
            char op = expr[pos++];
            node = new Node(string(1, op), node, parseTerm());
        }
        return node;
    }

    Node* parseTerm() {
        Node* node = parseFactor();

        while (pos < expr.size() &&
               (expr[pos] == '*' || expr[pos] == '/')) {
            char op = expr[pos++];
            node = new Node(string(1, op), node, parseFactor());
        }
        return node;
    }

    Node* parseFactor() {
        if (pos >= expr.size()) return nullptr;

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
                   isalnum(static_cast<unsigned char>(expr[pos])))
                operand += expr[pos++];

            return new Node(operand);
        }

        return nullptr;
    }

    void assignIds(Node* node) {
        if (!node || ids.count(node)) return;

        ids[node] = id++;
        assignIds(node->left);
        assignIds(node->right);
    }

    void showAST(Node* node) {
        if (!node) return;

        cout << "Node " << ids[node] << " [" << node->value << "]";

        if (node->left)
            cout << " -> Left: Node " << ids[node->left];

        if (node->right)
            cout << ", Right: Node " << ids[node->right];

        cout << '\n';

        showAST(node->left);
        showAST(node->right);
    }

    Node* makeDAG(Node* node) {
        if (!node) return nullptr;

        Node* l = makeDAG(node->left);
        Node* r = makeDAG(node->right);

        auto key = make_tuple(node->value, l, r);

        if (shared.count(key))
            return shared[key];

        Node* n = new Node(node->value, l, r);
        shared[key] = n;

        return n;
    }

    void showDAGOnce(Node* node) {
        if (!node || shown.count(node)) return;

        shown[node] = true;

        cout << "Node " << ids[node] << " [" << node->value << "]";

        if (node->left)
            cout << " -> Left: Node " << ids[node->left];

        if (node->right)
            cout << ", Right: Node " << ids[node->right];

        cout << '\n';

        showDAGOnce(node->left);
        showDAGOnce(node->right);
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

        ids.clear();
        id = 0;
        assignIds(ast);
        showAST(ast);

        shared.clear();
        ids.clear();
        shown.clear();
        id = 0;

        Node* dag = makeDAG(ast);

        assignIds(dag);

        cout << "\nDirected Acyclic Graph (DAG):\n";
        showDAGOnce(dag);
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