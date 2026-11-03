#include <bits/stdc++.h>
using namespace std;


// Function 1: Create partitions from group assignments
// Maps: group ID -> states belonging to that group

map<int, vector<int>> blocks(vector<int>& v, int n) {

    map<int, vector<int>> m;

    for (int i = 0; i < n; i++) {

        if (v[i] >= 0) {
            m[v[i]].push_back(i);
        }
    }

    return m;
}


// Function 2: Convert a group of states into a string
// Example: {0, 2, 3}

string name(vector<int>& mem) {

    string r = "{";

    for (int x : mem) {
        r += to_string(x) + ",";
    }

    r.back() = '}';

    return r;
}


// Function 3: Display partitions

void show(string t, map<int, vector<int>>& m) {

    cout << t;

    for (auto& p : m) {
        cout << " " << name(p.second);
    }

    cout << '\n';
}


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


    // Step 3: Initial partition
    // Group 0 = Non-final states
    // Group 1 = Final states

    for (int i = 0; i < n; i++) {

        if (seen[i]) {
            g[i] = fin[i];
        }
    }


    // Step 4: Create and display initial partitions

    map<int, vector<int>> P = blocks(g, n);

    cout << "\n";

    show("P0:", P);


    // Step 5: Refine partitions until stable

    for (int round = 1; ; round++) {

        // Signature:
        // Current group + destination groups for each symbol

        map<vector<int>, int> id;

        vector<int> ng(n, -1);

        for (int i = 0; i < n; i++) {

            if (g[i] >= 0) {

                vector<int> sig;

                // Current group
                sig.push_back(g[i]);

                // Destination groups
                for (int c = 0; c < k; c++) {
                    sig.push_back(g[d[i][c]]);
                }

                // Assign new group based on signature
                ng[i] = id.emplace(sig, id.size()).first->second;
            }
        }


        // Check whether partition is stable

        bool stable = (id.size() == P.size());


        // Update partitions

        g = ng;

        P = blocks(g, n);


        // Display new partition

        show("P" + to_string(round) + ":", P);


        if (stable)
            break;
    }


    // Step 6: Print minimised DFA

    cout << "\nMinimised DFA (" << P.size() << " states)\n";

    cout << setw(14) << "State";

    for (char c : sym) {
        cout << setw(10) << c;
    }

    cout << '\n';


    // Each partition represents one state

    for (auto& p : P) {

        int id = p.first;

        vector<int> mem = p.second;

        // First member represents the partition
        int r = mem[0];


        // Mark start state and final states

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
