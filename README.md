# Mathematical Logic and Algorithm Theory

This repository contains the solutions for the labs completed as part of the **Mathematical Logic and Algorithm Theory** course. Each lab focuses on propositional and predicate logic, formal theories, and algorithmic theories, with a practical implementation of algorithms and Brainfuck programming.

## Labs Overview

### Lab 1: Propositional Logic (Логика высказываний)
- **Task**: The program reads a formula of propositional logic in a specified normal form. The algorithms that solve the problem are implemented in a separate module.
    - The program builds a complete truth table for the entered formula.
    - The program proves the inconsistency of the entered formula.

### Lab 2: Predicate Logic (Логика предикатов)
- **Task**: Develop a program that reads several premises in propositional logic and displays all logical consequences from these premises on the screen.

### Lab 3: Formal Theories (Формальные теории)
- **Task**: Develop a program that implements the resolution method for propositional logic. The program reads several premises and a consequence, the truth of which must be proven. The output includes:
    - The result of the proof.
    - An explanation of the proof process, including the original set of disjuncts and the gluing operations performed.

### Lab 4: Algorithm Theory (Теория алгоритмов)
- **Task 1**: On the information tape of a Turing machine, three digits "1", "2", and "3" (without quotes) are written in three cells in any order. The head scans the leftmost digit. Develop a functional diagram of a Turing machine that arranges these digits in ascending order.
- **Task 2**: Implement the function `f(x) = xx'`, where `x'` is the reversal of the word `x`.
- **Task 3**: On the tape of a Post machine, there is an array of N marks. Develop a program that checks whether the number of marks is divisible by 3. If it is, place a mark after the array, leaving one empty cell in between.
- **Task 4**: Develop a program in **Brainfuck** that solves the second task of this lab. Use a Brainfuck interpreter to execute the program. Procedural extensions of the Brainfuck language are allowed for writing the program.

### Lab 5: Brainfuck Interpreter (Интерпретатор языка Brainfuck)
- **Task**: Create a Brainfuck language interpreter.

