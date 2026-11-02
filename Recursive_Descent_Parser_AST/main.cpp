
/* E  → T E'

E' → + T E' | - T E' | ε

T  → F T'

T' → * F T' | / F T' | ε

F  → ( E ) | id | num */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
using namespace std;

struct node {
    string data;
    node *left, *right;

    node(string d, node* l = NULL, node* r = NULL) {
        data = d;
        left = l;
        right = r;
    }
};

vector<string> t;
map<string, double> val;

int i = 0;
bool error = false;

//node* s();
node* e();
node* eprime(node* left);
node* t1();
node* tprime(node* left);
node* f();

/*
node* s() {
    if (t[i] == "id") {
        node* left = new node("id");
        i++;

        if (t[i] == "=") {
            i++;

            node* right = e();

            if (right == NULL) return NULL;

            return new node("=", left, right);
        }
    }

    error = true;
    return NULL;
}
*/

node* e() {
    node* left = t1();

    if (left == NULL) return NULL;

    return eprime(left);
}

node* eprime(node* left) {
    if (t[i] == "+") {
        i++;
        node* right = t1();

        if (right == NULL) return NULL;

        node* n = new node("+", left, right);
        return eprime(n);
    }

    if (t[i] == "-") {
        i++;
        node* right = t1();

        if (right == NULL) return NULL;

        node* n = new node("-", left, right);
        return eprime(n);
    }

    return left;
}

node* t1() {
    node* left = f();

    if (left == NULL) return NULL;

    return tprime(left);
}

node* tprime(node* left) {
    if (t[i] == "*") {
        i++;
        node* right = f();

        if (right == NULL) return NULL;

        node* n = new node("*", left, right);
        return tprime(n);
    }

    if (t[i] == "/") {
        i++;
        node* right = f();

        if (right == NULL) return NULL;

        node* n = new node("/", left, right);
        return tprime(n);
    }

    return left;
}

node* f() {
    if (t[i] == "(") {
        i++;

        node* n = e();

        if (n == NULL || t[i] != ")") {
            error = true;
            return NULL;
        }

        i++;
        return n;
    }

    if (t[i] == "id" || t[i] == "num") {
        node* n = new node(t[i]);
        i++;
        return n;
    }

    error = true;
    return NULL;
}

void tokenize(string s) {
    for (int j = 0; j < s.length(); j++) {
        if (isspace(s[j])) continue;

        if (isalnum(s[j])) {
            string word = "";

            while (j < s.length() && isalnum(s[j])) {
                word += s[j];
                j++;
            }

            j--;
            t.push_back(word);
        }
        else {
            t.push_back(string(1, s[j]));
        }
    }

    t.push_back("$");
}

double evaluate(node* n) {
    if (n->data == "id")
        return val["id"];

    if (n->data == "num")
        return val["num"];

    double a = evaluate(n->left);
    double b = evaluate(n->right);
    
    if (n->data == "+") return a + b;
    if (n->data == "-") return a - b;
    if (n->data == "*") return a * b;
    if (n->data == "/") return a / b;

    /*if (n->data == "=") {
        val["id"] = b;
        return b;
    }*/

    return 0;
}

void display(node* n, int space = 0) {
    if (n == NULL) return;

    for (int j = 0; j < space; j++)
        cout << "   ";

    cout << n->data << endl;

    display(n->left, space + 1);
    display(n->right, space + 1);
}

int main() {
    string s;
    double idvalue, numvalue;

    cout << "Enter expression: ";
    getline(cin, s);

    cout << "Enter id value: ";
    cin >> idvalue;

    cout << "Enter num value: ";
    cin >> numvalue;

    val["id"] = idvalue;
    val["num"] = numvalue;

    tokenize(s);

    node* root = e();

    if (error || root == NULL || t[i] != "$") {
        cout << "Invalid Expression";
    }
    else {
        cout << "Valid Expression\n";

        cout << "\nAST Tree:\n";
        display(root);

        cout << "\nResult: " << evaluate(root);
    }

    return 0;
}

