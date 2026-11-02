
#include <iostream>
#include <map>
#include <string>
#include <cctype>
using namespace std;

map<string, string> table;
string s;
int pos = 0;

struct Node {
    string value;
    Node *left, *right;

    Node(string v, Node *l = NULL, Node *r = NULL) {
        value = v;
        left = l;
        right = r;
    }
};

void skip() {
    while (pos < s.size() && s[pos] == ' ')
        pos++;
}

Node* expression();

Node* factor() {
    skip();

    if (pos < s.size() && s[pos] == '(') {
        pos++;
        Node* t = expression();
        skip();
        pos++;
        return t;
    }

    string v;
    while (pos < s.size() &&
          (isalnum(s[pos]) || s[pos] == '.'))
        v += s[pos++];

    return new Node(v);
}

Node* term() {
    Node* l = factor();
    skip();

    while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
        string op = s.substr(pos++, 1);
        l = new Node(op, l, factor());
        skip();
    }
    return l;
}

Node* expression() {
    Node* l = term();
    skip();

    while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
        string op = s.substr(pos++, 1);
        l = new Node(op, l, term());
        skip();
    }
    return l;
}

string check(Node* root) {
    if (root->left == NULL && root->right == NULL) {
        string v = root->value;

        if (isdigit(v[0]))
            return (v.find('.') != string::npos) ? "float" : "int";

        if (table.find(v) == table.end()) {
            cout << "Error: Undeclared Variable " << v << endl;
            return "error";
        }
        return table[v];
    }

    string l = check(root->left);
    string r = check(root->right);

    if (l == "error" || r == "error")
        return "error";

    if ((l != "int" && l != "float") ||
        (r != "int" && r != "float")) {
        cout << "Error: Invalid operands for " << root->value << endl;
        return "error";
    }

    return (l == "float" || r == "float") ? "float" : "int";
}

int main() {
    int n;
    string name, type, variable;

    cout << "Enter number of variables: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Enter variable name and type: ";
        cin >> name >> type;
        table[name] = type;
    }

    cout << "\nEnter assignment variable: ";
    cin >> variable;

    if (table.find(variable) == table.end()) {
        cout << "Error: Undeclared Variable " << variable;
        return 0;
    }

    cout << "Enter expression: ";
    cin.ignore();
    getline(cin, s);

    Node* root = expression();

    cout << "\nType of " << variable << " = " << table[variable] << endl;

    string result = check(root);

    if (result == "error")
        return 0;

    cout << "Expression type = " << result << endl;

    if (table[variable] == result)
        cout << "Assignment Valid";
    else
        cout << "Type Mismatch";

    return 0;
}

