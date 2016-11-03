// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include <stdio.h>
#include "tokens.h"

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct symbol {
	int kind; 	   // const = 1, var = 2, proc = 3
	char name[12]; // name up to 11 chars
	int val; 	   // number (ASCII value)
	int level; 	   // L level
	int addr; 	   // M address
} symbol;

symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
tok * tokenList;
int token;
int numError;

// Function prototypes
int program(tok * allTokens);
void block();
void constDeclaration();
void varDeclaration();
void statement();
void condition();
void relOp();
void expression();
void term();
void factor();
void advance();
void error(int num);
void eat(int id);