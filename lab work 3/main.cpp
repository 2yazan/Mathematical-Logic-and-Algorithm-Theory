// Develop a program implementing the resolution method for propositional logic. The program reads several formulas-premises
// and a consequence, the truth of which must be proven. On the screen, the program displays not only the result of the proof,
// but also an explanation of the proof, namely the original set of disjuncts and the gluings performed.

#include "logicmath.h"

// Convert a sequence of tokens,
// representing an expression in infix notation,
// to a sequence of tokens representing
// the expression in reverse Polish notation
// (Dijkstra's algorithm "Shunting Yard")

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
// Print the sequence of tokens
void printSequence(Queue q) {
    while (!q.empty()) {
        std::cout << q.front();
        q.pop();
    }
    std::cout << std::endl;
}

// Convert a string with an expression into a sequence of tokens
// (lexical analyzer)
Queue stringToSequence(const String& s) {
    Queue res;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!isSpace(s[i])) {
            res.push(toUpperCase(s[i]));
        }
    }
    return res;
}


// Extract variables from the sequence of tokens
unsigned getVariables(Queue s, Set& res) {
    unsigned c = 0;
    while (!s.empty()) {
        if (isVariable(s.front()) && res.count(s.front()) == 0) {
            res.insert(s.front());
            c++;
        }
        s.pop();
    }
    return c;
}

// Get variable values from the keyboard
Map inputVarValues(const Set& var) {
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

// Replace variables with their values
Queue substValues(Queue expr, Map& varVal) {
    Queue res;
    while (!expr.empty()) {
        if (isVariable(expr.front())) {
            res.push(varVal[expr.front()]);
        }
        else {
            res.push(expr.front());
        }
        expr.pop();
    }
    return res;
}

// Calculate the value of an operation, modifying the stack.
// The result is placed in the stack
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


// Calculate the value of an expression recorded in reverse Polish notation 
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

// Output the result of the calculation to the screen 
void printResult(Token r) {
    assert(isNumber(r));
    std::cout << "Value of the expression: " << r << std::endl;
}

// Construct the matrix of CNF formulas 
int SKNF(Queue expr, int** matr, Set vars, unsigned countVars) {
    unsigned mask;
    unsigned lim = 1 << countVars; // Calculate limit based on the number of variables
    unsigned c = 0;

    // Iterate through all possible combinations of truth values
    for (size_t i = 0; i < lim; i++) {
        Map varVals;
        mask = lim;
        Set::const_iterator k = vars.begin(); // Iterator for the variable set
        
        for (size_t j = 0; j < countVars; j++) { // Declare j here
            mask >>= 1; // Right shift mask to get the current truth value
            bool t = i & mask; // Determine the truth value for the current variable
            Token T = boolToToken(t); // Convert boolean to Token
            varVals.insert(VarVal(*k, T)); // Insert into variable values map
            
            k++; // Move to the next variable
        }

        Queue sExpr = substValues(expr, varVals); // Substitute values in the expression
        Token res = evaluate(sExpr); // Evaluate the substituted expression

        // If the result is false, update the matrix
        if (res == '0') {
            mask = lim;
            for (size_t j = 0; j < countVars; j++) { // Declare j here
                mask >>= 1;
                bool t = i & mask;
                if (t == true) {
                    (*matr)[j] = -1; // Set to -1 if the variable is true
                } else {
                    (*matr)[j] = 0; // Set to 0 if the variable is false
                }
                matr++;
                c++;
            }
        }
    }
    return c; // Return the number of clauses generated

}


// Check if the resolvent is empty 
bool EmptySequence(int* a, unsigned countVars) { 
    for (unsigned i = 0; i < countVars; i++)
        if (a[i] != 1)
            return false;
    return true;
}

// Check for equality of arrays 
bool EqvivArr(int* a, int* b, unsigned countVars) { 
    for (int i = 0; i < countVars; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

// Search for an array in the matrix 
bool SearchArr(int** matr, int n, unsigned countVars, int* a) { 
    for (int i = n - 1; i >= 0; i--) {
        if (EqvivArr(matr[i], a, countVars)) 
            return true;
    }
    return false;
}

// Write to the matrix of disjunctions 
void WriteArr(int** matr, int* n, unsigned countVars, int* a) { 
    for (int i = 0; i < countVars; i++) {
        matr[*n][i] = a[i];
    } 
    (*n)++;
}

// Output the disjunction 
void PrintfArr(int** matr, int n, unsigned countVars, Set vars) { 
    std::set<int>::iterator it;
    std::cout << "(";
    for (int i = 0, it = *vars.begin(); i < countVars; i++, it++) 
        if (matr[n][i] != 1) {
            if (matr[n][i] == -1)
                std::cout << '-' << (Token)(it);
            else
                std::cout << (Token)(it);
            std::cout << " | ";
        }
    std::cout << "\b\b\b)";
}

// Resolution method 
bool MethodResolution(int** matr, int n, unsigned countVars, Set vars) { 
    int k = n, tk = n;
    int* a = new int[countVars];
    const time_t TIMEOUT_LIMIT = 100; // Time limit allocated for finding a solution 
    time_t start = time(NULL); // Start time for finding a solution 
    bool is_solved = false;

    while (time(NULL) - start < TIMEOUT_LIMIT && !is_solved) { 
        for (int i = 0; i < k - 1 && !is_solved; i++)
            for (int j = i + 1; j < k && !is_solved; j++) { 
                for (int z = 0; z < countVars; z++) {
                    if (matr[i][z] == 0 && matr[j][z] == -1 || matr[i][z] == -1 && matr[j][z] == 0) 
                        a[z] = 1;
                    else if (matr[i][z] == 0 || matr[j][z] == 0) 
                        a[z] = 0;
                    else if (matr[i][z] == -1 && matr[j][z] == -1) 
                        a[z] = -1;
                    else if (matr[i][z] == 1 && matr[j][z] == 1) 
                        a[z] = 1;
                }
                is_solved = EmptySequence(a, countVars); 
                if (is_solved) {
                    std::cout << "Performing unification "; 
                    PrintfArr(matr, i, countVars, vars); 
                    std::cout << " and ";
                    PrintfArr(matr, j, countVars, vars); 
                    std::cout << ": empty resolvent\n"; 
                    return true;
                } else {
                    if (!SearchArr(matr, tk, countVars, a)) { 
                        WriteArr(matr, &tk, countVars, a); 
                        std::cout << "Performing unification "; 
                        PrintfArr(matr, i, countVars, vars); 
                        std::cout << " and ";
                        PrintfArr(matr, j, countVars, vars); 
                        std::cout << ": ";
                        PrintfArr(matr, tk, countVars, vars); 
                        std::cout << " \n";
                    }
                }
            }
        k = tk;
    }
    return is_solved;
}

// Output the set of disjuncts
void PrintfSetDis(int** matr, int n, int countVars, Set vars) {
    std::cout << "{";
    for (int i = 0; i < n; i++) {
        PrintfArr(matr, i, countVars, vars);
        std::cout << ", ";
    }
    std::cout << "\b\b}"; // Remove the last comma and space
}

// Main program
int main() {
    try {
        std::cout << "Enter the number of premises: ";
        int n;
        String expres;
        std::cin >> n;
        getchar(); // Clear input
        expres += "-(";

        // Input formulas
        String exp;
        for (int i = 0; i < n - 1; i++) {
            exp = inputExpr();
            expres += '(' + exp + ")&";
        }
        exp = inputExpr();
        expres += '(' + exp + ')'; 
        expres += ")>";

        std::cout << "Enter the consequence to check\n"; 
        exp = inputExpr();
        expres = expres + '(' + exp + ")"; 
        expres += ")";

        // Convert expression to a sequence of tokens
        Queue input = stringToSequence(expres);
        
        // Convert the sequence of tokens to postfix notation (RPN)
        Queue output = infixToPostfix(input);

        Set vars;
        unsigned countVars = getVariables(output, vars);
        unsigned t = 1 << countVars;

        // Create matrix
        int** matr = new int*[3 * t];
        for (int i = 0; i < 3 * t; i++) {
            matr[i] = new int[countVars];
        }

        // Build the CNF matrix
        unsigned k = SKNF(output, matr, vars, countVars);

        std::cout << "Set of disjuncts:\n"; 
        PrintfSetDis(matr, k, countVars, vars); 
        std::cout << "\n";

        bool isSolved = MethodResolution(matr, k, countVars, vars); 
        if (isSolved)
            std::cout << "An empty resolvent has been obtained; the theorem is proven\n";
        else
            std::cout << "It is impossible to obtain an empty resolvent; the theorem is disproven\n";
        
        // Delete the matrix
        for (int i = 0; i < 3 * t; i++) { 
            delete[] matr[i];
        }
        delete[] matr;
        
    }
    catch (const String& err) {
        // If an error occurs, print the message
        printErrorMessage(err);
        // And exit the program with an unsuccessful exit code
        exit(1);
    }
    // End of the program
    
    return 0;
}

