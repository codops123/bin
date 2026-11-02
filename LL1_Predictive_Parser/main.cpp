
#include <iostream>
#include <stack>
#include <map>
#include <vector>
using namespace std;

map<pair<char, char>, string> table;

void createTable() {
    table[{'S', 'a'}] = "aABe";

    table[{'A', 'b'}] = "bAc";
    table[{'A', 'd'}] = "d";

    table[{'B', 'f'}] = "f";
    table[{'B', 'e'}] = "eps";
}

void displayTable() {
    char nt[] = {'S', 'A', 'B'};
    char t[] = {'a', 'b', 'c', 'd', 'e', 'f', '$'};

    cout << "\nLL(1) Parsing Table\n\n";
    cout << "\t";

    for (char x : t)
        cout << x << "\t";

    cout << endl;

    for (char x : nt) {
        cout << x << "\t";

        for (char y : t) {
            if (table.find({x, y}) != table.end())
                cout << table[{x, y}] << "\t";
            else
                cout << "-\t";
        }

        cout << endl;
    }
}

void parse(string input) {
    stack<char> st;

    st.push('$');
    st.push('S');
    input += '$';

    int i = 0, step = 1;

    cout << "\nStep\tStack\t\tInput\t\tAction\n";

    while (!st.empty()) {
        char top = st.top();
        char ch = input[i];

        // Stack ko print karna (original stack change nahi hoga)
        stack<char> temp = st;
        string s = "";

        while (!temp.empty()) {
            s = temp.top() + s;
            temp.pop();
        }

        cout << step++ << "\t" << s << "\t\t"
             << input.substr(i) << "\t\t";

        // Dono $ hain = Accepted
        if (top == '$' && ch == '$') {
            cout << "Accepted\n";
            return;
        }

        // Terminal match
        else if (top == ch) {
            st.pop();
            i++;
            cout << "Match " << top << endl;
        }

        // Non-terminal
        else if (top >= 'A' && top <= 'Z') {

            if (table.find({top, ch}) == table.end()) {
                cout << "Rejected\n";
                return;
            }

            string p = table[{top, ch}];
            st.pop();

            // Production ko reverse order mein push karo
            if (p != "eps") {
                for (int j = p.length() - 1; j >= 0; j--)
                    st.push(p[j]);
            }

            cout << top << " -> " << p << endl;
        }

        // Mismatch
        else {
            cout << "Rejected\n";
            return;
        }
    }

    cout << "Rejected";
}
int main() {
    createTable();

    cout << "FIRST:\n";
    cout << "FIRST(S) = {a}\n";
    cout << "FIRST(A) = {b, d}\n";
    cout << "FIRST(B) = {f, eps}\n";

    cout << "\nFOLLOW:\n";
    cout << "FOLLOW(S) = {$}\n";
    cout << "FOLLOW(A) = {f, e}\n";
    cout << "FOLLOW(B) = {e}\n";

    displayTable();

    string input;
    cout << "\nEnter input string: ";
    cin >> input;

    parse(input);

    return 0;
}

