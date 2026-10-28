#include <iostream>
#include <string>
#include <memory>
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
    int pos = 0;
    int id = 0;
    map<tuple<string, Node*, Node*>, Node*> shared;
    map<Node*, int> ids;

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
            if (pos < expr.size() && expr[pos] == ')') pos++;
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

    void printTree(Node* node, string prefix = "", bool isLast = true) {
        if (!node) return;
        cout << prefix << (isLast ? "`-- " : "|-- ")
             << node->value << '\n';
        string p = prefix + (isLast ? "    " : "|   ");
        if (node->left && node->right) {
            printTree(node->left, p, false);
            printTree(node->right, p, true);
        } else if (node->left) printTree(node->left, p);
        else if (node->right) printTree(node->right, p);
    }

    Node* makeDAG(Node* node) {
        if (!node) return nullptr;
        Node* l = makeDAG(node->left);
        Node* r = makeDAG(node->right);
        auto key = make_tuple(node->value, l, r);
        if (shared.count(key)) return shared[key];
        Node* n = new Node(node->value, l, r);
        shared[key] = n;
        return n;
    }

    void printDAG(Node* n) {
        if (!n || ids.count(n)) return;
        ids[n] = id++;
        printDAG(n->left);
        printDAG(n->right);
    }

    void showDAG(Node* n) {
        if (!n) return;
        cout << "Node " << ids[n] << " [" << n->value << "]";
        if (n->left) cout << " -> Left: Node " << ids[n->left];
        if (n->right) cout << ", Right: Node " << ids[n->right];
        cout << '\n';
        if (n->left) showDAGOnce(n->left);
        if (n->right) showDAGOnce(n->right);
    }

    void showDAGOnce(Node* n) {
        if (!n || shown.count(n)) return;
        shown[n] = true;
        cout << "Node " << ids[n] << " [" << n->value << "]";
        if (n->left) cout << " -> Left: Node " << ids[n->left];
        if (n->right) cout << ", Right: Node " << ids[n->right];
        cout << '\n';
        showDAGOnce(n->left);
        showDAGOnce(n->right);
    }

    map<Node*, bool> shown;

public:
    explicit ExpressionTree(string s) : expr(s) {}

    void build() {
        pos = 0;
        Node* ast = parseExpression();

        if (pos != expr.size()) {
            cout << "Invalid expression near position " << pos << '\n';
            return;
        }

        cout << "\nAbstract Syntax Tree (AST):\n";
        printTree(ast);

        shared.clear();
        ids.clear();
        shown.clear();
        id = 0;

        Node* dag = makeDAG(ast);
        printDAG(dag);

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