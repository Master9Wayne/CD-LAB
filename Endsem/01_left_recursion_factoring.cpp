
#include <bits/stdc++.h>
using namespace std;

void removeLeftRecursion() {
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

        if (p.rfind(nt, 0) == 0)
            alpha.push_back(p.substr(nt.size()));
        else
            beta.push_back(p);
    }

    if (alpha.empty()) {
        cout << "No immediate left recursion found.\n";
        return;
    }

    if (beta.empty()) {
        cout << "Cannot apply transformation: no non-left-recursive alternative.\n";
        return;
    }

    cout << "\nAfter removing left recursion:\n";

    cout << nt << " -> ";

    for (int i = 0; i < beta.size(); i++) {
        if (i) cout << " | ";
        cout << beta[i] << nt << "'";
    }

    cout << "\n" << nt << "' -> ";

    for (int i = 0; i < alpha.size(); i++) {
        if (i) cout << " | ";

        if (alpha[i].empty())
            cout << "epsilon";
        else
            cout << alpha[i] << nt << "'";
    }

    cout << " | epsilon\n";
}

void leftFactoring() {
    string nt;
    int n;

    cout << "Enter non-terminal: ";
    cin >> nt;

    cout << "Enter number of alternatives: ";
    cin >> n;

    vector<string> p(n);

    cout << "Enter productions (without spaces):\n";

    for (auto &x : p)
        cin >> x;

    string prefix = p[0];

    for (int i = 1; i < n; i++) {
        int j = 0;

        while (j < prefix.size() &&
               j < p[i].size() &&
               prefix[j] == p[i][j]) {
            j++;
        }

        prefix.resize(j);
    }

    if (prefix.empty()) {
        cout << "No common prefix found.\n";
        return;
    }

    cout << "\nAfter left factoring:\n";

    cout << nt << " -> " << prefix << nt << "'\n";

    cout << nt << "' -> ";

    for (int i = 0; i < n; i++) {
        if (i) cout << " | ";

        string suffix = p[i].substr(prefix.size());

        cout << (suffix.empty() ? "epsilon" : suffix);
    }

    cout << "\n";
}

int main() {
    int choice;

    cout << "===== COMPILER DESIGN =====\n";
    cout << "1. Remove Left Recursion\n";
    cout << "2. Left Factoring\n";
    cout << "Enter choice: ";

    cin >> choice;

    switch (choice) {
        case 1:
            removeLeftRecursion();
            break;

        case 2:
            leftFactoring();
            break;

        default:
            cout << "Invalid choice.\n";
    }

    return 0;
}

