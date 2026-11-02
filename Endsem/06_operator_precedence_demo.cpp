
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;

    cout << "Enter number of terminals: ";
    cin >> n;

    vector<char> terminals(n);

    cout << "Enter terminals (including $): ";
    for (int i = 0; i < n; i++)
        cin >> terminals[i];

    // Precedence relation table
    map<pair<char, char>, char> table;

    cout << "\nEnter precedence table:\n";
    cout << "Use <, =, > or - (no relation)\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char relation;
            cout << terminals[i] << " vs "
                 << terminals[j] << ": ";
            cin >> relation;

            if (relation != '-')
                table[{terminals[i], terminals[j]}] = relation;
        }
    }

    string input;
    cout << "\nEnter input string: ";
    cin >> input;

    input += '$';

    stack<char> st;
    st.push('$');

    int pos = 0;

    // Find the topmost terminal in the stack
    auto topTerminal = [&]() {
        stack<char> temp = st;

        while (!temp.empty()) {
            char c = temp.top();

            if (find(terminals.begin(), terminals.end(), c)
                != terminals.end())
                return c;

            temp.pop();
        }

        return '$';
    };

    while (true) {
        char a = topTerminal();
        char b = input[pos];

        if (a == '$' && b == '$' && st.size() == 2) {
            cout << "Accepted\n";
            break;
        }

        auto it = table.find({a, b});

        if (it == table.end()) {
            cout << "Rejected\n";
            break;
        }

        char relation = it->second;

        if (relation == '<' || relation == '=') {
            // SHIFT
            st.push(b);
            pos++;
        }
        else if (relation == '>') {
            // REDUCE
            // Pop symbols until the previous
            // precedence boundary is found.

            st.pop();

            while (!st.empty()) {
                char x = topTerminal();

                auto prev = table.find({x, st.top()});

                if (prev != table.end() &&
                    prev->second == '<')
                    break;

                st.pop();
            }

            // Replace the reduced handle with N
            st.push('N');
        }
    }

    return 0;
}

