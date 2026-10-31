#include <bits/stdc++.h>
using namespace std;

struct Node {
    string val;
    Node *left=nullptr, *right=nullptr;
    Node(string v):val(move(v)){}
    Node(string v,Node*l,Node*r):val(move(v)),left(l),right(r){}
};

string expr;
size_t pos=0;
int tempNo=1;

Node* expression();
Node* term();
Node* factor();

Node* factor() {
    if(pos>=expr.size()) return nullptr;
    if(expr[pos]=='(') {
        ++pos; Node* n=expression();
        if(pos>=expr.size() || expr[pos]!=')') return nullptr;
        ++pos; return n;
    }
    size_t start=pos;
    while(pos<expr.size() &&
          (isalnum((unsigned char)expr[pos]) || expr[pos]=='_')) ++pos;
    if(start==pos) return nullptr;
    return new Node(expr.substr(start,pos-start));
}
Node* term() {
    Node* n=factor();
    if(!n) return nullptr;
    while(pos<expr.size() && (expr[pos]=='*' || expr[pos]=='/')) {
        string op(1,expr[pos++]);
        Node* r=factor(); if(!r) return nullptr;
        n=new Node(op,n,r);
    }
    return n;
}
Node* expression() {
    Node* n=term();
    if(!n) return nullptr;
    while(pos<expr.size() && (expr[pos]=='+' || expr[pos]=='-')) {
        string op(1,expr[pos++]);
        Node* r=term(); if(!r) return nullptr;
        n=new Node(op,n,r);
    }
    return n;
}

string emit(Node* n, vector<array<string,4>>& quads,
            vector<array<string,3>>& triples) {
    if(!n->left && !n->right) return n->val;
    string a=emit(n->left,quads,triples);
    string b=emit(n->right,quads,triples);
    string t="t"+to_string(tempNo++);
    quads.push_back({n->val,a,b,t});
    triples.push_back({n->val,a,b});
    cout<<t<<" = "<<a<<" "<<n->val<<" "<<b<<"\n";
    return t;
}

int main() {
    string stmt;
    cout<<"Enter assignment (example x=a+b*c): ";
    getline(cin,stmt);
    size_t eq=stmt.find('=');
    if(eq==string::npos){cout<<"Invalid assignment\n";return 0;}

    string lhs=stmt.substr(0,eq);
    expr=stmt.substr(eq+1);
    auto removeSpaces=[](string &s){
        s.erase(remove_if(s.begin(),s.end(),
            [](unsigned char c){return isspace(c);}),s.end());
    };
    removeSpaces(lhs); removeSpaces(expr);
    if(lhs.empty() || expr.empty()){cout<<"Invalid assignment\n";return 0;}

    Node* root=expression();
    if(!root || pos!=expr.size()){cout<<"Invalid expression\n";return 0;}

    vector<array<string,4>> quads;
    vector<array<string,3>> triples;
    cout<<"\nThree Address Code:\n";
    string result=emit(root,quads,triples);
    cout<<lhs<<" = "<<result<<"\n";

    cout<<"\nQUADRUPLES\nOp\tArg1\tArg2\tResult\n";
    for(auto &q:quads)
        cout<<q[0]<<'\t'<<q[1]<<'\t'<<q[2]<<'\t'<<q[3]<<'\n';
    cout<<lhs<<" = "<<result<<"  (assignment)\n";

    cout<<"\nTRIPLES\nIndex\tOp\tArg1\tArg2\n";
    for(int i=0;i<(int)triples.size();++i) {
        auto show=[&](string x){
            for(int j=0;j<i;++j)
                if(x=="t"+to_string(j+1)) return "("+to_string(j)+")";
            return x;
        };
        cout<<i<<'\t'<<triples[i][0]<<'\t'
            <<show(triples[i][1])<<'\t'<<show(triples[i][2])<<'\n';
    }
    cout<<"Assignment: "<<lhs<<" = ("<< (triples.empty() ? result : to_string(triples.size()-1)) <<")\n";
}
