// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "parser.h"

int program(tok * allTokens) {

	tokenList = allTokens;

	printf("\n");

	numError = 0;

	advance();

	block();

	eat(periodsym);

	return numError;
}

void block() {

	constDeclaration();
	varDeclaration();
	statement();
}

void constDeclaration() {

	if (token == constsym) {
		advance();
		eat(identsym);
		eat(eqlsym);
		eat(numbersym);

		while (token == commasym) {
			advance();
			eat(identsym);
			eat(eqlsym);
			eat(numbersym);
		}

		eat(semicolonsym);
	}
}

void varDeclaration() {

	if (token == varsym) {
		advance();
		eat(identsym);

		while (token == commasym) {
			advance();
			eat(identsym);
		}

		eat(semicolonsym);
	}
}

void statement() {

	if (token == identsym) {
		advance();
		eat(becomessym);
		expression();
	} else if (token == beginsym) {
		advance();
		statement();

		while (token == semicolonsym) {
			advance();
			statement();
		}

		eat(endsym);
	} else if (token == ifsym) {
		advance();
		condition();
		eat(thensym);
		statement();
	} else if (token == whilesym) {
		advance();
		condition();
		eat(dosym);
		statement();
	} else if (token == readsym) {
		advance();
		eat(identsym);
	} else if (token == writesym) {
		advance();
		eat(identsym);
	}
}

void condition() {

	if (token == oddsym) {
		advance();
		expression();
	} else {
		expression();
		relOp();
		expression();
	}
}

void relOp() {

	if (token != eqlsym || token != neqsym || token != lessym ||
		token != leqsym || token != gtrsym || token != geqsym) {

		error(-1);
		return; 
	}

	advance();
}

void expression() {

	if (token == plussym || token == minussym) {
		advance();
	}

	term();

	while (token == plussym || token == minussym) {
		advance();
		term();
	}
}

void term() {

	factor();

	while (token == multsym || token == slashsym) {
		advance();
		factor();
	}
}

void factor() {

	if (token == identsym) {
		advance();
	} else if (token == numbersym) {
		advance();
	} else if (token == lparentsym) {
		advance();
		expression();
		eat(rparentsym);
	} else {
		error(-2);
	}
}

void advance() {

	if (tokenList == NULL) {
		return;
	}

	token = tokenList -> id;
	tokenList = tokenList -> next;

	//printf("tok: %d\n", token);
}

void error(int num) {

	numError++;

	printf("Error: ");

	switch (num) {
		case -2:
			printf("The preceding factor cannot begin with this symbol.\n");
			break;

		case -1:
			printf("Relational operator expected.\n");
			break;

		case 0:
			printf("Code ended abruptly\n");
			break;

		default:
			printf("error: %d\n", num);
			break;
	}
}

void eat(int id) {

	if (token != id) error(id);
	else
	advance();
}
