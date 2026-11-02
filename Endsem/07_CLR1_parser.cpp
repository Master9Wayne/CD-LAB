#include <bits/stdc++.h>
using namespace std;
struct Action { char type; int value; }; // s, r, a
struct Production { char lhs; string rhs; };

// Supply a canonical LR(1) ACTION/GOTO table for your grammar.
// Demonstration table for S' -> S, S -> a.
int main() {
    map<pair<int,char>,Action> action;
    map<pair<int,char>,int> goTo;
    vector<Production> prod={{'S',"a"}};

    action[{0,'a'}]={'s',2};
    action[{1,'$'}]={'a',0};
    action[{2,'$'}]={'r',0};
    goTo[{0,'S'}]=1;

    string input;
    cout << "Enter input: ";
    cin >> input;
    input += '$';
    stack<int> st; st.push(0);
    int pos=0;

    while (true) {
        auto it=action.find({st.top(),input[pos]});
        if (it==action.end()) { cout<<"Rejected\n"; return 0; }
        Action a=it->second;
        if (a.type=='s') { st.push(a.value); ++pos; }
        else if (a.type=='r') {
            Production pr=prod.at(a.value);
            for (int k=0;k<(int)pr.rhs.size();++k) {
                if(st.size()<=1){cout<<"Rejected\n";return 0;}
                st.pop();
            }
            auto gt=goTo.find({st.top(),pr.lhs});
            if(gt==goTo.end()){cout<<"Rejected\n";return 0;}
            st.push(gt->second);
        } else if(a.type=='a') { cout<<"Accepted\n"; return 0; }
    }
}
