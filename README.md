# CPSC323-Assignment1-LexicalAnalyzer

## Problem Statement
Write a lexical analyzer using a FSM for identifiers, integers, and real numbers. 
The program should read a file containing the source code given from class to generate 
tokens and write out the results to an output file. 

## How to run the Program
This program was developed in a Linux environment. To compile and run the program with an 
executable file called "lexer", invoke the GNU C compiler with the command: 

gcc -o lexer assignment1.c

Run the program using the command ./lexer  and you will be prompted to enter the filename 
of the source code i.e. SampleInputFile1.txt.

After entering the filename, the program will run a lexical analysis of the source code 
file and write the token and lexeme output into a new file called output.txt.

## Design of the Program
The program generates a C source of a lexer that reads input from a file,
moves character by character to the next appropriate state in the FSM,
and matches the input with regular expressions to recognize tokens for each lexeme.

It uses an enumeration called FsmState to define the different states of the FSM. 
In total there are 16 states defined as: START, ID_START, IN_ID, ID_END, KEYWORD_END, 
NUM_START, IN_NUM, IN_REAL, INT_END, REAL_END, COMMENT_START, IN_COMMENT, END_COMMENT, 
OPERATOR, OPERATOR_END, SEPARATOR, where the names that contain the word "END" are the 
accepting states for a token. The program uses a loop that is defined with the condition 
that while obtaining the next character from a specified file stream is not end of file, 
run through the lexer to convert the sequence of characters to the corresponding token 
according to the matching regular expressions.

In the provided text files, the program identifies tokens and their corresponding lexemes 
from test.txt and writes this information to a new file called output.txt.
