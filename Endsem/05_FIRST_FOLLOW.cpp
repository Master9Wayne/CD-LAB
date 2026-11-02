#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> FIRST, FOLLOW;

bool isNT(char c) { return c >= 'A' && c <= 'Z'; }

set<char> firstString(const string& s) {
    set<char> result;
    bool allNullable = true;
    for (char c : s) {
        if (!isNT(c)) {
            if (c != '#') result.insert(c);
            if (c != '#') allNullable = false;
            if (c != '#') break;
        } else {
            for (char x : FIRST[c]) if (x != '#') result.insert(x);
            if (!FIRST[c].count('#')) { allNullable = false; break; }
        }
    }
    if (allNullable) result.insert('#');
    return result;
}

int main() {
    int n; char start;
    cout << "Number of productions: ";
    cin >> n;
    cout << "Start symbol: ";
    cin >> start;
    cout << "Enter productions like E=TR|# (one per line):\n";
    for (int i=0;i<n;++i) {
        string line; cin >> line;
        char lhs=line[0];
        string rhs=line.substr(2);
        string alt;
        stringstream ss(rhs);
        while (getline(ss,alt,'|')) grammar[lhs].push_back(alt);
    }

    bool changed=true;
    while (changed) {
        changed=false;
        for (auto &[A, alternatives] : grammar)
            for (const string& rhs : alternatives)
                for (char x : firstString(rhs))
                    if (FIRST[A].insert(x).second) changed=true;
    }

    FOLLOW[start].insert('$');
    changed=true;
    while (changed) {
        changed=false;
        for (auto &[A, alternatives] : grammar) {
            for (const string& rhs : alternatives) {
                for (int i=0;i<(int)rhs.size();++i) {
                    char B=rhs[i];
                    if (!isNT(B)) continue;
                    string beta=rhs.substr(i+1);
                    set<char> f=firstString(beta);
                    for (char x:f)
                        if (x!='#' && FOLLOW[B].insert(x).second) changed=true;
                    if (beta.empty() || f.count('#'))
                        for (char x:FOLLOW[A])
                            if (FOLLOW[B].insert(x).second) changed=true;
                }
            }
        }
    }

    cout << "\nFIRST:\n";
    for (auto &[A,s]:FIRST) {
        cout << "FIRST(" << A << ") = { ";
        for(char x:s) cout << x << ' ';
        cout << "}\n";
    }
    cout << "\nFOLLOW:\n";
    for (auto &[A,s]:FOLLOW) {
        cout << "FOLLOW(" << A << ") = { ";
        for(char x:s) cout << x << ' ';
        cout << "}\n";
    }
}
