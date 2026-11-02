
#include <iostream>
#include <string>
using namespace std;

int main() {

    // LR(0) parsing table
    cout << "\nLR(0) PARSING TABLE\n\n";

    cout << "State\t a\t b\t $\t S\t A\n";
    cout << "0\t s3\t -\t -\t 1\t 2\n";
    cout << "1\t -\t -\t acc\t -\t -\n";
    cout << "2\t -\t s4/r1\t r1\t -\t -\n";
    cout << "3\t r3\t r3\t r3\t -\t -\n";
    cout << "4\t r2\t r2\t r2\t -\t -\n";

    cout << "\nSR CONFLICT FOUND!\n";
    cout << "State 2, symbol b: s4 / r1\n";
    cout << "Grammar is not LR(0).\n";

    // SLR(1) parsing table
    cout << "\nSLR(1) PARSING TABLE\n\n";

    cout << "State\t a\t b\t $\t S\t A\n";
    cout << "0\t s3\t -\t -\t 1\t 2\n";
    cout << "1\t -\t -\t acc\t -\t -\n";
    cout << "2\t -\t s4\t r1\t -\t -\n";
    cout << "3\t -\t r3\t r3\t -\t -\n";
    cout << "4\t -\t r2\t r2\t -\t -\n";

    cout << "\nNo conflict in SLR(1).\n";
    cout << "Grammar is SLR(1).\n";

    // input string
    string input;
    cout << "\nenter input string: ";
    cin >> input;

    input += '$';

    string st = "0";
    int i = 0;

    cout << "\nStack\t\tInput\t\tAction\n";

    while (true) {

        int state = st[st.length() - 1] - '0';
        char ch = input[i];

        string action = "";

        // SLR(1) action table
        if (state == 0 && ch == 'a')
            action = "s3";

        else if (state == 1 && ch == '$')
            action = "acc";

        else if (state == 2 && ch == 'b')
            action = "s4";

        else if (state == 2 && ch == '$')
            action = "r1";

        else if (state == 3 && ch == 'b')
            action = "r3";

        else if (state == 3 && ch == '$')
            action = "r3";

        else if (state == 4 && ch == 'b')
            action = "r2";

        else if (state == 4 && ch == '$')
            action = "r2";

        cout << st << "\t\t" << input.substr(i) << "\t\t";

        // shift
        if (action == "s3" || action == "s4") {

            cout << "shift " << action << endl;

            st += ch;
            st += action[1];
            i++;
        }

        // reduce A -> a
        else if (action == "r3") {

            cout << "reduce A -> a" << endl;

            st.erase(st.length() - 2);
            st += "A2";
        }

        // reduce A -> Ab
        else if (action == "r2") {

            cout << "reduce A -> Ab" << endl;

            st.erase(st.length() - 4);
            st += "A2";
        }

        // reduce S -> A
        else if (action == "r1") {

            cout << "reduce S -> A" << endl;

            st.erase(st.length() - 2);
            st += "S1";
        }

        // accept
        else if (action == "acc") {

            cout << "ACCEPTED" << endl;
            break;
        }

        else {

            cout << "REJECTED" << endl;
            break;
        }
    }

    return 0;
}

