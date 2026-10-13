
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <iomanip>
using namespace std;

// Non-terminals:
// E  = E
// P  = E'
// T  = T
// Q  = T'
// F  = F
//
// Terminals: id, +, *, (, ), $

const string table[5][6] = {
    // id       +        *        (        )       $
    {"TP",     "",      "",      "TP",    "",     ""},
    {"",       "+TP",   "",      "",      "e",    "e"},
    {"FQ",     "",      "",      "FQ",    "",     ""},
    {"",       "e",     "*FQ",   "",      "e",    "e"},
    {"id",     "",      "",      "(E)",   "",     ""}
};

const char* nonTerminals[5] = {"E", "P", "T", "Q", "F"};

int getRow(char c) {
    switch (c) {
        case 'E': return 0;
        case 'P': return 1;
        case 'T': return 2;
        case 'Q': return 3;
        case 'F': return 4;
        default: return -1;
    }
}

int getColumn(const string& token) {
    if (token == "id") return 0;
    if (token == "+")  return 1;
    if (token == "*")  return 2;
    if (token == "(")  return 3;
    if (token == ")")  return 4;
    if (token == "$")  return 5;
    return -1;
}

bool isNonTerminal(char c) {
    return c == 'E' || c == 'P' ||
           c == 'T' || c == 'Q' || c == 'F';
}

vector<string> tokenize(const string& input) {
    vector<string> tokens;

    for (size_t i = 0; i < input.size();) {
        if (input.compare(i, 2, "id") == 0) {
            tokens.push_back("id");
            i += 2;
        } else {
            tokens.push_back(string(1, input[i]));
            i++;
        }
    }

    tokens.push_back("$");
    return tokens;
}

string stackToString(stack<string> s) {
    vector<string> elements;

    while (!s.empty()) {
        elements.push_back(s.top());
        s.pop();
    }

    string result;
    for (int i = (int)elements.size() - 1; i >= 0; i--) {
        result += elements[i];
        if (i > 0) result += " ";
    }

    return result;
}

int main() {
    string input;

    cout << "Enter input string (use id for identifiers): ";
    cin >> input;

    vector<string> tokens = tokenize(input);

    stack<string> st;
    st.push("$");
    st.push("E");

    size_t pos = 0;

    cout << "\n";
    cout << left
         << setw(25) << "Stack"
         << setw(25) << "Input"
         << "Action\n";

    cout << string(75, '-') << "\n";

    while (!st.empty()) {
        string currentStack = stackToString(st);

        string remainingInput;
        for (size_t i = pos; i < tokens.size(); i++) {
            remainingInput += tokens[i];
            if (i + 1 < tokens.size()) remainingInput += " ";
        }

        string top = st.top();
        string lookahead = tokens[pos];
        string action;

        if (top == "$" && lookahead == "$") {
            cout << left
                 << setw(25) << currentStack
                 << setw(25) << remainingInput
                 << "Accept\n";
            return 0;
        }

        if (!isNonTerminal(top[0])) {
            if (top == lookahead) {
                st.pop();
                pos++;
                action = "Match " + lookahead;
            } else {
                cout << left
                     << setw(25) << currentStack
                     << setw(25) << remainingInput
                     << "Error: terminal mismatch\n";
                return 0;
            }
        } else {
            int row = getRow(top[0]);
            int col = getColumn(lookahead);

            if (row == -1 || col == -1 ||
                table[row][col].empty()) {
                cout << left
                     << setw(25) << currentStack
                     << setw(25) << remainingInput
                     << "Error: no table entry\n";
                return 0;
            }

            string production = table[row][col];
            st.pop();

            if (production != "e") {
                vector<string> rhs;

                for (size_t i = 0; i < production.size();) {
                    if (production.compare(i, 2, "id") == 0) {
                        rhs.push_back("id");
                        i += 2;
                    } else {
                        rhs.push_back(string(1, production[i]));
                        i++;
                    }
                }

                for (int i = (int)rhs.size() - 1; i >= 0; i--) {
                    st.push(rhs[i]);
                }
            }

            action = top + " -> " +
                     (production == "e" ? "epsilon" : production);
        }

        cout << left
             << setw(25) << currentStack
             << setw(25) << remainingInput
             << action << "\n";
    }

    return 0;
}

