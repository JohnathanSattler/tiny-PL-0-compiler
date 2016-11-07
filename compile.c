// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "compile.h"

instruction code[MAX_CODE_LENGTH];

// main function
int main(int argc, const char * argv[]) {

	sourceCode * code = NULL;
	tok * toks = NULL;
	int error;
	const char * outputFileName[100];

	code = handleInput(argc, argv, code, outputFileName);

	printCode(code, 0, 0);

	toks = lex(toks, code);

	// printToks(toks);

	if (printToksErrors(toks) == 0) {
		if (program(toks, *outputFileName) == 0) {
			printf("No errors, program syntactically correct\n");
		}
	}

	return 0;
}
