#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "LinkedList.h"
#include "expression.h"

LinkedNode* firstLN = NULL;
LinkedNode* last = NULL;
int size = 0;

/**
 *	Creates the first node of the LinkedList and stores the appropriate data in the node
 *	@param p the first Exp node of the given expression
 *	@param expression the expression in string format
 */
void createLinkedList(Exp* p, char* expression) {
	LinkedNode* curr = (LinkedNode*) malloc(sizeof(LinkedNode)*1);
	LinkedNode* curr1 = (LinkedNode*) malloc(sizeof(LinkedNode)*1);
	curr->point = p;

	char* n = (char*) malloc(sizeof(char)*1);
	n = "foo";
	int t = strlen(n);
	int u = strlen(n);
	while(t > 0) {
		curr->name = n;
		curr->name++;
		n++;
		t--;
	}
	curr->name = curr->name - u;
	curr->string = expression;
	firstLN = curr;
	size++;
	last = curr1;
	firstLN->next = curr1;
	last->prev = curr;
}

/**
 *	Inserts a new node to the end of the LinkedList and stores all necessary data
 *	@param p the first Exp node of the given expression
 *	@param n the expression in string format
 */
void insertInLinkedList(Exp* p, char* n) {
	size++;
	// Takes care of the special case of when there's only one Linked Node in the Linked List
	if(size == 2) {
		last->point = p;
		int t = strlen(n);
		int u = strlen(n);
		while(t > 0) {
			last->name = n;
			last->name++;
			n++;
			t--;
		}
		last->name = last->name - u;
	} else {
		// Adds a new Linked Node to the Linked List when the LinkedList is of size greater than 2
		LinkedNode* curr = (LinkedNode*) malloc(sizeof(LinkedNode)*1);
		curr->point = p;

		int t = strlen(n);
		int u = strlen(n);
		while(t > 0) {
			curr->name = n;
			curr->name++;
			n++;
			t--;
		}
		curr->name = curr->name - u;
		LinkedNode* tmp = last;
		tmp->next = curr;
		curr->prev = tmp;
		last = curr;
	}
}

/**
 *	Returns the first Exp node of the expression. The correct expression is found using the expression's name
 *	@param n the name of the desired expression
 *	@return the first Exp node of the desired expression
 */
Exp* get(char* n) {
	LinkedNode* curr = firstLN;
	int s = size;
	while(curr != NULL) {
		if(s <= 0) {
			break;
		}
		// Compares n to the name of the LinkedNode if they match, return the linked node
		if(strcmp(curr->name, n) == 0) {
			return curr->point;
		}
		s--;
		curr = curr->next;
	}
	Exp* none = NULL;
	return none;
}

/**
 *	Returns the Linked Node with the specified function name
 *	@param	n the name of the desired expression
 *	@return the LinkedNode that contains the appropriate information about the function
 */
LinkedNode* getLL(char* n) {
	LinkedNode* curr = firstLN;
	int s = size;
	while(curr != NULL) {
		if(s <= 0) {
			break;
		}
		// Compares n to the name of the LinkedNode if they match, return the linked node
		if(strcmp(curr->name, n) == 0) {
			return curr;
		}
		s--;
		curr = curr->next;
	}
	LinkedNode* none = NULL;
	return none;
}

/**
 *	Returns the first Linked Node of the LinkedList
 *	@return returns the first LinkedNode
 */
LinkedNode* getFirstLN() {
	return firstLN;
}