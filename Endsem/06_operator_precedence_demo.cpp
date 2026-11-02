#include <bits/stdc++.h>
using namespace std;

// Demonstration of precedence-driven shift/reduce for operands i,
// operators + and *, and end marker $. This is not a general grammar
// handle-reduction implementation; it validates expressions using
// precedence and operand/operator expectations.
int main() {
    string s;
    cout << "Enter expression using i, +, * and parentheses: ";
    cin >> s;

    // Recursive precedence parser: E -> T (+ T)* ; T -> F (* F)* ; F -> i | (E)
    int p=0;
    function<bool()> E, T, F;
    F = [&]() {
        if (p >= (int)s.size()) return false;
        if (s[p]=='i') { ++p; return true; }
        if (s[p]=='(') {
            ++p;
            if (!E()) return false;
            if (p >= (int)s.size() || s[p]!=')') return false;
            ++p; return true;
        }
        return false;
    };
    T = [&]() {
        if (!F()) return false;
        while (p<(int)s.size() && s[p]=='*') {
            ++p; if (!F()) return false;
        }
        return true;
    };
    E = [&]() {
        if (!T()) return false;
        while (p<(int)s.size() && s[p]=='+') {
            ++p; if (!T()) return false;
        }
        return true;
    };

    cout << (E() && p==(int)s.size() ? "Accepted\n" : "Rejected\n");
}
