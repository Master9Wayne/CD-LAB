#include <bits/stdc++.h>
using namespace std;

// M[non-terminal][terminal] = production body
// Symbols in production bodies are separated by spaces.
// '#' represents epsilon (empty production).

map<string, map<string, string>> M = {
    {"E", {
        {"id", "T E'"},
        {"(", "T E'"}
    }},

    {"E'", {
        {"+", "+ T E'"},
        {")", "#"},
        {"$", "#"}
    }},

    {"T", {
        {"id", "F T'"},
        {"(", "F T'"}
    }},

    {"T'", {
        {"+", "#"},
        {"*", "* F T'"},
        {")", "#"},
        {"$", "#"}
    }},

    {"F", {
        {"id", "id"},
        {"(", "( E )"}
    }}
};

// Converts stack/input vector into a printable string.
string join(const vector<string>& v, size_t from = 0) {
    string s;

    for (size_t i = from; i < v.size(); i++) {
        s += v[i] + " ";
    }

    return s;
}

int main() {

    string s;

    cout << "Enter input (e.g. id+id*id): ";
    getline(cin, s);

    // Tokenization
    // "id" is treated as one token.
    // Every other character is treated as a separate token.

    vector<string> in;

    for (size_t i = 0; i < s.size(); i++) {

        if (isspace(s[i]))
            continue;

        if (s.compare(i, 2, "id") == 0) {
            in.push_back("id");
            i++;
        }
        else {
            in.push_back(string(1, s[i]));
        }
    }

    // Append end marker
    in.push_back("$");

    // Initialize stack
    // Stack top is at the back.
    vector<string> st = {"$", "E"};

    size_t ip = 0;

    cout << left
         << setw(25) << "STACK"
         << setw(25) << "INPUT"
         << "ACTION\n";

    cout << string(70, '-') << '\n';

    // Predictive parsing loop
    while (true) {

        string X = st.back();
        string a = in[ip];

        cout << setw(25) << join(st)
             << setw(25) << join(in, ip);

        // Case 1: Stack top matches input symbol
        if (X == a) {

            // Both are end markers
            if (X == "$") {
                cout << "ACCEPT\n";
                return 0;
            }

            cout << "Match " << a << '\n';

            st.pop_back();
            ip++;
        }

        // Case 2: Error - no production in parsing table
        else if (!M.count(X) || !M[X].count(a)) {

            cout << "ERROR: String rejected\n";
            return 1;
        }

        // Case 3: Expand non-terminal using parsing table
        else {

            string body = M[X][a];

            cout << X << " -> " << body << '\n';

            st.pop_back();

            // Epsilon production: push nothing
            if (body != "#") {

                istringstream ss(body);

                vector<string> sy;
                string t;

                while (ss >> t) {
                    sy.push_back(t);
                }

                // Push production symbols in reverse order
                // so that the leftmost symbol is on top.
                st.insert(
                    st.end(),
                    sy.rbegin(),
                    sy.rend()
                );
            }
        }
    }

    return 0;
}
