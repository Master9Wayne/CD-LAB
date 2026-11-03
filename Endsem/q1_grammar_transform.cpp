// Q1: Remove (direct + indirect) left recursion, then left-factor a grammar.
// Input : n, then n lines like  A->aBDc|Bd|#     ('#' = epsilon, uppercase = non-terminal, no spaces)
#include <bits/stdc++.h>
using namespace std;
using Seq = vector<string>;                 // one alternative = sequence of symbols (empty = epsilon)

map<string, vector<Seq>> G;                 // grammar: LHS -> alternatives
vector<string> order;                       // order of non-terminals (for output / indirect-LR removal)

string newName(string a) { while (G.count(a += "'")); return a; }   // A -> A', A'' ...

Seq tokenize(const string& s) {             // "aBA'c" -> {a, B, A', c}
    Seq r;
    for (char c : s) {
        if (c == '\'') r.back() += c;       // prime belongs to previous non-terminal
        else if (c != '#' && !isspace(c)) r.push_back(string(1, c));
    }
    return r;
}

void show(const string& title) {
    cout << title << '\n';
    for (auto& A : order) {
        cout << "  " << A << " -> ";
        for (size_t i = 0; i < G[A].size(); i++) {
            if (i) cout << " | ";
            if (G[A][i].empty()) cout << "#";
            for (auto& s : G[A][i]) cout << s;
        }
        cout << '\n';
    }
}

void removeLeftRecursion() {
    vector<string> base = order;            // fixed ordering A1..An
    for (size_t i = 0; i < base.size(); i++) {
        string Ai = base[i];
        for (size_t j = 0; j < i; j++) {    // indirect: replace Ai -> Aj γ by Ai -> δ γ for every Aj -> δ
            vector<Seq> nw;
            for (auto& p : G[Ai]) {
                if (!p.empty() && p[0] == base[j])
                    for (auto& d : G[base[j]]) { Seq q = d; q.insert(q.end(), p.begin() + 1, p.end()); nw.push_back(q); }
                else nw.push_back(p);
            }
            G[Ai] = nw;
        }
        vector<Seq> rec, non;               // direct: Ai -> Ai α | β
        for (auto& p : G[Ai]) (!p.empty() && p[0] == Ai ? rec : non).push_back(p);
        if (rec.empty()) continue;
        string Ap = newName(Ai); order.push_back(Ap);
        G[Ai].clear();
        for (auto b : non) { b.push_back(Ap); G[Ai].push_back(b); }               // Ai  -> β Ai'
        for (auto a : rec) { a.erase(a.begin()); a.push_back(Ap); G[Ap].push_back(a); }  // Ai' -> α Ai'
        G[Ap].push_back({});                                                       // Ai' -> ε
    }
}

void leftFactor() {
    for (bool changed = true; changed;) {
        changed = false;
        for (size_t k = 0; k < order.size() && !changed; k++) {
            string A = order[k];
            map<string, vector<Seq>> grp;   // group alternatives by first symbol
            for (auto& p : G[A]) if (!p.empty()) grp[p[0]].push_back(p);
            for (auto& [first, v] : grp) {
                if (v.size() < 2) continue;
                size_t l = 1;               // length of longest common prefix of the group
                while (all_of(v.begin(), v.end(), [&](const Seq& p) { return p.size() > l && p[l] == v[0][l]; })) l++;
                string Ap = newName(A); order.push_back(Ap);
                vector<Seq> keep;           // alternatives not in this group stay on A
                for (auto& p : G[A]) if (p.empty() || p[0] != first) keep.push_back(p);
                for (auto& p : v) G[Ap].push_back(Seq(p.begin() + l, p.end()));   // suffixes go to A'
                Seq pre(v[0].begin(), v[0].begin() + l); pre.push_back(Ap);       // A -> prefix A'
                keep.push_back(pre); G[A] = keep;
                changed = true; break;
            }
        }
    }
}

int main() {
    int n; string line;
    cout << "No. of productions, then productions (e.g. A->aBDc|Bd|#):\n"; cin >> n;
    while (n--) {
        cin >> line;
        string A = line.substr(0, line.find("->")), rhs = line.substr(line.find("->") + 2);
        if (!G.count(A)) order.push_back(A);
        stringstream ss(rhs); string alt;
        while (getline(ss, alt, '|')) G[A].push_back(tokenize(alt));
    }
    show("\nOriginal:");
    removeLeftRecursion(); show("\nAfter left-recursion removal:");
    leftFactor();          show("\nAfter left factoring:");
}
