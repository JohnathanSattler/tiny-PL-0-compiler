// Compiler Builder 8
// Lester Young
// Timothy Konzel
// Remington Howell
// Johnathan Sattler
// John Herold

#ifndef _tokensh_
#define _tokensh_

typedef enum tokens {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqlsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

typedef struct sourceCode {
  char c;
  struct sourceCode * next;
} sourceCode;

typedef struct tok {
  union {
   	char * str;
   	int number;
  };
  int error;
  int id;
  char * msg;
  struct tok * next;
} tok;

// instruction struct
typedef struct instruction
{
  int op;
  int l;
  int m;
} instruction;

#endif
