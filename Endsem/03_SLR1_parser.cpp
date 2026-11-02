#include <bits/stdc++.h>
using namespace std;

struct Action { char type; int value; }; // s=shift, r=reduce, a=accept
struct Production { char lhs; string rhs; };

int main() {
    // Demonstration table for augmented grammar:
    // (0) S' -> S
    // (1) S  -> a
    map<pair<int,char>, Action> action;
    map<pair<int,char>, int> goTo;
    vector<Production> prod = {{'S',"a"}};

    action[{0,'a'}] = {'s',2};
    action[{1,'$'}] = {'a',0};
    action[{2,'$'}] = {'r',1};
    goTo[{0,'S'}] = 1;

    string input;
    cout << "Enter input: ";
    cin >> input;
    input += '$';

    stack<int> states;
    states.push(0);
    int pos = 0;

    while (true) {
        int state = states.top();
        char look = input[pos];
        auto it = action.find({state,look});
        if (it == action.end()) { cout << "Rejected\n"; return 0; }

        Action a = it->second;
        if (a.type == 's') {
            states.push(a.value);
            ++pos;
        } else if (a.type == 'r') {
            Production p = prod.at(a.value - 1);
            for (int k = 0; k < (int)p.rhs.size(); ++k) {
                if (states.size() <= 1) { cout << "Rejected\n"; return 0; }
                states.pop();
            }
            auto gt = goTo.find({states.top(),p.lhs});
            if (gt == goTo.end()) { cout << "Rejected\n"; return 0; }
            states.push(gt->second);
        } else if (a.type == 'a') {
            cout << "Accepted\n";
            return 0;
        }
    }
}
