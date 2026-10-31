#include <bits/stdc++.h>
using namespace std;

// Factors one group of alternatives that share a common prefix.
int main() {
    string nt;
    int n;
    cout << "Enter non-terminal: ";
    cin >> nt;
    cout << "Enter number of alternatives: ";
    cin >> n;

    vector<string> p(n);
    for (auto &x : p) cin >> x;

    string prefix = p[0];
    for (int i = 1; i < n; ++i) {
        int j = 0;
        while (j < (int)prefix.size() && j < (int)p[i].size()
               && prefix[j] == p[i][j]) ++j;
        prefix.resize(j);
    }

    if (prefix.empty()) {
        cout << "No common prefix found.\n";
        return 0;
    }

    cout << nt << " -> " << prefix << nt << "'\n";
    cout << nt << "' -> ";
    for (int i = 0; i < n; ++i) {
        if (i) cout << " | ";
        string suffix = p[i].substr(prefix.size());
        cout << (suffix.empty() ? "epsilon" : suffix);
    }
    cout << '\n';
}
