// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

// Included libraries
#include "input.h"

// handle the arguments passed into the program
sourceCode * handleInput(int argc, const char * argv[], sourceCode * code, const char * outputFileName[]) {

	const char * inputFile, * outputFile;

	inputFile = argv[1];
	outputFile = argv[2];

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
