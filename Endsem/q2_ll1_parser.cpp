// Q2: Table-driven LL(1) predictive parser (hard-coded table from the image). '#' = epsilon.
#include <bits/stdc++.h>
using namespace std;

// M[non-terminal][terminal] = production body (symbols separated by spaces)
map<string, map<string, string>> M = {
    {"E",  {{"id", "T E'"},  {"(", "T E'"}}},
    {"E'", {{"+", "+ T E'"}, {")", "#"}, {"$", "#"}}},
    {"T",  {{"id", "F T'"},  {"(", "F T'"}}},
    {"T'", {{"+", "#"}, {"*", "* F T'"}, {")", "#"}, {"$", "#"}}},
    {"F",  {{"id", "id"},    {"(", "( E )"}}}};

string join(const vector<string>& v, size_t from = 0) {
    string s;
    for (size_t i = from; i < v.size(); i++) s += v[i] + " ";
    return s;
}

int main() {
    string s; cout << "Enter input (e.g. id+id*id): "; getline(cin, s);

    vector<string> in;                      // tokens: "id" is one token, any other char is its own token
    for (size_t i = 0; i < s.size(); i++) {
        if (isspace(s[i])) continue;
        if (s.compare(i, 2, "id") == 0) { in.push_back("id"); i++; }
        else in.push_back(string(1, s[i]));
    }
    in.push_back("$");

    vector<string> st = {"$", "E"};         // stack, top = back()
    size_t ip = 0;
    cout << left << setw(25) << "STACK" << setw(25) << "INPUT" << "ACTION\n" << string(70, '-') << '\n';
    while (true) {
        string X = st.back(), a = in[ip];
        cout << setw(25) << join(st) << setw(25) << join(in, ip);
        if (X == a) {                                       // terminal on top matches lookahead
            if (X == "$") { cout << "ACCEPT\n"; return 0; }
            cout << "Match " << a << '\n'; st.pop_back(); ip++;
        } else if (!M.count(X) || !M[X].count(a)) {         // terminal mismatch / blank table entry
            cout << "ERROR: String rejected\n"; return 1;
        } else {                                            // expand X by M[X][a]
            string body = M[X][a];
            cout << X << " -> " << body << '\n'; st.pop_back();
            if (body != "#") {                              // push body in reverse so first symbol is on top
                istringstream ss(body); vector<string> sy; string t;
                while (ss >> t) sy.push_back(t);
                st.insert(st.end(), sy.rbegin(), sy.rend());
            }
        }
    }
}
