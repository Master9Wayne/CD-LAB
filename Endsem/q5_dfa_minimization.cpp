#include <bits/stdc++.h>
using namespace std;

int main() {

    int n, k, s, f;

    // Step 1: Read DFA details
    cout << "No. of states (0..n-1) and alphabet size: ";
    cin >> n >> k;

    vector<char> sym(k);

    cout << "Symbols: ";
    for (char &c : sym)
        cin >> c;

    // Transition table
    // d[i][j] = next state from state i on symbol j
    vector<vector<int>> d(n, vector<int>(k));

    cout << "Transition table (row per state, column per symbol):\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cin >> d[i][j];
        }
    }

    // Read start state
    cout << "Start state: ";
    cin >> s;

    // Read final states
    cout << "No. of final states, then the states: ";
    cin >> f;

    vector<int> fin(n, 0);

    while (f--) {
        int x;
        cin >> x;
        fin[x] = 1;
    }


    // Step 2: Find reachable states using BFS
    // Unreachable states will have group number -1.

    vector<int> g(n, -1);
    vector<int> seen(n, 0);

    queue<int> q;

    q.push(s);
    seen[s] = 1;

    while (!q.empty()) {

        int u = q.front();
        q.pop();

        for (int v : d[u]) {

            if (!seen[v]) {
                seen[v] = 1;
                q.push(v);
            }
        }
    }

    // Initial partition:
    // Group 0 = Non-final states
    // Group 1 = Final states

    for (int i = 0; i < n; i++) {
        if (seen[i]) {
            g[i] = fin[i];
        }
    }


    // Step 3: Function to create partitions
    // map: group ID -> states belonging to that group

    auto blocks = [&](const vector<int>& v) {

        map<int, vector<int>> m;

        for (int i = 0; i < n; i++) {

            if (v[i] >= 0) {
                m[v[i]].push_back(i);
            }
        }

        return m;
    };


    // Step 4: Function to print a partition
    // Example: {0,3}

    auto name = [](const vector<int>& mem) {

        string r = "{";

        for (int x : mem) {
            r += to_string(x) + ",";
        }

        r.back() = '}';

        return r;
    };


    auto show = [&](const string& t,
                    const map<int, vector<int>>& m) {

        cout << t;

        for (auto& [id, mem] : m) {
            cout << " " << name(mem);
        }

        cout << '\n';
    };


    // Step 5: Initial partition
    auto P = blocks(g);

    cout << "\n";
    show("P0:", P);


    // Step 6: Refine partitions until no further splitting occurs

    for (int round = 1; ; round++) {

        // Signature of a state:
        // [current group, group reached on symbol 1, ..., group reached on symbol k]

        map<vector<int>, int> id;

        vector<int> ng(n, -1);

        for (int i = 0; i < n; i++) {

            if (g[i] >= 0) {

                vector<int> sig;

                // Current group
                sig.push_back(g[i]);

                // Groups reached on each input symbol
                for (int c = 0; c < k; c++) {
                    sig.push_back(g[d[i][c]]);
                }

                // States with identical signatures belong to same new group
                ng[i] = id.emplace(sig, id.size()).first->second;
            }
        }

        // If number of groups remains unchanged,
        // no more splitting is possible.

        bool stable = (id.size() == P.size());

        // Update partitions
        g = ng;
        P = blocks(g);

        // Display current partition
        show("P" + to_string(round) + ":", P);

        if (stable)
            break;
    }


    // Step 7: Print the minimised DFA

    cout << "\nMinimised DFA (" << P.size() << " states)\n";

    cout << setw(14) << "State";

    for (char c : sym) {
        cout << setw(10) << c;
    }

    cout << '\n';


    // Each partition represents one state in the minimised DFA.

    for (auto& [id, mem] : P) {

        // First member represents the entire partition
        int r = mem[0];

        // Mark start state with ->
        // Mark final states with *
        string state = "";

        if (g[s] == id)
            state += "->";

        if (fin[r])
            state += "*";

        state += name(mem);

        cout << setw(14) << state;

        // Print transitions between minimised states
        for (int c = 0; c < k; c++) {

            int next = d[r][c];

            int nextGroup = g[next];

            cout << setw(10) << name(P[nextGroup]);
        }

        cout << '\n';
    }

    return 0;
}
