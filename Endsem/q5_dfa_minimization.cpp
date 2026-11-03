// Q5: DFA minimisation by the partition (Moore) method.
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k, s, f;
    cout << "No. of states (0..n-1) and alphabet size: "; cin >> n >> k;
    vector<char> sym(k);
    cout << "Symbols: "; for (char& c : sym) cin >> c;
    vector<vector<int>> d(n, vector<int>(k));
    cout << "Transition table (row per state, column per symbol):\n";
    for (auto& row : d) for (int& x : row) cin >> x;
    cout << "Start state: "; cin >> s;
    cout << "No. of final states, then the states: "; cin >> f;
    vector<int> fin(n);
    while (f--) { int x; cin >> x; fin[x] = 1; }

    // Step 1: drop unreachable states (g[i] = -1), initial partition = {non-final}, {final}
    vector<int> g(n, -1), seen(n);
    queue<int> q; q.push(s); seen[s] = 1;
    while (!q.empty()) { int u = q.front(); q.pop(); for (int v : d[u]) if (!seen[v]) seen[v] = 1, q.push(v); }
    for (int i = 0; i < n; i++) if (seen[i]) g[i] = fin[i];

    auto blocks = [&](const vector<int>& v) {               // block id -> member states
        map<int, vector<int>> m;
        for (int i = 0; i < n; i++) if (v[i] >= 0) m[v[i]].push_back(i);
        return m;
    };
    auto name = [](const vector<int>& mem) {                // {0,3}
        string r = "{"; for (int x : mem) r += to_string(x) + ","; r.back() = '}'; return r;
    };
    auto show = [&](const string& t, const map<int, vector<int>>& m) {
        cout << t; for (auto& [id, mem] : m) cout << ' ' << name(mem); cout << '\n';
    };

    // Step 2: refine until no block splits. Two states stay together iff they share a block
    // and every symbol sends them to the same block.
    auto P = blocks(g); cout << '\n'; show("P0:", P);
    for (int round = 1;; round++) {
        map<vector<int>, int> id; vector<int> ng(n, -1);
        for (int i = 0; i < n; i++) if (g[i] >= 0) {
            vector<int> sig = {g[i]};
            for (int c = 0; c < k; c++) sig.push_back(g[d[i][c]]);
            ng[i] = id.emplace(sig, id.size()).first->second;
        }
        bool stable = id.size() == P.size();                // refinement only splits, so same count => same partition
        g = ng; P = blocks(g);
        show("P" + to_string(round) + ":", P);
        if (stable) break;
    }

    // Step 3: print minimised DFA  (-> start, * final)
    cout << "\nMinimised DFA (" << P.size() << " states)\n" << setw(14) << "State";
    for (char c : sym) cout << setw(10) << c;
    cout << '\n';
    for (auto& [id, mem] : P) {
        int r = mem[0];                                     // any member represents the block
        cout << setw(14) << string(g[s] == id ? "->" : "") + (fin[r] ? "*" : "") + name(mem);
        for (int c = 0; c < k; c++) cout << setw(10) << name(P[g[d[r][c]]]);
        cout << '\n';
    }
}
