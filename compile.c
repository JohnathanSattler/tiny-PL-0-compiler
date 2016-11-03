// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "compile.h"

// main function
int main(int argc, const char * argv[]) {

	sourceCode * code = NULL;
	tok * toks = NULL;
	int error;

	code = handleInput(argc, argv, code);

	printCode(code, 0, 0);

	toks = lex(toks, code);

	// printToks(toks);

	if (printToksErrors(toks) == 0) {
		if (program(toks) == 0) {
			printf("No errors, program syntactically correct\n");
		}
	}

	return 0;
}
