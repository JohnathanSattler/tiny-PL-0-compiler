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
		token = 0;
		return;
	}

	token = tokenList -> id;
	tokenList = tokenList -> next;
}

void error(int num) {

	numError++;

	printf("Error(%d): ", num);

	switch (num) {
		case -2:
			printf("The preceding factor cannot begin with this symbol.");
			break;

		case -1:
			printf("Relational operator expected.");
			break;

		case 0:
			printf("Code ended abruptly.");
			break;

		case 1:
			break;

		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;

		case 7:
			break;

		case 8:
			break;

		case 9:
			break;

		case 10:
			break;

		case 11:
			break;

		case 12:
			break;

		case 13:
			break;

		case 14:
			break;

		case 15:
			break;

		case 16:
			break;

		case 17:
			break;

		case 18:
			break;

		case 19: // periodsym
			printf("Period expected\n");
			break;

		case 20:
			break;

		case 21:
			break;

		case 22: // endsym
			printf("End expected\n");
			break;

		case 23:
			break;

		case 24:
			break;

		case 25:
			break;

		case 26:
			break;

		case 27:
			break;

		case 28:
			break;

		case 29:
			break;

		case 30:
			break;

		case 31:
			break;

		case 32:
			break;

		case 33:
			break;

		default:
			printf("error: %d", num);
			break;
	}

	printf("\n");
}

void eat(int id) {

	if (token != id) error(id);
	advance();
}
