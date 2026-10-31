
#include <bits/stdc++.h>
using namespace std;

int main() {
    string nt;
    int n;

    cout << "Enter non-terminal: ";
    cin >> nt;

    cout << "Enter number of productions: ";
    cin >> n;

    vector<string> alpha, beta;

    cout << "Enter productions (without spaces):\n";

    for (int i = 0; i < n; i++) {
        string p;
        cin >> p;

        if (p.substr(0, nt.size()) == nt)
            alpha.push_back(p.substr(nt.size()));
        else
            beta.push_back(p);
    }

    if (alpha.empty()) {
        cout << "No left recursion found.\n";
        return 0;
    }

    cout << "\nAfter removing left recursion:\n";

    cout << nt << " -> ";

    for (int i = 0; i < beta.size(); i++) {
        cout << beta[i] << nt << "'";
        if (i != beta.size() - 1) cout << " | ";
    }

    cout << "\n" << nt << "' -> ";

    for (int i = 0; i < alpha.size(); i++) {
        cout << alpha[i] << nt << "'";
        if (i != alpha.size() - 1) cout << " | ";
    }

    cout << " | epsilon\n";

    return 0;
}
