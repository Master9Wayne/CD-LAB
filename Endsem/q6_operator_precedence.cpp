#include <bits/stdc++.h>
using namespace std;

// Step 1: Define operator precedence
// ^ has highest precedence
// * and / have medium precedence
// + and - have lowest precedence

map<string, int> P = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"^", 3}
};

// Valid productions / handles
set<string> prods = {
    "id", "(E)",
    "E+E", "E-E", "E*E", "E/E", "E^E"
};


// Step 2: Find precedence relation between two terminals
// '<' = Shift
// '=' = Shift (equal precedence relation)
// '>' = Reduce
// 'A' = Accept
// 'x' = Error

char rel(const string& a, const string& b) {

    // End marker
    if (a == "$") {
        return (b == "$") ? 'A' : '<';
    }

    if (b == "$") {
        return (a == "(") ? 'x' : '>';
    }

    // Opening parenthesis
    if (a == "(") {
        return (b == ")") ? '=' : '<';
    }

    // id and closing parenthesis
    if (a == "id" || a == ")") {

        if (b == "id" || b == "(")
            return 'x';

        return '>';
    }

    // Lookahead is id or opening parenthesis
    if (b == "id" || b == "(") {
        return '<';
    }

    // Lookahead is closing parenthesis
    if (b == ")") {
        return '>';
    }

    // Compare operator precedence
    if (P[a] != P[b]) {
        return (P[a] > P[b]) ? '>' : '<';
    }

    // Equal precedence:
    // ^ is right associative, so shift
    // +, -, *, / are left associative, so reduce

    return (a == "^") ? '<' : '>';
}


// Step 3: Find topmost terminal in stack
// Ignore non-terminal E.

string topTerminal(const vector<string>& st) {

    for (int i = (int)st.size() - 1; i >= 0; i--) {

        if (st[i] != "E") {
            return st[i];
        }
    }

    return "$";
}


// Step 4: Convert vector into printable string

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


    // Step 5: Tokenization
    // id is treated as one token.
    // Operators and parentheses are separate tokens.

    vector<string> in;

    for (size_t i = 0; i < s.size(); i++) {

        if (isspace(s[i]))
            continue;

        string t;

        if (s.compare(i, 2, "id") == 0) {
            t = "id";
        }
        else {
            t = string(1, s[i]);
        }

        // Check whether token is valid
        if (t != "id" &&
            !P.count(t) &&
            t != "(" &&
            t != ")") {

            cout << "ERROR: invalid symbol '" << t << "'\n";
            return 1;
        }

        in.push_back(t);

        i += t.size() - 1;
    }

    // Append end marker
    in.push_back("$");


    // Step 6: Initialize stack
    vector<string> st = {"$"};

    size_t ip = 0;


    cout << left
         << setw(25) << "STACK"
         << setw(25) << "INPUT"
         << "ACTION\n";

    cout << string(70, '-') << '\n';


    // Step 7: Parsing loop

    while (true) {

        // Topmost terminal in stack
        string a = topTerminal(st);

        // Current input symbol
        string b = in[ip];

        // Find precedence relation
        char r = rel(a, b);


        cout << setw(25) << join(st)
             << setw(25) << join(in, ip);


        // Case 1: Accept or reject
        if (r == 'A') {

            // Accept only if stack contains exactly $ E
            bool ok = (st.size() == 2 && st[1] == "E");

            cout << (ok
                     ? "ACCEPT\n"
                     : "ERROR: String rejected\n");

            return !ok;
        }


        // Case 2: Invalid precedence relation
        if (r == 'x') {

            cout << "ERROR: String rejected\n";
            return 1;
        }


        // Case 3: Shift operation
        if (r == '<' || r == '=') {

            cout << a
                 << (r == '<' ? " < " : " = ")
                 << b
                 << "  Shift " << b << '\n';

            // Push input symbol onto stack
            st.push_back(b);

            // Move to next input symbol
            ip++;

            continue;
        }


        // Case 4: Reduce operation (r == '>')

        // Extract handle from stack
        // Handle is the substring that matches a production RHS.

        string last, handle;

        do {

            // Include E if present at stack top
            if (st.back() == "E") {

                handle = "E" + handle;
                st.pop_back();
            }

            // Get topmost terminal
            last = topTerminal(st);

            // Add terminal to handle
            handle = last + handle;

            st.pop_back();


            // Include preceding E if present
            if (st.back() == "E") {

                handle = "E" + handle;
                st.pop_back();
            }

        } while (rel(topTerminal(st), last) != '<');


        // Check whether extracted handle is a valid production
        if (!prods.count(handle)) {

            cout << "ERROR: invalid handle '"
                 << handle << "'\n";

            return 1;
        }


        // Perform reduction
        cout << a << " > " << b
             << "  Reduce E -> " << handle << '\n';

        // Replace handle with non-terminal E
        st.push_back("E");
    }

    return 0;
}
