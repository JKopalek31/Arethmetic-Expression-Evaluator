/**
 * Jett Kopalek
 * CSCI 461
 * Arithmetic Expression Evaluator
 * This program allows users to evaluate arithmetic expressions and assign variables to use to 
 * evalate an expression in prefix notation. It supports:
 * 
 * - Arithmetic operations: +, -, *, /, ^, and %
 * - Multi-digit numbers and negative numbers
 * - Variable assignment using the `LET` or 'let' keyword
 * - List variables using the 'VARS' or 'vars' command
 * - Clear variables from memory using the 'CLEAR' or 'clear' command 
 * - Case-sensitive variable names (`x` and `X` are treated as different variables)
 * - Parentheses for enforcing operator precedence
 * - Conversion of infix expressions to prefix notation
 * - Evaluation of prefix expressions with error handling
 * - Enter `$$` to exit the program.
 */

//Imports
#include <iostream> //For user input
#include <string> //For string manipulation
#include <stack> //For prefix evaluation
#include <unordered_map> //For storing variables
#include <cmath> //For math commands mod and exponent
#include <algorithm> //For reverse string
#include <sstream> //For traversing through tokens
#include <cctype> //For easy type checking
#include <vector> //For dynamic strings 

//Set standard namespace
using namespace std;

//Store variable assignment
unordered_map<string, double> variables;  

//Returns precedence of an operator
int precedence(char op) {
    switch (op) {
        case '^': return 3;
        case '*': case '/': case '%': return 2;
        case '+': case '-': return 1;
    }
    return 0;
}
//Check if a charachtor is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

//Tokenize expression to handle all kinds of numbers
vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    int openParens = 0, closeParens = 0; 

    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        if (isalnum(c) || (c == '-' && (i == 0 || isOperator(expression[i - 1]) || expression[i - 1] == '(' || isspace((unsigned char)expression[i - 1])))) {
            token += c;

        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (isOperator(c) || c == '(' || c == ')') {
                tokens.push_back(string(1, c));
                if (c == '(') openParens++;
                if (c == ')') closeParens++;
            } else if (!isspace(c)) {
                cerr << "Invalid operand or unknown variable: " << c << endl;
                exit(1);
            }
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }

    if (openParens != closeParens) {
        cerr << "Error: Mismatched parentheses" << endl;
        exit(1);
    }

    return tokens;
}

//Substitution method to substitute variables into expressions 
vector<string> substituteVariables(const vector<string>& tokens) {
    vector<string> substitutedTokens;
    for (const string& token : tokens) {
        // If token is a known variable, substitute its value
        if (variables.find(token) != variables.end()) {
            substitutedTokens.push_back(to_string(variables[token]));
        } 
        // If it's alphabetic but not in our variable map, it's unknown
        else if (isalpha(token[0])) {
            cerr << "Error: Unknown variable '" << token << "'" << endl;
            exit(1);
        } 
        else {
            substitutedTokens.push_back(token);
        }
    }
    return substitutedTokens;
}


//Convert infix to prefix notation
string infixToPrefix(const string& infixExpression) {
    vector<string> tokens = tokenize(infixExpression);
    tokens = substituteVariables(tokens);

    //Reverse the token
    reverse(tokens.begin(), tokens.end());

    //Swap parentheses
    for (string & token : tokens) {
        if (token == "(") token = ")";
        else if (token == ")") token = "(";
    }

    //Initialize stacks
    stack<string> prefixStack;
    stack<string> opStack;

    //Evalute tokens
    for (const string &token : tokens) {
        // If it's a number (or negative number)
        if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
            prefixStack.push(token);
        }
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                prefixStack.push(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
        else if (isOperator(token[0])) {
            while (!opStack.empty() && precedence(opStack.top()[0]) >= precedence(token[0])) {
                prefixStack.push(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        prefixStack.push(opStack.top());
        opStack.pop();
    }

    //Build prefix string
    string prefix;
    while (!prefixStack.empty()) {
        prefix += prefixStack.top() + " ";
        prefixStack.pop();
    }

    return prefix;
}

//Perform operations
double performOperation(char op, double operand1, double operand2) {
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 == 0) {
                cerr << "Error: Division by zero!" << endl;
                exit(1);
            }
            return operand1 / operand2;
        //EXTRA CREDIT MODIFICATION FOR MOD OPERATIONS
        case '%':
            if (operand2 == 0) {
                cerr << "Error: Modulo by zero!" << endl;
                exit(1);
            }
            return fmod(operand1, operand2);
        case '^': return pow(operand1, operand2);
        default:
            cerr << "Invalid operator: " << op << endl;
            exit(1);
    }
}

//Evaluate prefix expression
double evaluatePrefix(const string& expression) {
    //Initialize data structures
    vector<string> tokens;
    stringstream ss(expression);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    //Reverse tokens for better processing
    reverse(tokens.begin(), tokens.end());
    stack<double> operandStack;

    for (const string &token : tokens) {
        //Check if is variable 
        if (variables.find(token) != variables.end()) {
            operandStack.push(variables[token]);
        }
        //Check if is a number
        else if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
            operandStack.push(stod(token));
        }
        //Check if is a operator
        else if (token.size() == 1 && isOperator(token[0])) {
            if (operandStack.size() < 2) {
                cerr << "Invalid expression: not enough operands!" << endl;
                exit(1);
            }
            double operand1 = operandStack.top(); operandStack.pop();
            double operand2 = operandStack.top(); operandStack.pop();
            double result = performOperation(token[0], operand1, operand2);
            operandStack.push(result);
        }
        else {
            cerr << "Invalid operand or unknown variable: " << token << endl;
            exit(1);
        }
    }

    if (operandStack.size() != 1) {
        cerr << "Error: Malformed prefix expression!" << endl;
        exit(1);
    }

    return operandStack.top();
}

//Handle variable declarations
bool handleVariableOperations(const string &originalInput) {
    string upperInput = originalInput;
    transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);

    //Check vars command
    if (upperInput == "VARS") {
        if (variables.empty()) {
            cout << "No variables stored.\n";
        } else {
            cout << "Stored variables:\n";
            for (const auto& entry : variables) {
                cout << entry.first << " = " << entry.second << endl;
            }
        }
        return true;
    }

    //Check clear command
    if (upperInput == "CLEAR") {
        variables.clear();
        cout << "All variables cleared.\n";
        return true;
    }

    //Check let command
    if (upperInput.rfind("LET ", 0) == 0) {
        // Find '=' in the original string
        size_t equalPos = originalInput.find("=");
        if (equalPos == string::npos) {
            return false; 
        }
       
        //Get variable name's start value
        size_t varNameStart = 3; 
        //Skip spaces for flexability
        while (varNameStart < originalInput.size() && isspace((unsigned char)originalInput[varNameStart])) {
            varNameStart++;
        }

        //Get full variable name and trim white spaces
        string varName = originalInput.substr(varNameStart, equalPos - varNameStart);
        while (!varName.empty() && isspace((unsigned char)varName.back())) {
            varName.pop_back();
        }

        //Get the equivalence
        string expression = originalInput.substr(equalPos + 1);

        //Evaluate the expression if the equivalence is an expression
        string prefixExpression = infixToPrefix(expression);
        double result = evaluatePrefix(prefixExpression);

        //Store variable
        variables[varName] = result;

        cout << "\tVariable " << varName << " = " << result << endl;
        return true;
    }
    return false;
}

//Prints menu
void printMenu(){
    cout << "Arethmetic Expression Evaluater"<<endl;
    cout << "Enter infix expressions (e.g., '2 + 3 * 4' or 'x + y')."<<endl;
    cout << "Supports operators: +, -, *, /, ^, %"<<endl;
    cout << "Assign variables with 'LET <var> = <exp>' (e.g., 'LET x = 5')."<<endl;
    cout << "View variables with 'VARS' and clear variables with 'CLEAR'"<<endl;
    cout << "Arbitrary spaces are allowed. Enter '$$' to exit"<<endl;
}

//Main method
int main() {
    printMenu();
    while (true) {
        cout << "> ";
        string userString;
        getline(cin, userString);

        //Exit condition
        if (userString == "$$") {
            return 0;
        }

        //Handle user input
        if (!handleVariableOperations(userString)) {
            string prefix = infixToPrefix(userString);
            cout << "\tPrefix: " << prefix << endl;
            cout << "\tEvaluated Result: " << evaluatePrefix(prefix) << endl;
        }
    }
}
