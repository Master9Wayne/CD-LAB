#include <bits/stdc++.h>
using namespace std;

struct Node {
    string value;
    Node *left, *right;
    Node(string v, Node *l=nullptr, Node *r=nullptr)
        : value(move(v)), left(l), right(r) {}
};

map<string,Node*> interned;

Node* makeNode(const string& v, Node* l=nullptr, Node* r=nullptr) {
    string key = v + "|" + to_string((uintptr_t)l) + "|" + to_string((uintptr_t)r);
    if (interned.count(key)) return interned[key];
    return interned[key] = new Node(v,l,r);
}

void printTree(Node* root, string prefix="", bool last=true) {
    if (!root) return;
    cout << prefix << (last ? "`-- " : "|-- ") << root->value << '\n';
    vector<Node*> children;
    if (root->left) children.push_back(root->left);
    if (root->right) children.push_back(root->right);
    for (int i=0; i<(int)children.size(); ++i)
        printTree(children[i], prefix + (last ? "    " : "|   "), i==(int)children.size()-1);
}

int main() {
    // Illustrates AST and DAG for a+b*c+b*c.
    Node *a=makeNode("a"), *b=makeNode("b"), *c=makeNode("c");
    Node *mul=makeNode("*",b,c);           // shared subexpression
    Node *sum1=makeNode("+",a,mul);
    Node *root=makeNode("+",sum1,mul);

    cout << "DAG representation (shared b*c node):\n";
    printTree(root);
    cout << "Unique nodes: " << interned.size() << '\n';
    cout << "The repeated b*c expression points to the same node.\n";
}
