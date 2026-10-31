#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cctype>

using namespace std;

// Buffer size including sentinel (EOF character)
const int BUFFER_SIZE = 128;
const char SENTINEL = '\0'; // Sentinel to mark end of buffer

class TwoBufferInputSystem {
private:
    char buffer[2][BUFFER_SIZE + 1]; // Buffer pairs with extra slot for sentinel
    int activeBuffer;                // 0 or 1
    int forward;                     // Lookahead / scan pointer
    int lexeme_begin;                // Starting point of current lexeme
    int lexemeBuffer;                // Buffer index where lexeme started
    ifstream file;
    bool isFileEnded;

    unordered_set<string> keywords = {
        "int", "float", "char", "if", "else", "while", 
        "for", "return", "void", "double", "switch", "case"
    };

    void loadBuffer(int bufIndex) {
        if (isFileEnded) {
            buffer[bufIndex][0] = SENTINEL;
            return;
        }

        file.read(buffer[bufIndex], BUFFER_SIZE);
        streamsize bytesRead = file.gcount();

        // Place sentinel character at the end of valid data
        buffer[bufIndex][bytesRead] = SENTINEL;

        if (bytesRead < BUFFER_SIZE) {
            isFileEnded = true;
        }
    }

    char getNextChar() {
        char c = buffer[activeBuffer][forward++];

        // Sentinel encountered
        if (c == SENTINEL) {
            // Check if end of buffer or actual end of file
            if (isFileEnded && forward - 1 == file.gcount()) {
                return EOF;
            }

            // End of current buffer reached, switch to next buffer
            activeBuffer = 1 - activeBuffer;
            loadBuffer(activeBuffer);
            forward = 0;
            c = buffer[activeBuffer][forward++];
            
            if (c == SENTINEL) {
                return EOF;
            }
        }
        return c;
    }

    void retract(int steps = 1) {
        forward -= steps;
        if (forward < 0) {
            activeBuffer = 1 - activeBuffer;
            forward = BUFFER_SIZE - 1;
        }
    }

public:
    TwoBufferInputSystem(const string& filename) {
        file.open(filename, ios::in | ios::binary);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            exit(EXIT_FAILURE);
        }

        activeBuffer = 0;
        forward = 0;
        lexeme_begin = 0;
        lexemeBuffer = 0;
        isFileEnded = false;

        // Load the first buffer initially
        loadBuffer(0);
    }

    ~TwoBufferInputSystem() {
        if (file.is_open()) file.close();
    }

    void scanLexemes() {
        cout << "========================================================\n";
        cout << " Scanning File using Two-Buffer Input System (Sentinels)\n";
        cout << "========================================================\n";
        cout << left;
        cout.width(20); cout << "LEXEME";
        cout.width(20); cout << "TYPE" << endl;
        cout << "--------------------------------------------------------\n";

        while (true) {
            char ch = getNextChar();
            if (ch == (char)EOF) break;

            // Skip Whitespace
            if (isspace(ch)) {
                continue;
            }

            // Identifiers / Keywords recognition
            if (isalpha(ch) || ch == '_') {
                string lexeme = "";
                lexeme += ch;

                while (true) {
                    char nextCh = getNextChar();
                    if (isalnum(nextCh) || nextCh == '_') {
                        lexeme += nextCh;
                    } else {
                        retract(); // Lookahead pointer retracted
                        break;
                    }
                }

                cout.width(20); cout << lexeme;
                if (keywords.find(lexeme) != keywords.end()) {
                    cout.width(20); cout << "KEYWORD" << endl;
                } else {
                    cout.width(20); cout << "IDENTIFIER" << endl;
                }
            }
            // Number Constants
            else if (isdigit(ch)) {
                string numStr = "";
                numStr += ch;
                while (true) {
                    char nextCh = getNextChar();
                    if (isdigit(nextCh) || nextCh == '.') {
                        numStr += nextCh;
                    } else {
                        retract();
                        break;
                    }
                }
                cout.width(20); cout << numStr;
                cout.width(20); cout << "NUMBER_CONSTANT" << endl;
            }
            // Operators and Punctuations
            else {
                string symbol(1, ch);
                // Check two-character operators like ==, <=, >=, !=
                char nextCh = getNextChar();
                if ((ch == '=' || ch == '<' || ch == '>' || ch == '!') && nextCh == '=') {
                    symbol += nextCh;
                } else {
                    retract();
                }

                cout.width(20); cout << symbol;
                cout.width(20); cout << "OPERATOR / SYMBOL" << endl;
            }
        }
        cout << "========================================================\n";
    }
};

int main(int argc, char* argv[]) {
    string filename = "input.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    TwoBufferInputSystem scanner(filename);
    scanner.scanLexemes();

    return 0;
}
