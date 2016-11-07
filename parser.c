// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "parser.h"

symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];

tok * tokenList;

int token;
int numError;
int cx = 0;
int numSym = 0;

int program(tok * allTokens, const char * outputFileName) {

	int i;

	tokenList = allTokens;

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

	writeFile(outputFileName, code, cx);

	return numError;
}

void block() {

	constDeclaration();
	varDeclaration();
	statement();
}

void constDeclaration() {

	char * name;
	int val;
	tok * temp;
	int omit = 0;

	if (token == constsym) {
		do {
			temp = tokenList;

			advance();

			if (token == identsym) name = temp -> str;
			else omit = 1;

			eat(identsym);

			temp = tokenList;

			eat(eqlsym);

			if (token == numbersym) val = temp -> number;
			else omit = 1;

			eat(numbersym);

			if (!omit) {
				addSymbol(1, name, val, 0, cx);
				emit(LIT, 0, val);
			}
		} while (token == commasym);

		eat(semicolonsym);
	}
}

void varDeclaration() {

	char * name;
	tok * temp;
	int omit = 0;

	if (token == varsym) {
		do {
			temp = tokenList;

			advance();

			if (token == identsym) name = temp -> str;
			else omit = 1;

			eat(identsym);

			if (!omit) {
				addSymbol(2, name, 0, 0, cx);
				emit(LIT, 0, 0);
			}
		} while (token == commasym);

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
		case -4:
			printf("too many symbols");
			break;

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

		case 2: // identsym
			printf("const, var, procedure must be followed by identifier.");
			break;

		case 3: // numbersym
			printf("= must be followed by a number.");
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

		case 9: // eqlsym
			printf("Identifier must be followed by =.");
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

		case 24: // thensym
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
		code[cx].op = op;
		code[cx].l = l;
		code[cx].m = m;
		cx++;
	}
}

int symbolExists(char * name) {

	int i;

	for (i = 0; i < numSym; i++)
		if (strcmp(symbolTable[i].name, name) == 0)
			return i;

	return -1;
}

void addSymbol(int kind, char * name, int val, int level, int addr) {

	if (numSym > MAX_SYMBOL_TABLE_SIZE) {
		error(-4);
	} else {
		symbolTable[numSym].kind = kind;
		strcpy(symbolTable[numSym].name, name);
		symbolTable[numSym].val = val;
		symbolTable[numSym].level = level;
		symbolTable[numSym].addr = addr;
		numSym++;
	}
}
