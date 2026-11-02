#include <iostream>
using namespace std;

int main() {
    string s;
    cout << "Enter string: ";
    cin >> s;

    int n = s.length();
    int i = 0;

    while(i < n - 2) {
        if(s.substr(i, 3) != "aab") {
            cout << "Rejected";
            return 0;
        }
        i += 3;
    }

    if(i == n - 2 && s.substr(i, 2) == "aa")
        cout << "Accepted";
    else
        cout << "Rejected";

    return 0;
}
