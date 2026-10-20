
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
using namespace std;

struct Token {
    string type;
    string lexeme;
};

vector<Token> tokenize(const string& input) {
    vector<Token> tokens;

    for (size_t i = 0; i < input.size();) {
        if (isspace(static_cast<unsigned char>(input[i]))) {
            i++;
        } else if (isalnum(static_cast<unsigned char>(input[i])) ||
                   input[i] == '_') {
            string word;
            while (i < input.size() &&
                   (isalnum(static_cast<unsigned char>(input[i])) ||
                    input[i] == '_')) {
                word += input[i++];
            }
            tokens.push_back({"id", word});
        } else if (input[i] == '+' || input[i] == '*' ||
                   input[i] == '(' || input[i] == ')') {
            string symbol(1, input[i]);
            tokens.push_back({symbol, symbol});
            i++;
        } else {
            throw runtime_error("Invalid character in input");
        }
    }

    tokens.push_back({"$", "$"});
    return tokens;
}

struct Rule {
    string lhs;
    int length;
    string rhs;
};

Rule rules[] = {
    {"",  0, ""},
    {"E", 3, "E+T"},  // r1
    {"E", 1, "T"},    // r2
    {"T", 3, "T*F"},  // r3
    {"T", 1, "F"},    // r4
    {"F", 3, "(E)"},  // r5
    {"F", 1, "id"}    // r6
};

int actionTable[12][6] = {
    { 5,  0,  0,  4,  0,  0},  // 0
    { 0,  6,  0,  0,  0, 99},  // 1
    { 0, -2,  7,  0, -2, -2},  // 2
    { 0, -4, -4,  0, -4, -4},  // 3
    { 5,  0,  0,  4,  0,  0},  // 4
    { 0, -6, -6,  0, -6, -6},  // 5
    { 5,  0,  0,  4,  0,  0},  // 6
    { 5,  0,  0,  4,  0,  0},  // 7
    { 0,  6,  0,  0, 11,  0},  // 8
    { 0, -1,  7,  0, -1, -1},  // 9
    { 0, -3, -3,  0, -3, -3},  // 10
    { 0, -5, -5,  0, -5, -5}   // 11
};

// Columns: id, +, *, (, ), $
int gotoTable[12][3] = {
    {1, 2, 3},  // 0
    {-1,-1,-1}, // 1
    {-1,-1,-1}, // 2
    {-1,-1,-1}, // 3
    {8, 2, 3},  // 4
    {-1,-1,-1}, // 5
    {-1, 9, 3}, // 6
    {-1,-1,10}, // 7
    {-1,-1,-1}, // 8
    {-1,-1,-1}, // 9
    {-1,-1,-1}, // 10
    {-1,-1,-1}  // 11
};

int actionColumn(const string& token) {
    if (token == "id") return 0;
    if (token == "+")  return 1;
    if (token == "*")  return 2;
    if (token == "(")  return 3;
    if (token == ")")  return 4;
    if (token == "$")  return 5;
    return -1;
}

int gotoColumn(const string& symbol) {
    if (symbol == "E") return 0;
    if (symbol == "T") return 1;
    if (symbol == "F") return 2;
    return -1;
}

string stackString(const vector<string>& symbols,
                   const vector<int>& states) {
    string result = to_string(states[0]);

    for (size_t i = 0; i < symbols.size(); i++) {
        result += " " + symbols[i] + " " +
                  to_string(states[i + 1]);
    }

    return result;
}

int main() {
    string input;
    cout << "Enter expression: ";
    getline(cin, input);

    vector<Token> tokens;

    try {
        tokens = tokenize(input);
    } catch (const exception& e) {
        cout << e.what() << '\n';
        return 1;
    }

    vector<string> symbols;
    vector<int> states = {0};
    size_t pos = 0;
    int steps = 0;

    cout << "\n"
         << left << setw(35) << "STACK"
         << setw(35) << "INPUT"
         << "ACTION\n";

    cout << string(85, '-') << '\n';

    while (steps++ < 10000) {
        int state = states.back();
        string lookahead = tokens[pos].type;
        int col = actionColumn(lookahead);

        ostringstream remaining;
        for (size_t i = pos; i < tokens.size(); i++) {
            remaining << tokens[i].type << ' ';
        }

        cout << left << setw(35)
             << stackString(symbols, states)
             << setw(35) << remaining.str();

        if (col == -1) {
            cout << "ERROR: Invalid token\n";
            return 1;
        }

        int action = actionTable[state][col];

        if (action == 99) {
            cout << "ACCEPT\n";
            return 0;
        }

        if (action > 0) {
            cout << "Shift " << action << '\n';

            symbols.push_back(lookahead);
            states.push_back(action);
            pos++;
        } else if (action < 0) {
            int ruleNo = -action;
            Rule rule = rules[ruleNo];

            cout << "Reduce by " << rule.lhs
                 << " -> " << rule.rhs << '\n';

            for (int i = 0; i < rule.length; i++) {
                if (symbols.empty() || states.size() <= 1) {
                    cout << "ERROR: Invalid stack\n";
                    return 1;
                }
                symbols.pop_back();
                states.pop_back();
            }

            int nextState = states.back();
            int gc = gotoColumn(rule.lhs);

            if (gc == -1 || gotoTable[nextState][gc] == -1) {
                cout << "ERROR: Invalid GOTO\n";
                return 1;
            }

            symbols.push_back(rule.lhs);
            states.push_back(gotoTable[nextState][gc]);
        } else {
            cout << "ERROR: String rejected\n";
            return 1;
        }
    }

    cout << "ERROR: Parsing limit exceeded\n";
    return 1;
}
