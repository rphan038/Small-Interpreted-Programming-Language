#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expression.h"
#include "LinkedList.h"
#include "operations.h"

/**
 * Calls the newExpression(x) method from expression.h to create a new function
 *
 * @param x - Character pointer that contains the new function in character format
 */
void create(char* x) {
	//Tells the user to enter a new expression
	printf("%s\n", "New Expression:");
	//Uses standard input to initialize the character pointer to the new desired function
	fgets(x, 100, stdin);
	//Creates the data structure with this new function
	newExpression(x, true, NULL);
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}

/**
 *	Calls the newExpression(x) method from expression.h to create a new variable
 */
void variable() {
	// Asks user for a variable name and saves it
	char* w = (char*) malloc ((sizeof (char))*100);
	printf("%s\n", "Enter a Variable Name");
	scanf("%s", w);
	getchar();
	// Asks user for expression and saves it
	char* z = (char*) malloc((sizeof(char))*100);
	printf("%s\n", "Enter an Expression");
	fgets(z, 100, stdin);
	// Creates variable and saves it to symbol table
	newExpression(z, false, w);
	insertInLinkedList(getFirst(), w);
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}

/**
 *	Creates a function with no parameters
 */
void funcNoParam() {
	// Asks user for a function name and saves it
	char* w = (char*) malloc ((sizeof (char))*100);
	printf("%s\n", "Enter a Variable Name");
	scanf("%s", w);
	getchar();
	// Asks user for expression and saves it
	char* z = (char*) malloc((sizeof(char))*100);
	printf("%s\n", "Enter an Expression");
	fgets(z, 100, stdin);
	// Checks whether the function name has been used before
	LinkedNode* tmp = getLL(w);
	if(tmp != NULL) {
		newExpression(z, false, w);
		tmp->point = getFirst();
		tmp->string = z;
		LinkedNode* r = getFirstLN();
		// Goes through the entire LinkedList to check all functions
		while(r != NULL) {
			// If any of the functions in the LinkedList refer to the present expression, update it
			if(r->refersTo != NULL) {
				if(strcmp(r->refersTo, w) == 0) {
					newExpression(r->string, false, r->name);
					r->point = getFirst();
				}
			}
			r = r->next;
		}
	// If not, create a new expression like normal and update the symbol table appropriately
	} else {
		newExpression(z, false, w);
		insertInLinkedList(getFirst(), w);
		LinkedNode* s = getLL(w);
		if(getRefer() != NULL) 
			s->refersTo = getRefer();
		s->string = z;
	}
	printf("%s\n", "Next operation:");
}

/**
 *	Creates a function with parameters
 */
void funcParam() {
	// Asks user for a function name and saves it
	char* w = (char*) malloc ((sizeof (char))*100);
	printf("%s\n", "Enter a Variable Name");
	scanf("%s", w);
	getchar();
	// Asks user for an parameter names and saves it
	printf("%s\n", "Enter Parameter Names");
	char* vars = (char*) malloc((sizeof(char))*10);
	fgets(vars, 100, stdin);
	char* v = vars;
	char* vars1 = (char*) malloc((sizeof(char))*10);
	int tmp = 0;
	// Adds the parameters to the symbol table
	while(vars != NULL && *vars != '\0' && *vars != '\n') {
		*vars1 = *vars;
		vars++;
		vars1++;
		tmp++;
		if(*vars == ' ') {
			vars1 = vars1 - tmp;
			insertInLinkedList(NULL, vars1);
			vars1 = (char*) malloc((sizeof(char))*10);
			tmp = 0;
			vars++;
		}
	}
	vars1 = vars1 - tmp;
	insertInLinkedList(NULL, vars1);
	// Asks the user for an expression and saves it
	char* z = (char*) malloc((sizeof(char))*100);
	printf("%s\n", "Enter an Expression");
	fgets(z, 100, stdin);
	newExpression(z, false, w);
	insertInLinkedList(getFirst(), w);
	// Updates the symbol table with correct information
	LinkedNode* temp = getLL(w);
	temp->string = z;
	temp->data = v;
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}

/*
 * Calls the printExpression() method from expression.h to print the function currently in the data structure
 */
void print() {
	//Prints the function
	char* w = (char*) malloc ((sizeof (char))*100);
	printf("%s\n", "Which Expression?");
	scanf("%s", w);
	getchar();
	Exp* this = get(w);
	LinkedNode* node = getLL(w);
	if(this == NULL) {
		printf("%s\n", "Expression Does Not Exist");
	} else {
		printExpression(this);
	}
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}

/*
 * Manipulates strings to append a new set of data to the old one and creates a new data structure afterwards
 * @param - The character pointer that contains the old set of data
 * @return - The character pointer that contains both the old and new data
 */
char* append(char* x) {
	//Copies the contents of the original character pointer to y
	LinkedNode* k = getLL("foo");
	char* y = k->string;
	//Erases the contents of x
	x = (char*) malloc((sizeof (char))*100);
	//Copies all characters in y to x except for the last two characters which would be a null and a ')'
	memmove(x, y, strlen(y) - 2);
	//Asks the user to input an expression to append
	printf("%s\n", "Enter an Expression to Add:");
	//Declares two character pointers for more string manipulation
	char* app = (char*) malloc((sizeof (char))*100);
	char* app2 = (char*) malloc((sizeof (char))*100);
	//Initializes app with the inputed 
	fgets(app, 100, stdin);
	//Copies all of the elements in app to app2 except for the null \0
	memmove(app2, app, strlen(app) - 1);
	//Appends a space character to the end of the x character pointer
	strcat(x, " ");
	//Appends a closing parentheses at the end of the app2 character pointer
	strcat(app2, ")\n");
	//Adds the character pointer with new data to the end of the character pointer containing the old data 
	strcat(x, app2);
	//Creates the new data structure with the appended data elements
	newExpression(x, true, NULL);
	//frees the two pointers we don't need anymore
	free(app);
	free(app2);
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
	//Returns the character pointer so that the global variable is updated
	return x;
}

/*
 * Asks the user to input a desired subset and calls the subsetExpression() method to print out the results
 */
void subset() {
	//Asks the user to input a subset to print
	printf("%s\n", "Enter the Desired Subset:");
	// Declares a character pointer and allocates space to store what the desired subset is
	char* new = (char*) malloc((sizeof (char))*100);
	//Initializes the character pointer with what the user types
	fgets(new, 100, stdin);
	//Calls the subset method from expression.h
	subsetExpression(new);
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}

/*
 * Evaluates the function currently in the data structure by calling evaluateExpression()
 */
void evaluate() {
	char* w = (char*) malloc ((sizeof (char))*100);
	int a = 0;
	printf("%s\n", "Which Expression?");
	scanf("%s", w);
	getchar();
	Exp* this = get(w);
	if(this == NULL) {
		printf("%s\n", "Expression Does Not Exist");
	} else {
		a = evaluateExpression(this);
	}
	//Prints the result
	printf("The Evaluated Answer is: %d\n", a);
	//Asks the user to input the next operation
	printf("%s\n", "Next operation:");
}