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
int numError = 0;
int cx = 0;
int numSym = 0;
char * identName;
int identVal;
int relOpCode;
int numVar = 0;

int program(tok * allTokens, const char * outputFileName, int pm0) {

	int i;

	tokenList = allTokens;

	printf("\n");

	advance();
	block();
	eat(periodsym);

	emit(SIO, 0, SIO_HLT);

	if (numError == 0) {
		if (pm0) {
			printAssembly(code, cx);
			printf("\n");
		}

		writeFile(outputFileName, code, cx);
	}

	return numError;
}

void block() {

	constDeclaration();
	varDeclaration();

	emit(INC, 0, 4 + numVar);

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

			if (token == identsym)
				name = temp -> str;
			else
				omit = 1;

			eat(identsym);

			temp = tokenList;

			eat(eqlsym);

			if (token == numbersym)
				val = temp -> number;
			else
				omit = 1;

			eat(numbersym);

			if (!omit)
				addSymbol(1, name, val, 0, val);
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

			if (token == identsym)
				name = temp -> str;
			else
				omit = 1;

			eat(identsym);

			if (!omit) {
				addSymbol(2, name, 0, 0, 4 + numVar);
				numVar++;
			}
		} while (token == commasym);

		eat(semicolonsym);
	}
}

void statement() {

	int cx1;
	int cx2;

	char * name;
	int i;
	int m;

	if (token == identsym) {
		name = identName;
		i = symbolExists(name);

		if (i < 0)
			error(-5);
		else
			m = symbolTable[i].addr;

		advance();
		eat(becomessym);
		expression();

		if (symbolTable[i].kind == 2)
			emit(STO, 0, m);
		else
			error(-6);
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

		statement();
		emit(JMP, 0, cx1);
		code[cx2].m = cx;
	} else if (token == readsym) {
		advance();

		name = identName;
		i = symbolExists(name);

		if (i < 0)
			error(-5);
		else
			m = symbolTable[i].addr;

		eat(identsym);

		emit(SIO, 0, SIO_INP);
		emit(STO, 0, m);
	} else if (token == writesym) {
		advance();

		name = identName;
		i = symbolExists(name);

		if (i < 0)
			error(-5);
		else
			m = symbolTable[i].addr;

		eat(identsym);

		if (symbolTable[i].kind == 1)
			emit(LIT, 0, m);
		else
			emit(LOD, 0, m);

		emit(SIO, 0, SIO_OUT);
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

		emit(OPR, 0, relOpCode);
	}
}

void relOp() {

	if (token == eqlsym)
		relOpCode = OPR_EQL;
	else if (token == neqsym)
		relOpCode = OPR_NEQ;
	else if (token == lessym)
		relOpCode = OPR_LSS;
	else if (token == leqsym)
		relOpCode = OPR_LEQ;
	else if (token == gtrsym)
		relOpCode = OPR_GTR;
	else if (token == geqsym)
		relOpCode = OPR_GEQ;
	else
		error(-1);

	advance();
}

void expression() {

	int addop;

	if (token == plussym || token == minussym) {
		addop = token;

		advance();
		term();

		if (addop == minussym)
			emit(OPR, 0, OPR_NEG); // negate
	} else {
		term();
	}

	while (token == plussym || token == minussym) {
		addop = token;

		advance();
		term();

		if (addop == plussym)
			emit(OPR, 0, OPR_ADD); // addition
		else
			emit(OPR, 0, OPR_SUB); // subtraction
	}
}

void term() {

	int mulop;

	factor();

	while (token == multsym || token == slashsym) {
		mulop = token;

		advance();
		factor();

		if (mulop == multsym)
			emit(OPR, 0, OPR_MUL); // multiplication
		else
			emit(OPR, 0, OPR_DIV); // division
	}
}

void factor() {

	char * name;
	int i, m;

	if (token == identsym) {
		name = identName;
		i = symbolExists(name);

		if (i < 0)
			error(-5);
		else
			m = symbolTable[i].addr;

		advance();

		if (symbolTable[i].kind == 1)
			emit(LIT, 0, m);
		else
			emit(LOD, 0, m);
	} else if (token == numbersym) {
		m = identVal;

		advance();

		emit(LIT, 0, m);
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

	if (token == identsym)
		identName = tokenList -> str;
	else if (token == numbersym)
		identVal = tokenList -> number;

	tokenList = tokenList -> next;
}

void error(int num) {

	numError++;

	printf("Error(%d): ", num);

	switch (num) {
		case -6:
			printf("Assignment to constant or procedure is not allowed.");
			break;

		case -5:
			printf("Undeclared identifier");
			break;

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

		case 2: // identsym
			printf("const, var, procedure must be followed by identifier.");
			break;

		case 3: // numbersym
			printf("= must be followed by a number.");
			break;

		case 9: // eqlsym
			printf("Identifier must be followed by =.");
			break;

		case 16: // rparentsym
			printf("expected right parenthesis");
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

		case 22: // endsym
			printf("end expected");
			break;

		case 24: // thensym
			printf("then expected");
			break;

		case 26: // dosym
			printf("do expected.");
			break;

		default:
			printf("unkown error");
			break;
	}

	printf("\n");

	exit(1);
}

void eat(int id) {

	if (token != id)
		error(id);

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
