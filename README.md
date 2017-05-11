# Expressions-Calculation-Project
My C++ III project in sweet Collin

Created by Xiangru Zhou

**************************************
This project is to create a program that will read statements written to match the following syntax, build data structures to contain the necessary information to use in evaluating the expressions, and evaluate the expressions as necessary. The statements will be found in a file that will be passed to the program via a command line.

STATEMENTS:

Valid statements are of one of the following three forms:

1) EVAL;

2) VALUE Variable Sign Constant ; 

3) EXP Expression;

Where:

Variable is of the form: Vnn where nn ranges from 00 to 99. 

Sign is optional and is either a plus ‘+’ or minus ‘-‘.

Expression is a mathematical expression using variables and/or constants (optional signs) separated by math operators “+-*/” and optionally using parentheses “()” to contain an operator and two operands.


Statements may extend over more than one line.
