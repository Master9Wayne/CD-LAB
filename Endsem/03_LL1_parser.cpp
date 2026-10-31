#include <bits/stdc++.h>
using namespace std;

// Example grammar: E -> iT ; T -> +iT | epsilon
// For this grammar: M[E,i]=iT; M[T,+]=+iT; M[T,$]=epsilon.
int main() {
    map<pair<char,char>, string> table;
    table[{'E','i'}] = "iT";
    table[{'T','+'}] = "+iT";
    table[{'T','$'}] = "#"; // # denotes epsilon

    string input;
    cout << "Enter input (use i for identifier): ";
    cin >> input;
    input += '$';

    stack<char> st;
    st.push('$');
    st.push('E');
    int pos = 0;

    while (!st.empty()) {
        char top = st.top();
        char look = input[pos];

        if (top == look) {
            st.pop();
            ++pos;
        } else if (!isupper((unsigned char)top)) {
            cout << "Rejected\n";
            return 0;
        } else {
            auto it = table.find({top, look});
            if (it == table.end()) {
                cout << "Rejected\n";
                return 0;
            }
            st.pop();
            const string rhs = it->second;
            if (rhs != "#")
                for (int j = (int)rhs.size() - 1; j >= 0; --j)
                    st.push(rhs[j]);
        }
    }

    cout << (pos == (int)input.size() ? "Accepted\n" : "Rejected\n");
}
