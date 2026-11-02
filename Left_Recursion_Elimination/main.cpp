#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cout << "enter number of productions: ";
    cin >> n;
 
     vector<string> a(n), p(n);
      for (int i = 0; i < n; i++) {
        cout << "\nenter non-terminal: ";
        cin >> a[i];

        cout << "enter productions: ";
        cin >> p[i];
    }
    
     cout << "\noutput:\n";
     
    for (int i = 0; i < n; i++) {
        
        vector<string> alpha, beta;

        
        string temp = "";

        for (int j = 0; j <= p[i].length(); j++) {

            if (p[i][j] == '|' || j == p[i].length()) {

                if (temp.substr(0, a[i].length()) == a[i])
                    alpha.push_back(temp.substr(a[i].length()));
                else
                    beta.push_back(temp);

                temp = "";
            }
            else {
                temp += p[i][j];
            }
        }

        // no left recursion
        if (alpha.size() == 0) {
            cout << a[i] << " -> ";

            for (int j = 0; j < beta.size(); j++) {
                if (j > 0) cout << " | ";
                cout << beta[j];
            }

            cout << endl;
        }

        // left recursion present
        else {
            string b = a[i] + "'";

            cout << a[i] << " -> ";

            for (int j = 0; j < beta.size(); j++) {
                if (j > 0) cout << " | ";
                cout << beta[j] << b;
            }

            cout << endl;

            cout << b << " -> ";

            for (int j = 0; j < alpha.size(); j++) {
                if (j > 0) cout << " | ";
                cout << alpha[j] << b;
            }

            cout << " | eps" << endl;
        }
    }

    return 0;
}
