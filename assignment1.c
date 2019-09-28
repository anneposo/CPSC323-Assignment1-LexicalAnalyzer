#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZE 100

enum FsmState { START, ID_BEGIN, ID_INSIDE, ID_END, KEYWORD_END, NUM_BEGIN, NUM_INSIDE, INT_END, REAL_END }
enum IdentifierState { ID_START, IN_ID, ID_END, KEYWORD_END, }; // 4 states for identifier FSM
enum NumberState { NUM_START, IN_NUM, INT_END, DECIMAL, REAL_END }; // 5 states for integer and real FSM

const char* tokens = { "KEYWORD", "IDENTIFIER", "SEPARATOR", "OPERATOR" };
char buffer[BUFSIZE]; // holds the buffer of the current lexeme

// printToken prints the input token and lexeme in table format to the fp pointing to the output text file
void printToken(FILE* fp, const char* token, const char* lexeme) {
	fprintf(fp, "%s \t=\t ", token);
	fprintf(fp, "%s\n", lexeme);
}

void clearBuffer(char buf) {		//clearBuffer clears the buffer containing the current lexeme
	memset(buf, '\0', sizeof(buf));
}

void openFile(FILE* fp) {		// openFile opens file containing source code to test lexer
	char sourceCodeFile[100];

	printf("Enter source code filename: ");
	gets(sourceCodeFile);

	fp = fopen(sourceCodeFile, "r");
	if (fp == NULL) {
		printf("Could not open file.\n");
		exit(1);
	} else { printf("Opened %s successfully.\n\n", sourceCodeFile); }
}

void lexer(void) {
	FILE* fp;
	openFile(fp); //opens source code file

	enum IdentifierState currentIdState = ID_START;
	enum NumberState currentNumState = NUM_START;
	int i = 0;
	char ch;

	// use switch statement for FSM.
	while ((ch = fgetc(fp)) != EOF) {
		if(isalnum(ch) > 0) {										//**************************** FSM for identifiers and keywords
			switch (currentIdState) {

				case ID_START: // initial state - can go to 1st state or end
					if (isalnum(ch) > 0) {
						currentIdState = IN_ID;
						buffer[i++] = ch;
					} else { currentIdState = ID_END; }
					break;

				case IN_ID: // state 1 - can go back to 1 or to end
					if (isalnum(ch) > 0) {
						currentIdState = IN_ID;
						buffer[i++] = ch;
					} else { currentIdState = ID_END; }
					break;

				case ID_END: // accepting state for identifiers
					if (isSeparator(ch) || isOperator(ch)) {
						buffer[i] = '\0';
						if(isKeyword(buffer)) {
							currentIdState = KEYWORD_END;
						} else {
							 printToken(fp, "IDENTIFIER", buffer);  // call printToken to print token and lexeme to output file
							 currentIdState = ID_START;			// set state back to initial state
						}
					}
					break;

				case KEYWORD_END: // accepting state for keywords
					printToken("KEYWORD", buffer);  // call printToken to print token and lexeme to output file
					currentIdState = ID_START;  // set state back to initial state
					break;

				default:
					printf("Error: invalid state.\n");
					break;
			}
		}

		elseif(isdigit(ch) > 0) { 				//***************** FSM for numbers - integer or real
			switch (currentNumState) {
				case NUM_START:
					break;

				case IN_NUM:
					break;

				case INT_END:
					break;

				case DECIMAL:
					break;

				case REAL_END:
					break;

				default:
					printf("Error: invalid number state.\n");
				  break;
			}
		}

		//elseif (isSeparator(ch)) {}

	} //while loop end bracket

	fclose(fp); //close file after while loop executes
}

bool isOperator(char ch) {
	const char* operators[] = { "=", "<", ">", "+", "-", "/", "%", "*" }
	int match = 0;

	for (int i = 0; i < 8; i++) {
		if (strcmp(operators[i], ch) == 0) {
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
		if (strcmp(separators[i], ch) == 0) {
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
// 	char sourceCodeFile[50];
// 	char buffer[999];
// 	FILE *fp;
//
//
// 	// Get name of file to open to test lexer
// 	printf("Enter source code filename: ");
// 	gets(sourceCodeFile);
//
// 	// Open file to read source code
// 	fp = fopen(sourceCodeFile, "r");
// 	if (fp == NULL) { printf("Could not open file.\n"); exit(0); }
//
// //	Read the file and put first line of source code into buffer string
// 	fgets(buffer, sizeof(buffer), fp);
//
// 	printf("%s", buffer);

	return 0;
}
