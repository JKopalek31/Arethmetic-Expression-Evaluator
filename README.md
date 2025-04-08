# Arithmetic Expression Evaluator

## Overview
The **Arithmetic Expression Evaluator** is a C++ program that allows users to evaluate arithmetic expressions in prefix notation. It supports various arithmetic operations, variable assignments, and the ability to convert infix expressions into prefix notation. The program also includes error handling, memory management for variables, and case-sensitive variable names.

## Features
- **Arithmetic Operations**: 
  - Supports basic arithmetic operations: `+`, `-`, `*`, `/`, `^`, and `%`.
- **Variable Assignment**:
  - Assign variables using the `LET` or `let` keyword (e.g., `LET x = 5`).
- **List Variables**:
  - List all stored variables using the `VARS` or `vars` command.
- **Clear Variables**:
  - Clear variables from memory using the `CLEAR` or `clear` command.
- **Case-Sensitive Variables**:
  - Variable names are case-sensitive (e.g., `x` and `X` are treated as different variables).
- **Parentheses Handling**:
  - Supports parentheses for enforcing operator precedence.
- **Infix to Prefix Conversion**:
  - Convert infix expressions to prefix notation.
- **Prefix Expression Evaluation**:
  - Evaluate prefix expressions with built-in error handling.
- **Exit Program**:
  - Enter `$$` to exit the program.

## Usage
1. **Evaluate Arithmetic Expressions**: 
   - Enter arithmetic expressions in prefix notation, and the evaluator will return the result.
2. **Assign Variables**: 
   - Use the `LET` or `let` keyword to assign values to variables (e.g., `LET x = 10`).
3. **Listing Variables**: 
   - Type `VARS` or `vars` to list all variables currently stored in memory.
4. **Clear Variables**: 
   - Use `CLEAR` or `clear` to remove all stored variables.
5. **Exit Program**: 
   - To exit the program, type `$$`.

## Example Commands
```txt
LET x = 5
LET y = 10
VARS
( x + y ) *
