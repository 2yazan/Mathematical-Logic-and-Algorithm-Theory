#ifndef MATHLOGIC
#define MATHLOGIC

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <cassert>
#include <cstdlib>
#include <vector>

// Declaration of types.
// Token (lexeme):
typedef char Token;
// Stack of tokens:
typedef std::stack<Token> Stack;
// Sequence of tokens:
typedef std::queue<Token> Queue;
// Set of unique tokens:
typedef std::set<Token> Set;
// Table of variable values:
typedef std::map<Token, Token> Map;
// Pair of variable-value:
typedef std::pair<Token, Token> VarVal;
// String of characters:
typedef std::string String;

// Is the token a number?
inline bool isNumber(Token t) {
    return t == '0' || t == '1';
}

// Is the token a variable?
inline bool isVariable(Token t) {
    return (t >= 'A' && t <= 'Z') || (t >= 'a' && t <= 'z');
}

// Is the token an operation?
inline bool isOperation(Token t) {
    return (t == '|' || t == '&' || t == '-' || t == '>' || t == '~');
}

// Is the token an opening parenthesis?
inline bool isOpeningPar(Token t) {
    return t == '(';
}

// Is the token a closing parenthesis?
inline bool isClosingPar(Token t) {
    return t == ')';
}

// Return the priority of the operation
// (the larger the number, the higher the priority)
inline int priority(Token op) {
    assert (isOperation(op));
    int res = 0;
    switch (op) {
        case '-':
            // Negation — highest priority
            res = 5;
            break;
        case '&':
            // Conjunction
            res = 4;
            break;
        case '|':
            // Disjunction
            res = 3;
            break;
        case '>':
            // Implication
            res = 2;
            break;
        case '~':
            // Equivalence — lowest priority
            res = 1;
            break;
    }
    return res;
}

// Convert the sequence of tokens representing an expression in infix notation
// into a sequence of tokens representing the expression in postfix notation
// (Dijkstra's "Shunting Yard" algorithm)
Queue infixToPostfix(Queue input);

// Print the sequence of tokens
void printSequence(Queue q);

// Is the character a whitespace character?
inline bool isSpace(char c) {
    return c <= ' ';
}

// If the character is a lowercase letter, convert it to uppercase,
// otherwise just return the same character
inline char toUpperCase(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    } else {
        return c;
    }
}

// Convert a string with an expression into a sequence of tokens
// (lexical analyzer)
Queue stringToSequence(const String &s);

// Print an error message
inline void printErrorMessage(const String &err) {
    std::cerr << "*** ERROR! " << err << std::endl;
}

// Enter an expression from the keyboard
inline String inputExpr() {
    String expr;
    std::cout << "Propositional logic formula: ";
    std::getline(std::cin, expr);
    return expr;
}

// Extract variables from the sequence of tokens
Set getVariables(Queue s);

// Get the values of variables from the keyboard
Map inputVarValues(const Set &var);

// Replace variables with their values
Queue substValues(Queue expr, Map &varVal);

// Is the operation binary?
inline bool isBinOp(Token t) {
    return t == '&' || t == '|' || t == '>' || t == '~';
}

// Is the operation unary?
inline bool isUnarOp(Token t) {
    return t == '-';
}

// Get the boolean value of a token-number (true or false)
inline bool logicVal(Token x) {
    assert (isNumber(x));
    return x == '1';
}

// Convert a boolean value to a token-number
inline Token boolToToken(bool x) {
    if (x) {
        return '1';
    } else {
        return '0';
    }
}

// Calculate the result of a binary operation
inline Token evalBinOp(Token a, Token op, Token b) {
    assert (isNumber(a) && isBinOp(op) && isNumber(b));
    bool res;
    // Get the boolean values of the operands
    bool left = logicVal(a);
    bool right = logicVal(b);
    switch (op) {
        case '&':
            // Conjunction
            res = left && right;
            break;
        case '|':
            // Disjunction
            res = left || right;
            break;
        case '>':
            // Implication
            res = !left || right;
            break;
        case '~':
            // Equivalence
            res = (!left || right) && (!right || left);
            break;
    }
    return boolToToken(res);
}

// Calculate the result of a unary operation
inline Token evalUnarOp(Token op, Token a) {
    assert (isUnarOp(op) && isNumber(a));
    bool res = logicVal(a);
    switch (op) {
        case '-':
            // Negation
            res = !res;
            break;
    }
    return boolToToken(res);
}

// Calculate the value of an operation by modifying the stack.
// The result is placed on the stack
void evalOpUsingStack(Token op, Stack &s);

// Calculate the value of an expression written in postfix notation
Token evaluate(Queue expr);

// Display the calculation result on the screen
void printResult(Token r);

// Display all values in vector `a` up to the nth element
void output_array(std::vector<Token> a, int n);

// Display the truth table for the formula `input`
void printTruthTable(Queue input);

// Determine if the formula in postfix notation `output` is valid
// (identically true) on all interpretations
bool isValid(Queue output);

// Display all vectors for the formula in postfix notation `output`,
// consisting of all possible combinations of values of its variables,
// for which the formula evaluates to true
void printTrueVectors(Queue output);

#endif // MATHLOGIC
