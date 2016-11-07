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
	const char * outputFileName[100];

	int * source = (int *) malloc(sizeof(int));
	int * clean = (int *) malloc(sizeof(int));
	int * printTok = (int *) malloc(sizeof(int));
	int * compiled = (int *) malloc(sizeof(int));

	code = handleInput(argc, argv, code, outputFileName, source, clean, printTok, compiled);

	printCode(code, *source, *clean);

	toks = lex(toks, code);

	if (*printTok)
		printToks(toks);

	if (printToksErrors(toks) == 0)
		if (program(toks, *outputFileName, *compiled) == 0)
			printf("No errors, program syntactically correct\n");

	return 0;
}
