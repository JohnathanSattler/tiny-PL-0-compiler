// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "input.h"

// handle the arguments passed into the program
sourceCode * handleInput(int argc, const char * argv[], sourceCode * code, const char * outputFileName[], int * s, int * c, int * t, int * p) {

	const char * inputFile, * outputFile;
	int i, source = 0, clean = 0, printTokens = 0, pm0 = 0;

	for (i = 1; i < argc; i++)
		if (strcmp(argv[i], "--source") == 0)
			source = 1;
		else if (strcmp(argv[i], "--clean") == 0)
			clean = 1;
		else if (strcmp(argv[i], "--tokens") == 0)
			printTokens = 1;
		else if (strcmp(argv[i], "--pm0") == 0)
			pm0 = 1;
		else {
			inputFile = argv[i];
			i++;
			if (i >= argc) {
				printf("Invalid Arguments. Usage: ./compile (arguments) (inputfile) (outputfile)\n");
				exit(1);
			}
			outputFile = argv[i];
		}

	*s = source;
	*c = clean;
	*t = printTokens;
	*p = pm0;

	*outputFileName = outputFile;

	code = readFile(inputFile, code);

	return code;
}

// read the input file
sourceCode * readFile(const char * fileName, sourceCode * code) {

	FILE * ifp;
	char temp;
	sourceCode * last = code;

	// opens file
	ifp = fopen(fileName, "r");

	// if there is no file, execute print statement and exits function
	if (ifp == NULL) {
		printf("Error: File not found\n");
		exit(0);
	}

	while(fscanf(ifp, "%c", &temp) != EOF) {
		sourceCode * current = (sourceCode *) malloc(sizeof(sourceCode));
		current -> c = temp;
		current -> next = NULL;

		if (last == NULL) {
			last = current;
			code = last;
		} else {
			last -> next = current;
			last = last -> next;
		}
	}

	// closes file
	fclose(ifp);

	return code;
}

void writeFile(const char * fileName, instruction * code, int size) {

	FILE * ofp;
	int i;

	ofp = fopen(fileName, "w");

	for (i = 0; i < size; i++) {
		fprintf(ofp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
	}

	fclose(ofp);

	return;
}
