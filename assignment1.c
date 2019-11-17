// Anne Poso
// CWID 889236576
// CPSC 323-03 Assignment 1
// Lexical Analyzer

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> // for isalpha() and isdigit()

#define BUFSIZE 100

void printToken(FILE*, const char*, const char*);
void clearBuffer(char*);
void getFile(void);
void lexer(void);
bool isOperator(char);
bool isSeparator(char);
bool isKeyword(const char*);

enum FsmState { START, ID_START, IN_ID, ID_END, KEYWORD_END,
								NUM_START, IN_NUM, IN_REAL, INT_END, REAL_END,
								COMMENT_START, IN_COMMENT, END_COMMENT,
							 	OPERATOR, OPERATOR_END, SEPARATOR };
//enum IdentifierState { ID_START, IN_ID, ID_END }; // 3 states for identifier FSM
//enum NumberState { NUM_START, IN_NUM, INT_END, IN_REAL, REAL_END }; // 5 states for integer and real FSM

char buffer[BUFSIZE]; // holds the buffer of the current lexeme
char symBuf; // holds the buffer for separators and operators
char sourceCodeFile[BUFSIZE]; //holds filename of source code to scan with lexer program

// printToken prints the input token and lexeme in table format to the fp pointing to the output text file
void printToken(FILE* fp, const char* token, const char* lexeme) {
	fprintf(fp, "%s \t=\t ", token);
	fprintf(fp, "%s\n", lexeme);
	printf("Recorded token and corresponding lexeme to output.txt file.\n");
}

void printCharToken(FILE* fp, const char* token, char lexeme) { // for printing single char separator/operator
	fprintf(fp, "%s \t=\t ", token);
	fprintf(fp, "%c\n", lexeme);
	printf("Recorded token and corresponding lexeme to output.txt file.\n");
}

void clearBuffer(char* buf) {		//clearBuffer clears the buffer containing the current lexeme
	memset(buf, '\0', sizeof(buf));
}

void getFile(void) {		// getFile gets filename of source code to test lexer
	printf("Enter source code filename: ");
	fgets(sourceCodeFile, sizeof(sourceCodeFile), stdin);
  sourceCodeFile[strcspn(sourceCodeFile, "\n")] = '\0'; // removes newline from user input
}

// parses through input source file and generates tokens for lexemes
void lexer(void) {
	FILE* fp; // file pointer to input source code file
	FILE* outputPtr; // file pointer to output text file
	outputPtr = fopen("output.txt", "w"); // open output file with write permissions
	getFile(); //opens source code file to scan with lexer
	fp = fopen(sourceCodeFile, "r");
	if (fp == NULL) {
		printf("Could not open file.\n");
		exit(1);
	} else { printf("Opened %s successfully.\n\n", sourceCodeFile); }

	enum FsmState currentState = START;
	// enum IdentifierState currentIdState = ID_START; // declares current ID state variable and initializes to starting state
	// enum NumberState currentNumState = NUM_START; // declares current number state variable and initializes to starting state
	int i = 0; //index for buffer
	int j = 0; //index for symBuf
	char ch;

	fprintf(outputPtr, "TOKENS \t\t\t LEXEMES\n"); // table header
	fprintf(outputPtr, "---------------------------------------------\n");

	while ((ch = fgetc(fp)) != EOF) { // while not at end of source file

			//**************************** using switch case for FSM of identifiers, keywords, integers, and reals

			switch (currentState) {
				case START:   // 0 - initial identifier state
				  if (ch == '\r' || ch == ' ' || ch == '\n' || ch == '\t') {
						break;		// skips any white space in beginning of buffer
					} else {
						buffer[i++] = ch; // add current character to buffer
						if(isalpha(ch) > 0) {
							currentState = ID_START; // if alphabetical character, go to state 1 identifier start
						}
				  	else if (isdigit(ch) > 0) { // if digit character, go to state 5 number start
							currentState = NUM_START;
						}
						else if (ch == '!') { // "!" signifies beginning/end of a comment, go to state 10 comment start
							currentState = COMMENT_START;
						}
						else if (isOperator(ch)) {
							currentState = OPERATOR; // if operator character, go to state 13 operator state
						}
						else if (isSeparator(ch)) { // if separator character, go to 15 separator state
							currentState = SEPARATOR;
						}
					}
					break;

				case ID_START: // 1 - initial identifier state - can only go to 2nd state
					if (isSeparator(ch) || isOperator(ch)) {
						symBuf = ch; // for encountering separators/operators at ends of identifiers with no space separator ( i.e. x=a )
						currentState = ID_END;
					}
					else {
						buffer[i++] = ch;
						currentState = IN_ID;
					}
					break;

				case IN_ID: // state 2 - can go back to 2 or to state 3 ID end accepting state
					if (isalpha(ch) > 0 || isdigit(ch) > 0 || ch == '$') {
						buffer[i++] = ch;		//legal identifiers start with alphabetical character and can include any number or '$' character
						currentState = IN_ID;
					}
					else if (isSeparator(ch) || isOperator(ch)) {
						symBuf = ch; // for encountering separators/operators at ends of identifiers with no space separator ( i.e. function(); )
						currentState = ID_END;
					}
					else { currentState = ID_END; }
					break;

				case NUM_START: // 5 - initial number state - can go to 6th state inside number
					if (isSeparator(ch) || isOperator(ch)) {
						currentState = INT_END;
						symBuf = ch;
					} else {
						buffer[i++] = ch;
						currentState = IN_NUM;
					}
					break;

				case IN_NUM: // state 6 - can go back to state 6, or state 7 in real number, or state 8 in int end
					if (isdigit(ch) > 0) {
						buffer[i++] = ch;
						currentState = IN_NUM;
					}
					else if(ch == '.') {
						buffer[i++] = ch;
						currentState = IN_REAL;
					}
					else if (isSeparator(ch) || isOperator(ch)) {
						symBuf = ch;
						currentState = INT_END;
					}
					else { currentState = INT_END; }
					break;

				case IN_REAL: // state 7 - can go back to state 7 or state 9 real number end
					if (isdigit(ch) > 0) {
						buffer[i++] = ch;
						currentState = IN_REAL;
					}
					else { currentState = REAL_END; }
					break;

				case COMMENT_START: // state 10 - can only go to state 11 inside comment
					buffer[i++] = ch;
					currentState = IN_COMMENT;
					break;

				case IN_COMMENT: // state 11 - state continues to loop back to state 11 inside comment until end comment '!' is reached
					buffer[i++] = ch;
					if (ch == '!') {
						currentState = END_COMMENT;
					}
					break;

				case OPERATOR: // state 13 - can loop back into OPERATOR state or go to state 14 end of operator
					if(isOperator(ch)) {
						buffer[i++] = ch;
					} else { currentState = OPERATOR_END; }
					break;

				default:
					printf("Error: invalid state.\n");
					break;
				}

			if (currentState == ID_END || currentState == INT_END || currentState == REAL_END || currentState == END_COMMENT ||
					currentState == SEPARATOR || currentState == OPERATOR_END) {  // final accepting states
					buffer[i] = '\0';
					if (currentState == ID_END) {
						if(isKeyword(buffer)) { //check if string is a keyword or identifier
							printToken(outputPtr, "KEYWORD", buffer);
						} else { printToken(outputPtr, "IDENTIFIER", buffer); } // call printToken to print token and lexeme to output file
					}
					else if (currentState == INT_END) {
						printToken(outputPtr, "INTEGER", buffer);
					}
					else if (currentState == REAL_END) {
						printToken(outputPtr, "REAL NUMBER", buffer);
					}
					else if (currentState == END_COMMENT) {
						printToken(outputPtr, "COMMENT", buffer);
					}
					else if (currentState == SEPARATOR) {
						printToken(outputPtr, "SEPARATOR", buffer);
					}
					else if (currentState == OPERATOR_END) {
						printToken(outputPtr, "OPERATOR", buffer);
					}

					currentState = START; 	// set state back to initial state
					clearBuffer(buffer); // clears lexeme buffer
					i = 0;
			}

			if (symBuf != 0) { // for cases where separator/operator is next to identifier/number
				if (isSeparator(ch)) {
					printCharToken(outputPtr, "SEPARATOR", symBuf);
				}
				else if (isOperator(ch)) { printCharToken(outputPtr, "OPERATOR", symBuf); }
				symBuf = 0;
			}


	} //while loop end bracket

	fclose(fp); //close files after while loop executes
	fclose(outputPtr);
}

bool isOperator(char ch) {
	const char* operators[] = { "=", "<", ">", "+", "-", "/", "%", "*" };
	int match = 0;

	for (int i = 0; i < 8; i++) {
		if (strchr(operators[i], ch)) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isSeparator(char ch) {
	const char* separators[] = { "'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!" };
	int match = 0;

	for (int i = 0; i < 13; i++) {
		if (strchr(separators[i], ch)) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isKeyword(const char* buf) {
	const char* keywords[] = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend",
						  	  					 "do", "doend", "for", "forend", "input", "output", "and", "or" , "function" };
	int match = 0;

	for (int i = 0; i < 18; i++) {
		if (strcmp(keywords[i], buf) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}


int main(void) {
	lexer();
	printf("\nFinished lexical analysis. Results found in output.txt.\n");
	return 0;
}
