// The program must read the formula of propositional logic in the specified normal form. 
// Algorithms that solve the problem must be contained in a separate module.
// The program must build a complete truth table of the entered formula.
// The program must prove the inconsistency of the entered formula

#include "mathlogic.h"
#include <windows.h>

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
                std::cerr << "Enter 0 or 1!" << std::endl;
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
    // If the operation is binary:
    if (isBinOp(op)) {
        // The stack should have two operands
        if (s.size() >= 2) {
            // If so, extract the right operand (number)
            Token b = s.top();
            if (!isNumber(b)) {
                throw String("Invalid expression!");
            }
            s.pop();
            // Then extract the left operand (number)
            Token a = s.top();
            if (!isNumber(a)) {
                throw String("Invalid expression!");
            }
            s.pop();
            // Push the result of the operation into the stack
            s.push(evalBinOp(a, op, b));
        } else {
            throw String("Invalid expression!");
        }
    } else if (isUnarOp(op) && !s.empty()) { // Otherwise, if it's a unary operation
        // Extract the operand
        Token a = s.top();
        if (!isNumber(a)) {
            throw String("Invalid expression!");
        }
        s.pop();
        // Push the result of the operation into the stack
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
        // If it's a number:
        if (isNumber(t)) {
            // Push it into the stack
            s.push(t);
        } else if (isOperation(t)) { // If it's an operation:
            // Evaluate it, modifying the stack
            // (the result is also placed in the stack)
            evalOpUsingStack(t, s);
        }
    }
    // The result is the single element in the stack
    if (s.size() == 1) {
        // Return the result
        return s.top();
    } else {
        throw String("Invalid expression!");
    }
}

void printResult(Token r) {
    assert (isNumber(r));
    std::cout << "Expression value: " << r << std::endl;
}

void output_array(std::vector<Token> a, int n) {
    std::cout << " ";
    for (int i = 0; i < n; i++)
        printf(" %c |", a[i]);
}

void printTruthTableInner(Queue output, Set variables, int i, int n) {
    static std::vector<Token> D(100, '0');
    for (int j = '0'; j <= '1'; j++) {
        D[i] = j;
        if (i == n - 1) {
            output_array(D, n);
            // Calculate the value of the expression:
            Map varVal;
            int k = 0; // Using D (vector of 1's and 0's)
            for (Set::const_iterator x = variables.begin(); x != variables.end(); ++x) { // For variable names
                varVal.insert(VarVal(*x, D[k]));
                k++;
            }
            Queue rpn = substValues(output, varVal);
            Token res = evaluate(rpn);
            printf(" %c\n", res);
        } else
            printTruthTableInner(output, variables, i + 1, n);
    }
}

void printTruthTable(Queue input) {
    // Get the set of all variables from the original formula
    Set variables = getVariables(input);
    for (auto var: variables)
        printf(" %c |", var);
    printSequence(input);
    std::cout << std::endl;
    // Convert the token sequence to postfix notation
    Queue output = infixToPostfix(input);
    printTruthTableInner(output, variables, 0, variables.size());
}

void output_vector(std::vector<Token> a, int n) {
    printf("{");
    for (int i = 0; i < n; i++)
        printf("%c, ", a[i]);
    printf("\b\b}\n");
}

bool isValidInner(Queue output, Set variables, int i, int n) {
    static std::vector<Token> D(100, '0');
    static int n_truth = 0;
    for (int j = '0'; j <= '1'; j++) {
        D[i] = j;
        if (i == n - 1) {
            // Calculate the value of the expression:
            Map varVal;
            int k = 0; // Using D (vector of 1's and 0's)
            for (Set::const_iterator x = variables.begin(); x != variables.end(); ++x, ++k) {
                varVal.insert(VarVal(*x, D[k]));
            }
            Queue rpn = substValues(output, varVal);
            Token res = evaluate(rpn);
            if (res == '1')
                n_truth++;
        } else
            isValidInner(output, variables, i + 1, n);
    }
    return n_truth == (1 << n);
}

bool isValid(Queue output) {
    Set variables = getVariables(output);
    return isValidInner(output, variables, 0, variables.size());
}

void printTrueVectorsInner(Queue output, Set variables, int i, int n) {
    static std::vector<Token> D(100, '0');
    for (int j = '0'; j <= '1'; j++) {
        D[i] = j;
        if (i == n - 1) {
            //calculating the value of the expression:
            Map varVal;
            int k = 0; //by the values ​​of D (vector 1c and 0y)
            for (Set::const_iterator x = variables.begin(); x != variables.end(); ++x, ++k)
                varVal.insert(VarVal(*x, D[k]));

            Queue rpn = substValues(output, varVal);
            Token res = evaluate(rpn);
            if (res == '1')
                output_vector(D, n);
        } else
            printTrueVectorsInner(output, variables, i + 1, n);
    }
}

void printTrueVectors(Queue output) {
    Set variables = getVariables(output);
    printTrueVectorsInner(output, variables, 0, variables.size());
}

int main() {
    SetConsoleOutputCP(CP_UTF8);//Подключение русского языка
    std::string expr = inputExpr();
    Queue input = stringToSequence(expr);
    try{
        printTruthTable(input);
        Queue output = infixToPostfix(input);
        if (isValid(output)) {
            std::cout << "The formula is valid because it is true under all interpretations.";
        } else {
            std::cout << "The formula is invalid because it is not true in all interpretations..\n";
        }
    } catch (const String &err){
        printErrorMessage(err);
        exit(1);
    }
    return 0;
}
