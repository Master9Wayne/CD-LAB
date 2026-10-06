#include <bits/stdc++.h>
using namespace std;

void printGrammar(const map<string, vector<string>>& grammar) {
    for (auto const& pair : grammar) {
        cout << pair.first << " -> ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            cout << pair.second[i];
            if (i < pair.second.size() - 1) {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

void removeLeftRecursion(map<string, vector<string>>& grammar) {
    map<string, vector<string>> newGrammar;
    
    for (auto const& pair : grammar) {
        string nonTerminal = pair.first;
        vector<string> rhsList = pair.second;
        vector<string> alphas, betas;

        for (const string& rhs : rhsList) {
            if (rhs.length() >= nonTerminal.length() && rhs.substr(0, nonTerminal.length()) == nonTerminal) {
                alphas.push_back(rhs.substr(nonTerminal.length()));
            } else {
                betas.push_back(rhs);
            }
        }

        if (!alphas.empty()) {
            string newNonTerminal = nonTerminal + "'";
            vector<string> newRhsA;
            vector<string> newRhsAPrime;

            if (betas.empty()) {
                newRhsA.push_back(newNonTerminal);
            } else {
                for (const string& beta : betas) {
                    if (beta == "e" || beta == "epsilon") 
                        newRhsA.push_back(newNonTerminal);
                    else 
                        newRhsA.push_back(beta + newNonTerminal);
                }
            }

            for (const string& alpha : alphas) {
                newRhsAPrime.push_back(alpha + newNonTerminal);
            }
            newRhsAPrime.push_back("e");

            newGrammar[nonTerminal] = newRhsA;
            newGrammar[newNonTerminal] = newRhsAPrime;
        } else {
            newGrammar[nonTerminal] = rhsList;
        }
    }
    grammar = newGrammar;
}

void removeLeftFactoring(map<string, vector<string>>& grammar) {
    bool changed = true;
    
    while (changed) {
        changed = false;
        map<string, vector<string>> newGrammar;

        for (auto const& pair : grammar) {
            string nonTerminal = pair.first;
            vector<string> rhsList = pair.second;

            if (changed) {
                newGrammar[nonTerminal] = rhsList;
                continue;
            }

            string bestPrefix = "";
            vector<string> bestGroup;

            for (size_t i = 0; i < rhsList.size(); i++) {
                for (size_t j = i + 1; j < rhsList.size(); j++) {
                    size_t k = 0;
                    while (k < rhsList[i].length() && k < rhsList[j].length() && rhsList[i][k] == rhsList[j][k]) {
                        k++;
                    }
                    if (k > 0) {
                        string prefix = rhsList[i].substr(0, k);
                        vector<string> currentGroup;
                        
                        for (const string& s : rhsList) {
                            if (s.find(prefix) == 0) {
                                currentGroup.push_back(s);
                            }
                        }
                        
                        if (currentGroup.size() > 1 && prefix.length() > bestPrefix.length()) {
                            bestPrefix = prefix;
                            bestGroup = currentGroup;
                        }
                    }
                }
            }

            if (bestPrefix.length() > 0) {
                changed = true;
                string newNonTerminal = nonTerminal + "*";
                
                while (grammar.count(newNonTerminal) || newGrammar.count(newNonTerminal)) {
                    newNonTerminal += "*";
                }

                vector<string> newRhsA;
                vector<string> newRhsAPrime;

                newRhsA.push_back(bestPrefix + newNonTerminal);

                for (const string& s : rhsList) {
                    if (find(bestGroup.begin(), bestGroup.end(), s) == bestGroup.end()) {
                        newRhsA.push_back(s);
                    } else {
                        string remainder = s.substr(bestPrefix.length());
                        if (remainder == "") remainder = "e";
                        newRhsAPrime.push_back(remainder);
                    }
                }

                newGrammar[nonTerminal] = newRhsA;
                newGrammar[newNonTerminal] = newRhsAPrime;
            } else {
                newGrammar[nonTerminal] = rhsList;
            }
        }
        if (changed) {
            grammar = newGrammar;
        }
    }
}

int main() {
    map<string, vector<string>> grammar;
    int n;
    
    cout << "--- Grammar Minimizer (Left Recursion & Left Factoring) ---\n";
    cout << "Note: Use 'e' to represent epsilon (empty string).\n\n";
    
    cout << "Enter the number of productions: ";
    if (!(cin >> n)) return 0;
    
    cout << "Enter productions in the format (S->aA|bB|c) without spaces:\n";
    for (int i = 0; i < n; i++) {
        string input;
        cin >> input;
        
        size_t arrowPos = input.find("->");
        if (arrowPos == string::npos) {
            cout << "Invalid format. Ignoring: " << input << endl;
            continue;
        }
        
        string nonTerminal = input.substr(0, arrowPos);
        string rhsStr = input.substr(arrowPos + 2);
        
        stringstream ss(rhsStr);
        string rhs;
        while (getline(ss, rhs, '|')) {
            grammar[nonTerminal].push_back(rhs);
        }
    }

    cout << "\n[ ORIGINAL GRAMMAR ]\n";
    printGrammar(grammar);

    removeLeftRecursion(grammar);
    cout << "\n[ AFTER LEFT RECURSION REMOVAL ]\n";
    printGrammar(grammar);

    removeLeftFactoring(grammar);
    cout << "\n[ AFTER LEFT FACTORING REMOVAL ]\n";
    printGrammar(grammar);

    return 0;
}
