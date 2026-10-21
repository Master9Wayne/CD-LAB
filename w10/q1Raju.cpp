#include <iostream>
#include <stack>
#include <string>
#include <iomanip>
using namespace std;

// ACTION columns: id, +, *, (, ), $
string action[12][6] = {
    {"s5", "",   "",   "s4", "",    ""},
    {"",   "s6", "",   "",   "",    "acc"},
    {"",   "r2", "s7", "",   "r2",  "r2"},
    {"",   "r4", "r4", "",   "r4",  "r4"},
    {"s5", "",   "",   "s4", "",    ""},
    {"",   "r6", "r6", "",   "r6",  "r6"},
    {"s5", "",   "",   "s4", "",    ""},
    {"s5", "",   "",   "s4", "",    ""},
    {"",   "s6", "",   "",   "s11", ""},
    {"",   "r1", "s7", "",   "r1",  "r1"},
    {"",   "r3", "r3", "",   "r3",  "r3"},
    {"",   "r5", "r5", "",   "r5",  "r5"}
};

// GOTO columns: E, T, F
int goTo[12][3] = {
    { 1,  2,  3},  // State 0
    {-1, -1, -1},  // State 1
    {-1, -1, -1},  // State 2
    {-1, -1, -1},  // State 3
    { 8,  2,  3},  // State 4
    {-1, -1, -1},  // State 5
    {-1,  9,  3},  // State 6: GOTO(6,F) = 3
    {-1, -1, 10},  // State 7
    {-1, -1, -1},  // State 8
    {-1, -1, -1},  // State 9
    {-1, -1, -1},  // State 10
    {-1, -1, -1}   // State 11
};

// Productions:
// r1: E -> E + T
// r2: E -> T
// r3: T -> T * F
// r4: T -> F
// r5: F -> ( E )
// r6: F -> id

string lhs[] = {"", "E", "E", "T", "T", "F", "F"};
string rhs[] = {"", "E+T", "T", "T*F", "F", "(E)", "id"};
int len[]    = { 0,  3,    1,   3,     1,   3,    1};

int col(string input) {
    if (input.substr(0, 2) == "id") return 0;
    if (input[0] == '+') return 1;
    if (input[0] == '*') return 2;
    if (input[0] == '(') return 3;
    if (input[0] == ')') return 4;
    if (input[0] == '$') return 5;
    return -1;
}

int ntcol(string s) {
    if (s == "E") return 0;
    if (s == "T") return 1;
    return 2; // F
}

int main() {
    string input;

    cout << "Enter input (e.g. id+id*id): ";
    cin >> input;
    input += '$';

    stack<int> states;
    stack<string> symbols;
    states.push(0);

    cout << '\n' << left << setw(35) << "STACK"
         << setw(25) << "INPUT" << "ACTION\n";
    cout << string(85, '-') << '\n';

    while (true) {
        // Display stack from bottom to top
        int s[100], n = 0;
        string y[100];
        int m = 0;

        stack<int> tempS = states;
        stack<string> tempY = symbols;

        while (!tempS.empty()) {
            s[n++] = tempS.top();
            tempS.pop();
        }

        while (!tempY.empty()) {
            y[m++] = tempY.top();
            tempY.pop();
        }

        string stackStr = "";

        for (int i = n - 1; i >= 0; i--) {
            stackStr += to_string(s[i]) + " ";
            if (i > 0)
                stackStr += y[i - 1] + " ";
        }

        cout << left << setw(35) << stackStr
             << setw(25) << input;

        int state = states.top();
        int c = col(input);

        if (c == -1 || action[state][c] == "") {
            cout << "ERROR: String rejected\n";
            break;
        }

        string a = action[state][c];

        if (a == "acc") {
            cout << "ACCEPT\n";
            break;
        }

        if (a[0] == 's') {
            int next = stoi(a.substr(1));
            cout << "Shift " << next << '\n';

            if (c == 0) {
                symbols.push("id");
                states.push(next);
                input.erase(0, 2);
            } else {
                symbols.push(string(1, input[0]));
                states.push(next);
                input.erase(0, 1);
            }
        }
        else if (a[0] == 'r') {
            int r = stoi(a.substr(1));

            cout << "Reduce by " << lhs[r]
                 << " -> " << rhs[r] << '\n';

            for (int i = 0; i < len[r]; i++) {
                if (symbols.empty() || states.size() <= 1) {
                    cout << "ERROR: Invalid stack\n";
                    return 1;
                }

                symbols.pop();
                states.pop();
            }

            string A = lhs[r];
            int next = goTo[states.top()][ntcol(A)];

            if (next == -1) {
                cout << "ERROR: Invalid GOTO\n";
                return 1;
            }

            symbols.push(A);
            states.push(next);
        }
    }

    return 0;
}
