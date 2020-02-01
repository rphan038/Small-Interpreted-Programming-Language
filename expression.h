#include <stdbool.h>
#ifndef EXP
#define EXP
typedef struct {
	char* symbol;
	struct Exp* next;
	struct Exp* prev;
	struct Exp* subset;
	bool tp;
} Exp;

extern void newExpression(char* word, bool isCurrent, char*);
extern int newExpressionHelp(Exp* x, char* word);
extern void printExpression(Exp*);
extern int printExpressionHelp(Exp* x, int lgth);
extern void subsetExpression(char* com);
extern void subsetExpressionHelp(Exp* x, char* com, bool rec);
extern int evaluateExpression(Exp*);
extern int evaluateExpressionHelp(Exp* u);
extern void newFuncParam(char*, char*);
extern Exp* getFirst();
extern int count(Exp*);
extern void appendExp(char*, char*);
extern char* appendExpHelp(char**, char*, int);
extern void traverse(Exp*, char*, int);
extern char* firstAndRest(char*);
extern char* getRefer();
extern int eval(char, int[], int);

#endif