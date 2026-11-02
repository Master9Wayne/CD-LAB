
#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
string input;
int pos = 0;

// Recursive descent parsing function
bool parse(char nt) {

    if (!grammar.count(nt))
        return false;

    for (string production : grammar[nt]) {

        int savedPos = pos;
        bool success = true;

        for (char symbol : production) {

            if (symbol == '#')
                continue;

            if (isupper(symbol)) {
                if (!parse(symbol)) {
                    success = false;
                    break;
                }
            }
            else {
                if (pos < input.size() && input[pos] == symbol)
                    pos++;
                else {
                    success = false;
                    break;
                }
            }
        }

        if (success)
            return true;

        // Backtrack if production fails
        pos = savedPos;
    }

    return false;
}

int main() {

    int n;
    char start;

    cout << "Enter number of non-terminals: ";
    cin >> n;

    cout << "Enter start symbol: ";
    cin >> start;

    cout << "Enter grammar productions:\n";
    cout << "Format: A ab|ac (use # for epsilon)\n";

    for (int i = 0; i < n; i++) {

        char nt;
        string rhs;

        cin >> nt >> rhs;

        stringstream ss(rhs);
        string production;

        while (getline(ss, production, '|'))
            grammar[nt].push_back(production);
    }

    cout << "Enter input string: ";
    cin >> input;

    pos = 0;

    bool accepted = parse(start);

    if (accepted && pos == input.size())
        cout << "String ACCEPTED\n";
    else
        cout << "String REJECTED\n";

    return 0;
}
