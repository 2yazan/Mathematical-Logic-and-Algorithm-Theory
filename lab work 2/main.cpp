/* Develop a program that can read several formulas-premises of propositional
logic and display on the screen all formulas-consequences from these premises. */

#include "mathlogic.h"
#include <windows.h>

using namespace std;

Queue infixToPostfix(Queue input) {
    // Output sequence (output queue):
    Queue output;
    // Working stack:
    Stack s;
    // Current input token:
    Token t;
    while (!input.empty()) {
        // Get token from the beginning of the input sequence
        t = input.front();
        input.pop();
        if (isNumber(t) || isVariable(t)) {
            output.push(t);
        } else if (isOperation(t)) { // If the token is an operation op1, then:
            // While the top of the stack contains an operation token op2
            // and op1's priority is less than or equal to op2's priority:
            while (!s.empty() && isOperation(s.top())
                   && priority(t) <= priority(s.top())
                    ) {
                // Move op2 from the stack to the output queue
                output.push(s.top());
                s.pop();
            }
            // Push op1 to the stack
            s.push(t);
        } else if (isOpeningPar(t)) { // If the token is an opening parenthesis:
            // Push it to the stack
            s.push(t);
        } else if (isClosingPar(t)) { // If the token is a closing parenthesis:
            // While the top of the stack is not an opening parenthesis:
            while (!s.empty() && !isOpeningPar(s.top())) {
                // Move operation tokens from the stack
                // to the output queue
                assert (isOperation(s.top()));
                output.push(s.top());
                s.pop();
            }
            // If the stack ended before an opening parenthesis was encountered:
            if (s.empty()) {
                // An opening parenthesis is missing in the expression
                throw String("Missing opening parenthesis!");
            } else {
                // Otherwise, discard the opening parenthesis from the stack
                // (but do not add it to the output queue)
                s.pop();
            }
        } else {
            // In other cases, the input sequence
            // contains a token of unknown type
            String msg("Unknown symbol \'");
            msg += t + String("\'!");
            throw msg;
        }
    }
    // No more tokens on input, but there may still be tokens in the stack.
    // While the stack is not empty:
    while (!s.empty()) {
        // If the token at the top of the stack is an opening parenthesis:
        if (isOpeningPar(s.top())) {
            // There is an unclosed parenthesis in the expression
            throw String("Unclosed parenthesis!");
        } else {
            // Otherwise, move the operation token from the stack to the output queue
            assert (isOperation(s.top()));
            output.push(s.top());
            s.pop();
        }
    }
    // End of algorithm.
    // Return the resulting sequence
    return output;
}

void printSequence(Queue q) {
    while (!q.empty()) {
        std::cout << q.front();
        q.pop();
    }
}

Queue stringToSequence(const String &s) {
    Queue res;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!isSpace(s[i])) {
            res.push(toUpperCase(s[i]));
        }
    }
    return res;
}

Set getVariables(Queue s) {
    Set res;
    while (!s.empty()) {
        if (isVariable(s.front()) && res.count(s.front()) == 0) {
            res.insert(s.front());
        }
        s.pop();
    }
    return res;
}

Map inputVarValues(const Set &var) {
    Token val;
    Map res;
    for (Set::const_iterator i = var.begin(); i != var.end(); ++i) {
        do {
            std::cout << *i << " = ";
            std::cin >> val;
            if (!isNumber(val)) {
                std::cerr << "Введите 0 или 1!" << std::endl;
            }
        } while (!isNumber(val));
        res.insert(VarVal(*i, val));
    }
    return res;
}

Queue substValues(Queue expr, Map &varVal) {
    Queue res;
    while (!expr.empty()) {
        if (isVariable(expr.front())) {
            res.push(varVal[expr.front()]);
        } else {
            res.push(expr.front());
        }
        expr.pop();
    }
    return res;
}
void evalOpUsingStack(Token op, Stack &s) {
    assert (isOperation(op));
    // If the operation is binary, then:
    if (isBinOp(op)) {
        // There should be two operands in the stack
        if (s.size() >= 2) {
            // If so, extract the right operand-number
            Token b = s.top();
            if (!isNumber(b)) {
                throw String("Invalid expression!");
            }
            s.pop();
            // Then extract the left operand-number
            Token a = s.top();
            if (!isNumber(a)) {
                throw String("Invalid expression!");
            }
            s.pop();
            // Place the result of the operation in the stack
            s.push(evalBinOp(a, op, b));
        } else {
            throw String("Invalid expression!");
        }
        // Otherwise, the operation is unary
    } else if (isUnarOp(op) && !s.empty()) {
        // Extract the operand
        Token a = s.top();
        if (!isNumber(a)) {
            throw String("Invalid expression!");
        }
        s.pop();
        // Place the result of the operation in the stack
        s.push(evalUnarOp(op, a));
    } else {
        throw String("Invalid expression!");
    }
}

Token evaluate(Queue expr) {
    // Working stack
    Stack s;
    // Current token
    Token t;
    // While the input sequence contains tokens:
    while (!expr.empty()) {
        // Read the next token
        t = expr.front();
        assert (isNumber(t) || isOperation(t));
        expr.pop();
        // If this is a number, then:
        if (isNumber(t)) {
            // Place it in the stack
            s.push(t);
            // If this is an operation, then:
        } else if (isOperation(t)) {
            // Compute it by modifying the stack
            // (the result is also placed in the stack)
            evalOpUsingStack(t, s);
        }
    }
    // The result is the only element in the stack
    if (s.size() == 1) {
        // Return the result
        return s.top();
    } else {
        throw String("Invalid expression!");
    }
}

void printResult(Token r) {
    assert (isNumber(r));
    std::cout << "Value of the expression: " << r << std::endl;
}

// Matches each variable from var with the value from a
Map inputByArray(const Set &var, int *a) {
    Token val;
    Map res;
    for (Set::const_iterator i = var.begin(); i != var.end(); ++i) {
        val = (*a ? '1' : '0');
        a++;

        res.insert(VarVal(*i, val));
    }
    return res;
}

// Writes the truth table for the expression input
// with the set of variables vars
void getTruthTable(Queue &input, Set &vars, std::vector<std::vector<int>> &a) {
    static int D[100] = {0};
    static int i = 0;
    static int z = 0; // column number in the truth table
    for (int x = 0; x <= 1; x++) {
        D[i] = x;
        if (i == vars.size() - 1) {
            // Calculate the value of the expression:
            Map Vars = inputByArray(vars, D); // each variable corresponds to its value from D
            Token res = evaluate(substValues(input, Vars)); // calculate the value for the row of the table
            for (int k = 0; k <= i; k++)
                a[z][k] = D[k];
            a[z][vars.size()] = res == '1';
            z++;
        } else {
            i++;
            getTruthTable(input, vars, a);
            i--;
        }
    }
    if (i == 0) {
        z = 0;
    }
}

// Outputs CNF by the given set of disjuncts and all its combinations
void writeExpr(std::vector<String> &d) {
    static int D[100]; // formation of the combination
    static int i = 0;
    for (int x = 0; x <= 1; x++) {
        D[i] = x;
        if (i == d.size() - 1) {
            int end = 1;
            for (int v = 0; v < d.size(); v++) { // for all elements from the set of disjuncts
                if (D[v]) {
                    if (!end) { // if not reached the last disjunct, then output
                        std::cout << "&" << d[v];
                    } else { // if reached the end -- output the last disjunct and state that this is the end
                        end = 0;
                        std::cout << d[v];
                    }
                }
            }
            if (!end) // if there are still disjuncts
                std::cout << "\n";
        } else {
            i++;
            writeExpr(d);
            i--;
        }
    }
}

// Outputs all consequence formulas for all premise formulas found in s
void OutputConsequences(String s) {
    Queue input = stringToSequence(s);
    Queue output = infixToPostfix(input); // convert the sequence of tokens to RPN
    Set vars = getVariables(output); // set of variables
    int table_size = 1 << vars.size();
    std::vector<std::vector<int>> a(table_size); // matrix = truth table
    for (int i = 0; i < table_size; i++)
        a[i].resize(vars.size() + 1);
    getTruthTable(output, vars, a); // the truth table for the output expression is written to a
    std::vector<String> result;
    for (int i = 0; i < a.size(); i++) { // for the truth table of the CNF expression
        if (a[i][vars.size()] == 0) { // forming disjunct according to the corresponding row of the table
            String d = "(";
            if (a[i][0] == 1)
                d = d + "-";
            d = d + *(vars.begin()); // value of the first element in the set of expression elements
            auto ptr_curr = vars.begin(); // pointer to elements from the set of variables
            for (int j = 1; j < a[i].size() - 1; j++) {
                d = d + "|";
                if (a[i][j] == 1)
                    d = d + "-";

                d = d + *(++ptr_curr);
            }
            d = d + ")";
            result.push_back(d);
        }
    }
    writeExpr(result);
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // Connects to the Russian language
    std::cout << "Enter the number of premise formulas:"; // read the number of premises
    int n;
    std::cin >> n;
    String res = "(";
    String p;
    std::cout << "Enter premise formulas:";
    std::cin >> p;  // read the first premise
    res = res + p + ")";
    for (int i = 1; i < n; i++) {  // read the remaining premises
        String s;
        std::cin >> s;
        res = res + "&" + "(" + s + ")";
    }
    std::cout << "All consequence formulas:\n";  // output consequence formulas
    OutputConsequences(res);
    return 0;
}
