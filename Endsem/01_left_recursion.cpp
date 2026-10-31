#include <bits/stdc++.h>
using namespace std;

// Input: one-character nonterminal, then productions without spaces.
// Example: A, 3, Aa, Ab, c
int main() {
    string nt;
    int n;
    cout << "Enter non-terminal: ";
    cin >> nt;
    cout << "Enter number of productions: ";
    cin >> n;

    vector<string> alpha, beta;
    for (int i = 0; i < n; ++i) {
        string p;
        cin >> p;
        if (p.rfind(nt, 0) == 0) alpha.push_back(p.substr(nt.size()));
        else beta.push_back(p);
    }

    if (alpha.empty()) {
        cout << "No immediate left recursion found.\n";
        return 0;
    }
    if (beta.empty()) {
        cout << "Cannot apply this transformation: no non-left-recursive alternative.\n";
        return 0;
    }

    cout << nt << " -> ";
    for (int i = 0; i < (int)beta.size(); ++i) {
        if (i) cout << " | ";
        cout << beta[i] << nt << "'";
    }
    cout << "\n" << nt << "' -> ";
    for (int i = 0; i < (int)alpha.size(); ++i) {
        if (i) cout << " | ";
        if (alpha[i].empty()) cout << "epsilon";
        else cout << alpha[i] << nt << "'";
    }
    cout << " | epsilon\n";
}
