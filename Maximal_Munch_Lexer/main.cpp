#include <bits/stdc++.h>
using namespace std;

int main() {

    unordered_set<string> operators = {
        "+", "-", "*", "/", "=",
        ">", "<", "!", ">=", "<=",
        "==", "!=", "++", "--",
        "+=", "-=", "*=", "/=",
        "<<", ">>", "<<=", ">>="
    };

    string input;
    cout << "Enter expression: ";
    getline(cin, input);

    int i = 0;

    while (i < input.size()) {

        if (input[i] == ' ') {
            i++;
        }

        else if (operators.find(string(1,input[i])) != operators.end()) {

            string match = "";

            for (int j = 1; j <= 3 && i + j <= input.size(); j++) {

                string temp = input.substr(i, j);

                if (operators.find(temp) != operators.end()) {
                    match = temp;
                }
            }

            if (match != "") {
                cout << match << endl;
                i += match.length();
            }
            else {
                cout << "Invalid operator\n";
                i++;
            }
        }

        else {

            string word = "";

            while (i < input.size() &&
                   input[i] != ' '&& 
                   operators.find(string(1, input[i])) == operators.end()) {

                word += input[i];
                i++;
            }

            cout << word << endl;
        }
    }

    return 0;
}
