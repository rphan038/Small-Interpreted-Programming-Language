#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression.h"
#include "operations.h"
#include "LinkedList.h"

/**
 * Main method that runs the program
 * @return - Returns 0 to signify the end of the program
 */
int main() {
	// Asks the user to input the first expression of the program
	printf("%s\n", "Enter an Expression:");
	//Declares the character pointer that will contain the data in character format
	char* x = (char*) malloc((sizeof (char))*100);
	//Initializes the character pointer with what the user has inputted.
	fgets(x, 100, stdin);
	//Translates the inputted data in the character pointer to the data structure
	newExpression(x, false, NULL);

	createLinkedList(getFirst(), x);

	//Variable containing the operation being performed
	char operation;
	//Asks the user to input a first operation to be done to the data structure
	printf("%s\n","First operation:");
	//Initializes the operation character variable to what the user inputted
	scanf("%c", &operation);
	//Consumes the \n so that scanf does not ask for an input twice
	getchar();
	//The program will keep running until the operation variable equals q
	while(operation != 'q') {
		//If operation equals c
		if(operation == 'c') {
			//Create a new data structure with new data
			create(x);
			//Saves the next input to the operation variable
			scanf("%c",&operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If operation equals p
		} else if(operation == 'p') {
			//Print the expression currently in the data structure
			print();
			//Saves the next input to the operation variable
			scanf("%c",&operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If operation equals a
		} else if(operation == 'a') {
			//Adds new data to the current data structure
			append(x);
			//Saves the next input to the operation variable
			scanf("%c",&operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If operation equals s
		} else if(operation == 's') {
			//Prints the subset the the user desired
			subset();
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If operation equals e
		} else if (operation == 'e') {
			//Evaluates the function in the data structure
			evaluate();
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If the operation is not a valid command
		} else if (operation == 'v') {
			variable();
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If the operation is not a valid command
		} else if (operation == 'l') {
			funcParam();
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If the operation is not a valid command
		} else if (operation == 'd') {
			funcNoParam();
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
			//If the operation is not a valid command
		} else if((operation != 'c') || (operation != 'p') || (operation != 'a') || (operation != 's') || 
			(operation != 'e') || (operation != 'q') || (operation != 'v') || (operation != 'l') 
			|| (operation != 'd')) {
			//Tell the user it is invalid and ask them to input a valid command
			printf("%s\n", "Invalid Operation - Enter a Valid Operation:");
			//Saves the next input to the operation variable
			scanf("%c", &operation);
			//Consumes the \n so that the scanf does not ask for an input twice
			getchar();
		} 
	}
	return 0;
}