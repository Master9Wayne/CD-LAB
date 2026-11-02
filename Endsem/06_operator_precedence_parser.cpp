#include <bits/stdc++.h>
using namespace std;

/*
    TABLE-DRIVEN OPERATOR PRECEDENCE PARSER
    ---------------------------------------
    This program checks whether an input string is accepted using:
      1. A user-provided operator precedence table.
      2. User-provided grammar productions.

    SYMBOL CONVENTIONS
      - Terminals: single characters, e.g. i, +, *, (, ), $
      - Non-terminals: uppercase single characters, e.g. E, T, F
      - '$' is reserved as the end marker.
      - Do not include '$' in grammar productions.
      - Epsilon productions (#) are NOT allowed in operator-precedence
        grammars. Such grammars must be epsilon-free and must not have
        two adjacent non-terminals on a production's RHS.

    WHAT TO CHANGE FOR A DIFFERENT GRAMMAR
      A. Enter the new terminal set.
      B. Enter the complete precedence table in the same terminal order.
      C. Enter the grammar productions (one production per line).
      D. Enter the start symbol and input string.

    Example operator-precedence grammar:
      E -> E+E | E*E | (E) | i

    Example terminals (including end marker):
      + * i ( ) $

    Example precedence table, with rows and columns in that order:
             +   *   i   (   )   $
        +    >   <   <   <   >   >
        *    >   >   <   <   >   >
        i    >   >   -   -   >   >
        (    <   <   <   <   =   -
        )    >   >   -   -   >   >
        $    <   <   <   <   -   =

    Use:
      <  =  >   for precedence relations
      -       for "no relation"

    IMPORTANT:
      The precedence table must be consistent with the grammar.
      The parser performs exact RHS handle matching during reductions.
*/

struct Production {
    char lhs;
    string rhs;
};

// A symbol is a terminal if it appears in the user-provided terminal set.
bool isTerminal(char c, const set<char>& terminals) {
    return terminals.count(c) > 0;
}

// Find the rightmost terminal in the stack.
// Non-terminals are skipped.
int rightmostTerminalIndex(const vector<char>& st,
                           const set<char>& terminals) {
    for (int i = (int)st.size() - 1; i >= 0; --i) {
        if (isTerminal(st[i], terminals))
            return i;
    }
    return -1;
}

// Find a grammar production whose complete RHS equals the handle.
// Returns its index, or -1 if no production matches.
// If more than one production has the same RHS, the grammar is ambiguous
// for this reduction and the function reports that separately.
int findProduction(const string& handle,
                   const vector<Production>& productions,
                   bool& ambiguous) {
    int found = -1;
    ambiguous = false;

    for (int i = 0; i < (int)productions.size(); ++i) {
        if (productions[i].rhs == handle) {
            if (found != -1) {
                ambiguous = true;
                return -1;
            }
            found = i;
        }
    }
    return found;
}

// Print the current stack and the unconsumed input.
void printConfiguration(const vector<char>& st,
                        const string& input,
                        int pos,
                        const string& action) {
    cout << left << setw(22);

    string stackText(st.begin(), st.end());
    cout << stackText;

    cout << left << setw(22) << input.substr(pos);
    cout << action << '\n';
}

int main() {
    cout << "===== OPERATOR PRECEDENCE PARSER =====\n\n";

    // ---------------------------------------------------------
    // STEP 1: Read the terminal symbols.
    // Include '$' as the final terminal.
    // Example: + * i ( ) $
    // ---------------------------------------------------------
    int n;
    cout << "Enter number of terminals (including $): ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of terminals.\n";
        return 0;
    }

    vector<char> terminalsList(n);
    set<char> terminals;

    cout << "Enter terminals (single characters, including $): ";
    for (int i = 0; i < n; ++i) {
        cin >> terminalsList[i];

        if (!terminals.insert(terminalsList[i]).second) {
            cout << "Duplicate terminal: " << terminalsList[i] << '\n';
            return 0;
        }
    }

    if (!terminals.count('$')) {
        cout << "Error: '$' must be included as an end marker.\n";
        return 0;
    }

    // ---------------------------------------------------------
    // STEP 2: Read the precedence table.
    //
    // table[a][b] means the relation between:
    //   a = rightmost terminal on the stack
    //   b = current input/lookahead terminal
    //
    // Enter '<', '=', '>', or '-' (no relation).
    // ---------------------------------------------------------
    map<pair<char, char>, char> table;

    cout << "\nEnter the operator precedence table.\n";
    cout << "For every row/column pair, enter <, =, >, or -.\n";
    cout << "'-' means no precedence relation.\n\n";

    cout << "Rows = stack terminals; columns = input terminals.\n\n";

    cout << setw(8) << "";
    for (char c : terminalsList)
        cout << setw(6) << c;
    cout << '\n';

    for (char row : terminalsList) {
        cout << setw(8) << row;

        for (char col : terminalsList) {
            char relation;
            cin >> relation;

            if (relation != '<' && relation != '=' &&
                relation != '>' && relation != '-') {
                cout << "Invalid relation. Use <, =, >, or -.\n";
                return 0;
            }

            if (relation != '-')
                table[{row, col}] = relation;

            cout << setw(6) << relation;
        }
        cout << '\n';
    }

    // ---------------------------------------------------------
    // STEP 3: Read grammar productions.
    //
    // Enter one production per line:
    //   E E+E
    //   E E*E
    //   E (E)
    //   E i
    //
    // The first item is the LHS; the second is the RHS.
    // Do not enter arrows or spaces inside the RHS.
    // ---------------------------------------------------------
    int productionCount;
    cout << "\nEnter number of grammar productions: ";
    cin >> productionCount;

    if (productionCount <= 0) {
        cout << "Invalid number of productions.\n";
        return 0;
    }

    vector<Production> productions;
    set<char> nonTerminals;

    cout << "Enter productions as: LHS RHS\n";
    cout << "Example: E E+T\n";
    cout << "Use one line for each alternative.\n";

    for (int i = 0; i < productionCount; ++i) {
        char lhs;
        string rhs;
        cin >> lhs >> rhs;

        if (!isupper((unsigned char)lhs)) {
            cout << "Error: LHS must be an uppercase non-terminal.\n";
            return 0;
        }

        if (rhs == "#") {
            cout << "Error: epsilon productions are not allowed in "
                    "operator-precedence grammars.\n";
            return 0;
        }

        // Check that every RHS symbol is either a declared terminal
        // or an uppercase non-terminal.
        for (char c : rhs) {
            if (!isTerminal(c, terminals) &&
                !isupper((unsigned char)c)) {
                cout << "Error: symbol '" << c
                     << "' is neither a declared terminal nor "
                        "an uppercase non-terminal.\n";
                return 0;
            }

            if (c == '$') {
                cout << "Error: '$' is reserved for the input end marker "
                        "and cannot appear in a production.\n";
                return 0;
            }
        }

        nonTerminals.insert(lhs);
        for (char c : rhs) {
            if (isupper((unsigned char)c))
                nonTerminals.insert(c);
        }

        // Operator-precedence parsing cannot reduce a RHS that has
        // no terminal boundary (for example E -> T). Use an
        // operator-precedence grammar with at least one terminal
        // in every RHS, and no adjacent non-terminals.
        bool hasTerminal = false;
        for (char c : rhs)
            if (isTerminal(c, terminals))
                hasTerminal = true;

        if (!hasTerminal) {
            cout << "Error: each RHS must contain at least one terminal. "
                    "Unit productions such as E -> T are not supported "
                    "by this handle-reduction parser.\n";
            return 0;
        }

        for (int j = 1; j < (int)rhs.size(); ++j) {
            if (isupper((unsigned char)rhs[j - 1]) &&
                isupper((unsigned char)rhs[j])) {
                cout << "Error: adjacent non-terminals are not allowed "
                        "in an operator-precedence grammar.\n";
                return 0;
            }
        }

        productions.push_back({lhs, rhs});
    }

    // ---------------------------------------------------------
    // STEP 4: Read the start symbol and input string.
    // The program appends '$' automatically.
    // ---------------------------------------------------------
    char startSymbol;
    cout << "Enter start symbol: ";
    cin >> startSymbol;

    if (!nonTerminals.count(startSymbol)) {
        cout << "Error: start symbol has no grammar production.\n";
        return 0;
    }

    string input;
    cout << "Enter input string: ";
    cin >> input;

    // '$' is appended by the parser. Do not enter it yourself.
    if (!input.empty() && input.back() == '$') {
        cout << "Enter the input WITHOUT the final '$'; "
                "the program appends it automatically.\n";
        return 0;
    }

    for (char c : input) {
        if (!isTerminal(c, terminals) || c == '$') {
            cout << "Invalid input symbol: " << c << '\n';
            return 0;
        }
    }

    input += '$';

    // ---------------------------------------------------------
    // STEP 5: Perform operator-precedence parsing.
    //
    // Stack initially contains only '$'.
    //
    // SHIFT:
    //   If relation is '<' or '=', shift the lookahead terminal.
    //
    // REDUCE:
    //   If relation is '>', find the complete handle:
    //     - Pop stack symbols from right to left.
    //     - Find the nearest terminal to the left of the handle.
    //     - Stop when its relation with the leftmost terminal
    //       of the handle is '<'.
    //   Reverse the collected handle.
    //   Match the COMPLETE handle against a grammar RHS.
    //   Replace the handle with that production's LHS.
    //
    // ACCEPT:
    //   Stack is '$' followed by the start symbol, and lookahead is '$'.
    // ---------------------------------------------------------
    vector<char> st;
    st.push_back('$');

    int pos = 0;
    int steps = 0;
    const int MAX_STEPS = 10000; // Prevent infinite loops for bad tables.

    cout << "\nPARSING TRACE\n";
    cout << left << setw(22) << "STACK"
         << setw(22) << "INPUT"
         << "ACTION\n";
    cout << string(65, '-') << '\n';

    while (steps++ < MAX_STEPS) {
        // Successful final configuration:
        // stack = [$, startSymbol], input lookahead = '$'.
        if (pos < (int)input.size() &&
            input[pos] == '$' &&
            st.size() == 2 &&
            st[0] == '$' &&
            st[1] == startSymbol) {
            printConfiguration(st, input, pos, "ACCEPT");
            cout << "\nString ACCEPTED.\n";
            return 0;
        }

        if (pos >= (int)input.size()) {
            cout << "\nString REJECTED: input ended unexpectedly.\n";
            return 0;
        }

        int ti = rightmostTerminalIndex(st, terminals);
        if (ti == -1) {
            cout << "\nString REJECTED: no terminal found on stack.\n";
            return 0;
        }

        char stackTerminal = st[ti];
        char lookahead = input[pos];

        auto relationIt = table.find({stackTerminal, lookahead});

        // Missing relation means syntax error.
        if (relationIt == table.end()) {
            printConfiguration(st, input, pos, "ERROR: no precedence relation");
            cout << "\nString REJECTED.\n";
            return 0;
        }

        char relation = relationIt->second;

        // ---------------- SHIFT ----------------
        if (relation == '<' || relation == '=') {
            // The end marker must never be shifted.
            if (lookahead == '$') {
                printConfiguration(st, input, pos,
                                   "ERROR: cannot shift end marker");
                cout << "\nString REJECTED.\n";
                return 0;
            }

            string action = "SHIFT '" + string(1, lookahead) + "'";
            printConfiguration(st, input, pos, action);

            st.push_back(lookahead);
            ++pos;
        }

        // ---------------- REDUCE ----------------
        else if (relation == '>') {
            // Keep the pre-reduction configuration for the trace.
            const vector<char> stackBeforeReduction = st;

            vector<char> handleReversed;
            bool foundBoundary = false;
            char leftmostTerminalOfHandle = '\0';

            // Pop at least one symbol, then continue until the
            // '<' boundary between adjacent terminals is found.
            while (st.size() > 1) {
                char popped = st.back();
                st.pop_back();
                handleReversed.push_back(popped);

                if (isTerminal(popped, terminals)) {
                    // Since we pop right-to-left, this variable is
                    // updated until it holds the leftmost terminal
                    // of the handle.
                    leftmostTerminalOfHandle = popped;

                    int leftIndex = rightmostTerminalIndex(st, terminals);
                    if (leftIndex == -1)
                        break;

                    char leftTerminal = st[leftIndex];

                    auto boundaryIt =
                        table.find({leftTerminal,
                                    leftmostTerminalOfHandle});

                    if (boundaryIt == table.end()) {
                        break;
                    }

                    if (boundaryIt->second == '<') {
                        // The handle may begin with one or more
                        // non-terminals between this boundary terminal
                        // and the leftmost terminal of the handle.
                        // Include those symbols too.
                        while ((int)st.size() - 1 > leftIndex) {
                            handleReversed.push_back(st.back());
                            st.pop_back();
                        }

                        foundBoundary = true;
                        break;
                    }
                }
            }

            if (!foundBoundary) {
                printConfiguration(stackBeforeReduction, input, pos,
                                   "ERROR: handle boundary not found");
                cout << "\nString REJECTED.\n";
                return 0;
            }

            reverse(handleReversed.begin(), handleReversed.end());
            string handle(handleReversed.begin(), handleReversed.end());

            bool ambiguous = false;
            int productionIndex =
                findProduction(handle, productions, ambiguous);

            if (ambiguous) {
                printConfiguration(stackBeforeReduction, input, pos,
                                   "ERROR: multiple productions match handle");
                cout << "\nString REJECTED: ambiguous reduction.\n";
                return 0;
            }

            if (productionIndex == -1) {
                printConfiguration(stackBeforeReduction, input, pos,
                                   "ERROR: no production matches handle");
                cout << "\nString REJECTED: invalid handle '" 
                     << handle << "'.\n";
                return 0;
            }

            char lhs = productions[productionIndex].lhs;

            string action = "REDUCE " + string(1, lhs) + " -> " + handle;
            printConfiguration(stackBeforeReduction, input, pos, action);

            // Replace the complete RHS handle with its LHS.
            st.push_back(lhs);
        }

        // A '-' relation is not stored in the map, so it is handled
        // by the missing-relation case above.
    }

    cout << "\nString REJECTED: maximum parsing steps exceeded.\n";
    return 0;
}
