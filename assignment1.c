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

//enum FsmState { START, ID_BEGIN, ID_INSIDE, ID_END, KEYWORD_END, NUM_BEGIN, NUM_INSIDE, INT_END, REAL_END };
enum IdentifierState { ID_START, IN_ID, ID_END }; // 4 states for identifier FSM
enum NumberState { NUM_START, IN_NUM, INT_END, DECIMAL, REAL_END }; // 5 states for integer and real FSM

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

	enum IdentifierState currentIdState = ID_START; // declares current ID state variable and initializes to starting state
	enum NumberState currentNumState = NUM_START; // declares current number state variable and initializes to starting state
	int i = 0;
	char ch;

	// use switch statement for FSM.
	while ((ch = fgetc(fp)) != EOF) {
			if((isalnum(ch) > 0) || currentIdState == IN_ID) {		//**************************** FSM for identifiers and keywords
			switch (currentIdState) {

				case ID_START: // initial identifier state - can go to 1st state
					currentIdState = IN_ID;
					buffer[i++] = ch;
					break;

				case IN_ID: // state 1 - can go back to 1 or to end
					if (isalnum(ch) > 0) {
						currentIdState = IN_ID;
						buffer[i++] = ch;
					} else { currentIdState = ID_END; }
					break;

				// case ID_END: // accepting state for identifiers
				// 	if (isSeparator(ch) || isOperator(ch)) {
				// 		buffer[i] = '\0';
				// 		if(isKeyword(buffer)) {
				// 			printToken(outputPtr, "KEYWORD", buffer);
				// 		} else { printToken(outputPtr, "IDENTIFIER", buffer); } // call printToken to print token and lexeme to output file
				// 		currentIdState = ID_START; 	// set state back to initial state
				// 		clearBuffer(buffer); // clears lexeme buffer
				// 	}
				// 	break;
				default:
					printf("Error: invalid state.\n");
					break;
				}
			}
			if (currentIdState == ID_END) {  // accepting state for identifiers/keywords
				if (isSeparator(ch) || isOperator(ch)) {
					buffer[i] = '\0';
					if(isKeyword(buffer)) { //check if string is a keyword or identifier
						printToken(outputPtr, "KEYWORD", buffer);
					} else { printToken(outputPtr, "IDENTIFIER", buffer); } // call printToken to print token and lexeme to output file
					currentIdState = ID_START; 	// set state back to initial state
					clearBuffer(buffer); // clears lexeme buffer
					i = 0;
				}
			 }

	 	 	  else if((isdigit(ch) > 0) || currentNumState == IN_NUM) { 				//***************** FSM for numbers - integer or real
	 				switch (currentNumState) {
						case NUM_START: // initial number state - can go to 1st state
							currentNumState = IN_NUM;
							buffer[i++] = ch;
							break;

						case IN_NUM: // state 1 - can go back to state 1, or state 2 integer, or state 3 decimal, or state 4 real number
							break;

						// case INT_END:
						// 	break;

						case DECIMAL: // state 3 - can go to state 1 inside number or state 4 real number accepting state
							break;

						// case REAL_END:
						// 	break;

						default:
							printf("Error: invalid number state.\n");
						  break;
					}
				}
				if (currentNumState == INT_END) {
					
				}

				if (currentNumState == REAL_END) {

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
	printf("Finished.\n");

	return 0;
}
