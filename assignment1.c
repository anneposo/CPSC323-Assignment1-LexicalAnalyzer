#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> // for isalnum() and isdigit()

#define BUFSIZE 100

void printToken(FILE*, const char*, const char*);
void clearBuffer(char*);
void getFile(void);
void lexer(void);
bool isOperator(char);
bool isSeparator(char);
bool isKeyword(const char*);

enum FsmState { START, ID_START, IN_ID, ID_END, KEYWORD_END, NUM_START, IN_NUM, IN_REAL, INT_END, REAL_END, COMMENT_START, IN_COMMENT, END_COMMENT };
//enum IdentifierState { ID_START, IN_ID, ID_END }; // 3 states for identifier FSM
//enum NumberState { NUM_START, IN_NUM, INT_END, IN_REAL, REAL_END }; // 5 states for integer and real FSM

char buffer[BUFSIZE]; // holds the buffer of the current lexeme
char sourceCodeFile[BUFSIZE]; //holds filename of source code to scan with lexer program

// printToken prints the input token and lexeme in table format to the fp pointing to the output text file
void printToken(FILE* fp, const char* token, const char* lexeme) {
	fprintf(fp, "%s \t=\t ", token);
	fprintf(fp, "%s\n", lexeme);
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

void lexer(void) {
	FILE* fp;
	FILE* outputPtr;
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
	int i = 0;
	char ch;

	// use switch statement for FSM.
	while ((ch = fgetc(fp)) != EOF) {
			//if((isalpha(ch) > 0) || currentIdState == IN_ID) {		//**************************** FSM for identifiers and keywords
			switch (currentState) {
				case START:   // 0 - initial identifier state - can go to state 1 id start, or state 2 num start
					buffer[i++] = ch;
					if(isalpha(ch) > 0) {
						currentState = ID_START;
					}
				  if (isdigit(ch) > 0) {
						currentState = NUM_START;
					}
					if (ch == '!') { // "!" signifies beginning/end of a comment
						currentState = COMMENT_START;
					}
					break;

				case ID_START: // 1 - initial identifier state - can go to 2nd state
					buffer[i++] = ch;
					currentState = IN_ID;
					break;

				case IN_ID: // state 2 - can go back to 2 or to ID end accepting state
					if (isalpha(ch) > 0 || isdigit(ch) > 0 || ch == '$') {
						buffer[i++] = ch;
						currentState = IN_ID;
					}
					else { currentState = ID_END; }
					break;

				case NUM_START: // 3 - initial number state - can go to 4th state in number
					buffer[i++] = ch;
					currentState = IN_NUM;
					break;

				case IN_NUM: // state 4 - can go back to state 4, or state 5 in real number, or state 7 in int end
					if (isdigit(ch) > 0) {
						buffer[i++] = ch;
						currentState = IN_NUM;
					}
					else if(ch == '.') {
						buffer[i++] = ch;
						currentState = IN_REAL;
					}
					else { currentState = INT_END; }
					break;

				case IN_REAL: // state 5 - can go back to state 5 or state 8 real number end
					if (isdigit(ch) > 0) {
						buffer[i++] = ch;
						currentState = IN_REAL;
					} else { currentState = REAL_END; }
					break;

				case COMMENT_START:
					buffer[i++] = ch;
					currentState = IN_COMMENT;
					break;

				case IN_COMMENT:
					buffer[i++] = ch;
					if (ch == '!') {
						currentState = END_COMMENT;
					}
					break;

				default:
					printf("Error: invalid state.\n");
					break;
				}

			if (currentState == ID_END) {  // accepting state for identifiers/keywords
		//		if (isSeparator(ch) || isOperator(ch)) {
					buffer[i] = '\0';
					if(isKeyword(buffer)) { //check if string is a keyword or identifier
						printToken(outputPtr, "KEYWORD", buffer);
					} else { printToken(outputPtr, "IDENTIFIER", buffer); } // call printToken to print token and lexeme to output file
					currentState = START; 	// set state back to initial state
					clearBuffer(buffer); // clears lexeme buffer
					i = 0;
		//		}
			}

			if (currentState == INT_END || currentState == REAL_END) {
					buffer[i] = '\0';
					if (currentState == INT_END) {
						printToken(outputPtr, "INTEGER", buffer);
					} else { printToken(outputPtr, "REAL NUMBER", buffer); }
					currentState = START;
					clearBuffer(buffer);
					i = 0;
			}

			if (currentState == END_COMMENT) {
				buffer[i] = '\0';
				printToken(outputPtr, "COMMENT", buffer);
				currentState = START;
				clearBuffer(buffer);
				i = 0;
			}

		//elseif (isSeparator(ch)) {}

	} //while loop end bracket

	fclose(fp); //close files after while loop executes
	fclose(outputPtr);
}

bool isOperator(char ch) {
	const char* operators[] = { "=", "<", ">", "+", "-", "/", "%", "*" };
	int match = 0;

	for (int i = 0; i < 8; i++) {
		if (strchr(operators[i], ch) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isSeparator(char ch) {
	const char* separators[] = { " ", "'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!" };
	int match = 0;

	for (int i = 0; i < 13; i++) {
		if (strchr(separators[i], ch) == 0) {
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
	printf("\nFinished.\n");

	return 0;
}
