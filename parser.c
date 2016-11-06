// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "parser.h"

int program(tok * allTokens) {

	int i;

	tokenList = allTokens;
	cx = 0;

	printf("\n");

	numError = 0;

	advance();

	block();

	eat(periodsym);

	printf("Code:\n");
	for (i = 0; i < cx; i++) {
		printf("%d %d %d\n", code[i].op, code[i].l, code[i].m);
	}
	printf("\n");

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

	int cx1;
	int cx2;

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

		cx1 = cx;
		emit(JPC, 0, 0);
		statement();
		code[cx1].m = cx;
	} else if (token == whilesym) {
		cx1 = cx;
		advance();
		condition();
		cx2 = cx;
		emit(JPC, 0, 0);
		eat(dosym);

		emit(JPC, 0, cx1);
		statement();
		code[cx2].m = cx;
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

	if (token != eqlsym && token != neqsym && token != lessym &&
		token != leqsym && token != gtrsym && token != geqsym) {

		error(-1);
		return; 
	}

	advance();
}

void expression() {

	int addop;

	if (token == plussym || token == minussym) {
		addop = token;
		advance();
		term();
		if (addop == minussym) {
			emit(OPR, 0, OPR_NEG); // negate
		}
	} else {
		term();
	}

	while (token == plussym || token == minussym) {
		addop = token;
		advance();
		term();
		if (addop == plussym) {
			emit(OPR, 0, OPR_ADD); // addition
		} else {
			emit(OPR, 0, OPR_SUB); // subtraction
		}
	}
}

void term() {

	int mulop;

	factor();

	while (token == multsym || token == slashsym) {
		mulop = token;
		advance();
		factor();
		if (mulop == multsym) {
			emit(OPR, 0, OPR_MUL); // multiplication
		} else {
			emit(OPR, 0, OPR_DIV); // division
		}
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

	// printf("tok: %d\n", token);
}

void error(int num) {

	numError++;

	printf("Error(%d): ", num);

	switch (num) {
		case -3:
			printf("code too long");
			break;

		case -2:
			printf("the preceding factor cannot begin with this symbol.");
			break;

		case -1:
			printf("relational operator expected.");
			break;

		case 0:
			printf("code ended abruptly.");
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

		case 16: // rparentsym
			printf("expected right parenthesis");
			break;

		case 17:
			break;

		case 18: // semicolonsym
			printf("semicolon expected");
			break;

		case 19: // periodsym
			printf("period expected");
			break;

		case 20: // becomesym
			if (token == eqlsym) {
				printf("use = instead of :=");
			} else {
				printf("expected :=");
			}
			break;

		case 21:
			break;

		case 22: // endsym
			printf("end expected");
			break;

		case 23:
			break;

		case 24:
			printf("then expected");
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
			printf("unkown error");
			break;
	}

	printf("\n");
}

void eat(int id) {

	if (token != id) error(id);
	advance();
}

void emit(int op, int l, int m) {

	if (cx > MAX_CODE_LENGTH) {
		error(-3);
	} else {
		code[cx].op = op;//sym.val;
		code[cx].l = l;//sym.level;
		code[cx].m = m;//sym.addr;
		cx++;
	}
}
