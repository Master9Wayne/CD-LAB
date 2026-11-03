// Q6: Operator-precedence parser (stack based) for  E -> E+E | E-E | E*E | E/E | E^E | (E) | id
// Precedence:  ^ (right assoc, highest) > * / > + -  (left assoc).   '<' shift, '=' shift, '>' reduce
#include <bits/stdc++.h>
using namespace std;

map<string, int> P = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}};
set<string> prods = {"id", "(E)", "E+E", "E-E", "E*E", "E/E", "E^E"};

// Relation between terminal a (top of stack) and b (lookahead): '<' '=' '>' , 'A' accept, 'x' error
char rel(const string& a, const string& b) {
    if (a == "$") return b == "$" ? 'A' : '<';
    if (b == "$") return a == "(" ? 'x' : '>';
    if (a == "(") return b == ")" ? '=' : '<';
    if (a == "id" || a == ")") return (b == "id" || b == "(") ? 'x' : '>';
    if (b == "id" || b == "(") return '<';                  // a is an operator
    if (b == ")") return '>';
    if (P[a] != P[b]) return P[a] > P[b] ? '>' : '<';       // operator vs operator
    return a == "^" ? '<' : '>';                            // equal precedence: right assoc shifts, left reduces
}

string topTerminal(const vector<string>& st) {              // topmost non-"E" symbol
    for (int i = st.size() - 1; i >= 0; i--) if (st[i] != "E") return st[i];
    return "$";
}

string join(const vector<string>& v, size_t from = 0) {
    string s;
    for (size_t i = from; i < v.size(); i++) s += v[i] + " ";
    return s;
}

int main() {
    string s; cout << "Enter input (e.g. id+id*id): "; getline(cin, s);

    vector<string> in;
    for (size_t i = 0; i < s.size(); i++) {
        if (isspace(s[i])) continue;
        string t = s.compare(i, 2, "id") == 0 ? "id" : string(1, s[i]);
        if (t != "id" && !P.count(t) && t != "(" && t != ")") { cout << "ERROR: invalid symbol '" << t << "'\n"; return 1; }
        in.push_back(t); i += t.size() - 1;
    }
    in.push_back("$");

    vector<string> st = {"$"};
    size_t ip = 0;
    cout << left << setw(25) << "STACK" << setw(25) << "INPUT" << "ACTION\n" << string(70, '-') << '\n';
    while (true) {
        string a = topTerminal(st), b = in[ip];
        char r = rel(a, b);
        cout << setw(25) << join(st) << setw(25) << join(in, ip);

        if (r == 'A') {                                     // $ vs $ : accept only if stack is exactly $ E
            bool ok = st.size() == 2 && st[1] == "E";
            cout << (ok ? "ACCEPT\n" : "ERROR: String rejected\n"); return !ok;
        }
        if (r == 'x') { cout << "ERROR: String rejected\n"; return 1; }
        if (r == '<' || r == '=') {                         // shift
            cout << a << (r == '<' ? " < " : " = ") << b << "  Shift " << b << '\n';
            st.push_back(b); ip++;
            continue;
        }
        // r == '>' : pop the handle = everything above the last '<' relation (with adjacent E's)
        string last, handle;
        do {
            if (st.back() == "E") handle = "E" + handle, st.pop_back();
            last = topTerminal(st);
            handle = last + handle; st.pop_back();
            if (st.back() == "E") handle = "E" + handle, st.pop_back();
        } while (rel(topTerminal(st), last) != '<');
        if (!prods.count(handle)) { cout << "ERROR: invalid handle '" << handle << "'\n"; return 1; }
        cout << a << " > " << b << "  Reduce E -> " << handle << '\n';
        st.push_back("E");
    }
}
