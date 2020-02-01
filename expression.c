#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "expression.h"
#include "LinkedList.h"


Exp* first = NULL;		//This Exp pointer points to the first node of the expression
int length = 0;			//Keeps track of how many elements are in the expression
char* refer = NULL;

/**
 * Takes an input a character pointer and uses this pointer to create the appropriate Exp data structure
 * @param word - A character pointer containing the desired expression to be stored in the data structure
 * @param isCurrent - Boolean variable telling whether this is the current expression or not
 * @param name - The name of the expression
 */
void newExpression(char* word, bool isCurrent, char* name) {
	//Resets the length to 0
	length = 0;
	char* duplicate = (char*) malloc ((sizeof (char))*10);
	duplicate = word;
	//Declares the first node of the data structure and allocates space for it
	Exp* curr = (Exp*) malloc(sizeof(Exp)*1);
	//Stores the first character in the char pointer to the data element of Exp node
	curr->symbol = word;
	curr->tp = false;
	//Initializes the first Exp pointer to this first node so that it can be referenced later in the program
	first = curr;
	//Increments the character pointer to point to the next character
	word++;
	//Increases the length variable because the an element has just been added
	length++;


	// Searches the first element of the expression and determines if there is something that is not
	// a sign or a number
	if(!isdigit(*word) && *word != '+' && *word != '-' && *word != '*' && *word != '\0' && *word != '\n') {
		char* var = (char*) malloc ((sizeof (char))*10);
		char* p = word;
		int l = 1;
		// Finds the full name or operator to later be processed
		*var = *p;
		var++;
		p++;
		while(*p != ' ') {
			*var = *p;
			var++;
			p++;
			l++;
		}
		var = var - l;

		// if the first operator is a, call the method that appends functions together
		if(strlen(var) == 1 && *var == 'a') {
			appendExp(p, name);
			return;
		// If the operators are either r or f, that means the first and rest operators are being used
		// Calls the method that finds the correct element or set of elements after first and rest operators
		} else if(strlen(var) == 1 && (*var == 'f' || *var == 'r')) {
			char* e = (char*) malloc((sizeof(char))*10);
			e = firstAndRest(p);
			refer = e;
			return;
		}
		LinkedNode* test = getLL(var);
		// If the first element of the expression is already an expression, that means the program is 
		// dealing with a function with parameters and calls that function to deal with it
		if(test != NULL) {
			newFuncParam(test->name, p);
			return;
	 	}
	 	free(var);
	}
	


	//For every element that the character pointer points to that doesn't equal \0...
	while(*word != '\0') {
		//Declare and allocate space for a new node
		Exp* curr1 = (Exp*) malloc(sizeof(Exp)*1);
		//Store the character data into the new node
		curr1->symbol = word;
		curr1->tp = false;
		//Set curr's next node to the new node
		curr->next = curr1;
		//Set the new node's previous node to curr
		curr1->prev = curr;

		// This deals with the defining of variables when referred to in another function
		if(*word != '(' && *word != ' ' && *word != ')' && *word != '\0' && *word != '\n' 
			&& *word != '-' && *word != '+' && *word != '*' && !isdigit(*word)) {
			char* v = (char*) malloc((sizeof(char))*10);
			int t = 1;
			*v = *word;
			v++;
			word++;
			while(*word != ' ' && *word != ')' && *word != '\0') {
				*v = *word;
				v++;
				word++;
				length++;
				t++;
			}
			v = v - t;
			// Looks for the variable in the symbol tablw
			LinkedNode* missing = getLL(v);
			// This means the variable has not been used yet and simply needs to be initialized
			if(missing->point == NULL) {
				curr1->symbol = v;
				missing->point = curr1;
				word--;
			// The old value needs to be replaced by a new value
			} else {
				Exp* ss = missing->point;
				int m = 0;
				if(strlen(ss->symbol) > 2) {
					*curr1->symbol = *ss->symbol;
					ss->symbol++;
					while(*ss->symbol != '\n' && *ss->symbol != '\0') {
						Exp* new = (Exp*) malloc((sizeof(Exp))*1);
						new->symbol = ss->symbol;
						curr1->next = new;
						new->prev = curr1;
						ss->symbol++;
						curr1 = new;
						m++;
					}
				} else {
					*curr1->symbol = *ss->symbol;
				}
				word--;
			}
		}

		//Increment the curr so that the nodes don't get overwritten
		curr = curr1;
		//Increment the character pointer to point to the next character
		word++;
		//A new node has been added so increase the length variable
		length++;
		//This next block of code checks to see if a subset needs to be created
		//The program detects a subset by checking whether the char pointer is pointing to an open parentheses
		if(*word == '(') {
			//Declares and allocates space for a new Exp node
			Exp* tmp = (Exp*) malloc((sizeof(Exp))*1);
			//Stores the opening parentheses to this new node
			tmp->symbol = word;
			//Sets the tmp node to be the subset of the curr node
			curr->subset = tmp;
			//This Exp pointer simply saves the place curr was at before creating the subset
			Exp* place = curr;
			//Increment curr to be the subset
			curr = curr->subset;
			//Calls a recursive function to help with the creation of the subset
			int d = newExpressionHelp(curr, word);
			//d stores how many characters have been accounted for in this step
			//Increments the word character pointer so that duplicate elements are not created
			word = word + d;
			//Increments the length variable appropriately
			length = length + d;
			//Restores the curr pointer to its original place before the creation of the subset
			curr = place;
		}
	}
	// If the expression is the current expression or will be the current expression, update symbol table
	if(isCurrent) {
		LinkedNode* r = getLL("foo");
		r->point = first;
		r->string = duplicate;
	} else if(name != NULL) {
		LinkedNode* r = getLL(name);
		if(r != NULL)
			r->string = duplicate;
	}
}

/**
 *	Creates a function by defining values of the parameters of another function
 *	@param n - The name of the function with parameters that is being referred to
 *	@param num - The string that contains the defined values of the parameters
 */
void newFuncParam(char* n, char* num) {
	LinkedNode* tmp = getLL(n);
	Exp* g = tmp->point;
	g->tp = true;
	char* d = tmp->data;
	num++;
	// Goes through the rest of the definition string to initialize the parameters
	while(*num != '\0' && *num != ')' && *num != '\n') {
		// Finds the parameters in the referred function
		char* var = (char*) malloc ((sizeof (char))*10);
		int l = 1;
		*var = *d;
		var++;
		d++;
		while(*d != ' ' && *d != '\n') {
			*var = *d;
			var++;
			d++;
			l++;
		}
		d++;
		var = var - l;
		Exp* miss = get(var);

		// If the element has an opening parentheses, that means a subset needs to be created
		if(*num == '(') {
			// Adds an ending parentheses to the expression because it will be missing
			char temp = ')';
			char* temp1 = (char*) malloc((sizeof(char))*1);
			temp1 = &temp;
			miss->symbol = (char*) malloc((sizeof(char))*1);
			*miss->symbol = *temp1;
			Exp* before = miss->prev;
			Exp* mid = (Exp*) malloc((sizeof(Exp))*1);
			mid->symbol = num;
			before->subset = mid;
			// Calls the expression that deals with making subset expressions
			int q = newExpressionHelp(mid, num);
			num = num + q + 2;
			continue;
		} else {
			Exp* before = miss->prev;
			Exp* h = before->subset;
			if(h != NULL)
				before->subset = NULL;
		}

		// Updates the Exp node with the new information
		char* full = (char*) malloc ((sizeof (char))*10);;
		*full = *num;
		int p = 1;
		full++;
		num++;
		while(*num != ' ' && *num != '\n' && *num != ')') {
			*full = *num;
			full++;
			num++;
			p++;
		}
		num++;
		full = full - p;
		miss->symbol = full;
		miss->tp = true;
	}
	// This expression will now be the current expression so update the symbol table
	LinkedNode* r = getLL("foo");
	r->point = tmp->point;
	r->string = tmp->string;
}

/**
 *	Getter method for the refer variable which is a variable that tracks whether a function refers to 
 *	another function
 *	@return - Returns refer
 */
char* getRefer() {
	return refer;
}

/**
 *	Counts how many elements are in the expression
 *	@param x - The starting Exp node
 *	@return - Returns how many elements there are in the stated expression
 */
int count(Exp* x) {
	int c = 0;
	while(x != NULL) {
		c++;
		// Some special cases that need to be accounted for in my specific implementation
		if(*x->symbol != ' ' && *x->symbol != '\n' && *x->symbol != '\0' && isdigit(*x->symbol) && x->tp) {
			c = c + strlen(x->symbol) - 1;
		} else if(!isdigit(*x->symbol) && *x->symbol != ' ' && *x->symbol != '\n' && *x->symbol != '\0'
			&& *x->symbol != '+' && *x->symbol != '-' && *x->symbol != '*' && *x->symbol != '(' 
			&& *x->symbol != ')') {
			c = c + strlen(x->symbol) - 1;
		}
		// Recursively calls this function on the subset expression
		if(x->subset != NULL) {
			Exp* y = x;
			x = x->subset;
			c = c + count(x);
			x = y;
		}
		x = x->next;
	}
	return c;
}

/**
 *	Getter method for the first Exp node of the expression it is currently pointed at
 *	@return - Returns the first Exp node
 */
Exp* getFirst() {
	return first;
}

/** 
 * A helper method to help create subsets of the expression by using recursion
 * @param x - The node pointer where the subset begins
 * @param word - A character pointer containing data to be stored in data structure format
 * @return - Returns an integer representing the number of characters that have been added to data structure
 */
int newExpressionHelp(Exp* x, char* word) {
	//Saves the first character of the subset into the data structure
	x->symbol = word;
	x->tp = false;
	//Increments the character pointer to point to the next character
	word++;
	//Added another node to the data structure so increase the length
	length++;
	//This variable counts how many characters have been stored in the data structure
	int wordInc = 1;
	//While the chracter pointer does not point to a null character \0...
	while(*word != '\0') {
		//Declare and allocate space for a new node
		Exp* curr1 = (Exp*) malloc(sizeof(Exp)*1);
		//Save the character to this node
		curr1->symbol = word;
		curr1->tp = false;
		//Set x's next node to be this newly created node
		x->next = curr1;
		//Set the new nodes' previous node to be x
		curr1->prev = x;
		
		// Deals with any variables in the subset part of the expression
		if(*word != '(' && *word != ' ' && *word != ')' && *word != '\0' && *word != '\n' 
			&& *word != '-' && *word != '+' && *word != '*' && !isdigit(*word)) {
			char* v = (char*) malloc((sizeof(char))*10);
			int t = 1;
			*v = *word;
			v++;
			word++;
			wordInc++;
			while(*word != ' ' && *word != ')' && *word != '\0') {
				*v = *word;
				v++;
				word++;
				wordInc++;
				t++;
			}
			v = v - t;
			LinkedNode* missing = getLL(v);
			curr1->symbol = v;
			missing->point = curr1;
			word--;
			wordInc--;
		}


		//Iterate the expression once so that elements are not overwritten
		x = curr1;
		//Increment the character pointer to point to the next character
		word++;
		//Increment the counter variable
		wordInc++;
		//There is another subset if there is another opening parentheses
		if(*word == '(') {
			//Declare and allocate space for a new Exp node
			Exp* tmp = (Exp*) malloc((sizeof(Exp))*1);
			//Save the opening parentheses to the new node
			tmp->symbol = word;
			//Update x's subset to be this newly created node
			x->subset = tmp;
			//Save the place of x before it goes to update the subset of the expression
			Exp* place = x;
			//Iterate to the subset
			x = x->subset;
			//Recursively call this method to create another subset
			int d = newExpressionHelp(x, word);
			//Increment the word character pointer so that there are no repeated elements
			word = word + d;
			//Increment the counter variable
			wordInc = wordInc + d;
			//Restore x to point to its original place before it created a subset
			x = place;
			//If the character pointer points to a closing parentheses, the expression is not longer a subset
		} else if(*word == ')') {
			//Decrease the length since the closing parentheses will not be part of the subset
			length--;
			//Return how many characters have been added to the data structure
			return wordInc;
		}
	}
	//Return how many characters have been added to the data structure
	return wordInc;
}

/**
 *	Given a list of function names, append all other functions to the first function
 *	@param word - The string that contains the function names
 *	@param name - the name of the function that the result will be saved to
 */
void appendExp(char* word, char* name) {
	char* var = (char*) malloc((sizeof(char))*10);
	char** m = (char**) malloc((sizeof(char*))*10);
	int l = 0;
	int t = 0;

	// Goes through all of the elements in the data string
	while(*word != ')') {
		if(*word != ' ') {
			*var = *word;
			var = var - l;
			LinkedNode* tmp = getLL(var);
			l++;
			if(tmp != NULL) {
				*m = tmp->string;
				m++;
				t++;
				var = (char*) malloc((sizeof(char))*10);
				l = 0;
			}
			var = var + l;
		}
		word++;
	}
	// Uses string manipulation to append the first two functions
	m = m - t;
	char* x = (char*) malloc((sizeof (char))*100);
	char* y = (char*) malloc((sizeof (char))*100);
	memmove(x, *m, strlen(*m) - 2);
	m++;
	t--;
	memmove(y, *m, strlen(*m) - 1);
	strcat(x, " ");
	strcat(y, ")\n");
	strcat(x, y);
	m++;
	t--;
	// Calls a recursive function that appends the rest of the functions if necessary
	char* result = appendExpHelp(m, x, t);
	// Creates a new function with this newly created string with all appended functions
	newExpression(result, false, name);
}

/**
 *	Uses recursion to repeatedly append functions to each other
 *	@param end - A double character pointer that points to the appropriate string version of expressions
 *	@param start - The current resulting string
 *	@param count - How many appends are left
 *	@return - Returns the final expression in string format
 */
char* appendExpHelp(char** end, char* start, int count) {
	if(count <= 0)
		return start;
	char* x = (char*) malloc((sizeof (char))*100);
	char* y = (char*) malloc((sizeof (char))*100);
	memmove(x, start, strlen(start) - 2);
	memmove(y, *end, strlen(*end) - 1);
	strcat(x, " ");
	strcat(y, ")\n");
	strcat(x, y);
	end++;
	count--;
	return appendExpHelp(end, x, count);
} 

/**
 *	Traverses a given expression looking for the correct node or set of nodes using first and rest operators
 *	@param s - The first Exp node of the expression
 *	@param word - String containing the first and rest operators
 *	@param num - Number of characters before reaching the beginning of the string starting from function name
 */
void traverse(Exp* s, char* word, int num) {
	bool sub = false;
	bool sub1 = false;
	// Goes through the input string and finds which case the current expression is dealing with
	while(num > 0 && s != NULL) {
		if((*s->symbol == ')' || *s->symbol == '(') && *word == 'r' && sub)
			s = s->next;
		if(*word == 'r' && num == 2) {
			s = s->next;
			sub1 = true;
		} else if(*word == 'r') {
			s = s->next;
			s = s->next;
			sub = false;
		} else if(*word == 'f' && s->subset != NULL) {
			s = s->subset;
			sub = true;
		}
		word--;
		num--;
	}
	// Solves the problem according to which case
	if(s->next != NULL && !sub && !sub1) {
		if(*s->symbol == ' ')
			s = s->next;
		char* x = (char*) malloc((sizeof(char))*10);
		*x = *s->symbol;
		x++;
		Exp* u = s->next;
		// This case is if the desired element is a single digit less than 10
		if(isdigit(*s->symbol) && *s->symbol != '(' && !isdigit(*u->symbol)) {
			Exp* n = (Exp*) malloc((sizeof(Exp))*1);
			*x = '\n';
			n->symbol = x;
			x--;
			Exp* m = (Exp*) malloc((sizeof(Exp))*1);
			m->symbol = x;
			m->next = n;
			n->prev = m;
			first = m;
		// This case happens if the desired element is a single digit greater than or equal to 10
		} else if(isdigit(*u->symbol)){
			Exp* start = (Exp*) malloc((sizeof(Exp))*1);
			char tt = *u->symbol;
			char* tt1 = (char*) malloc((sizeof(char))*10);
			*tt1 = tt;
			start->symbol = tt1;
			x--;
			Exp* m = (Exp*) malloc((sizeof(Exp))*1);
			m->symbol = x;
			m->next = start;
			start->prev = m;
			s = s->next;
			s = s->next;
			while(s != NULL && *s->symbol != ' ') {
				Exp* n = (Exp*) malloc((sizeof(Exp))*1);
				*tt1 = *s->symbol;
				n->symbol = tt1;
				start->next = n;
				n->prev = start;
				start = n;
				s = s->next;
			}
			// Adds a line break to the expression
			Exp* h = (Exp*) malloc((sizeof(Exp))*1);
			char yy = '\n';
			char* yy1 = (char*) malloc((sizeof(char))*1);
			*yy1 = yy;
			h->symbol = yy1;
			start->next = h;
			h->prev = start;
			first = m;
		}
	// This case happens if the desired set of nodes is in a subset
	} else if(sub) {
		char* x = (char*) malloc((sizeof(char))*1);
		char v = *s->symbol;
		char* y = (char*) malloc((sizeof(char))*1);
		Exp* m = (Exp*) malloc((sizeof(Exp))*1);
		m->symbol = s->symbol;
		m->tp = false;
		first = m;
		s = s->next;
		while(s != NULL) {
			Exp* n = (Exp*) malloc((sizeof(Exp))*1);
			n->symbol = s->symbol;
			n->tp = false;
			m->next = n;
			n->prev = m;
			if(s->subset != NULL) {
				Exp* a = s->subset;
				newExpressionHelp(a, a->symbol);
				n->subset = a;
			}
			m = n;
			s = s->next;
		}
		// Adds the ending parentheses and line break to the expression
		v = ')';
		*y = v;
		Exp* z = (Exp*) malloc((sizeof(Exp))*1);
		z->symbol = y;
		z->tp = false;
		m->next = z;
		z->prev = m;
		char q = '\n';
		*x = q;
		Exp* w = (Exp*) malloc((sizeof(Exp))*1);
		w->tp = false;
		w->symbol = x;
		z->next = w;
		w->prev = z;
	// This case occurs when the operator ends on a rest
	} else if(sub1) {
		char* x = (char*) malloc((sizeof(char))*1);
		char v = *s->symbol;
		char* y = (char*) malloc((sizeof(char))*1);
		Exp* m = (Exp*) malloc((sizeof(Exp))*1);
		m->symbol = s->symbol;
		m->tp = false;
		first = m;
		s = s->next;
		while(s != NULL) {
			Exp* n = (Exp*) malloc((sizeof(Exp))*1);
			n->symbol = s->symbol;
			n->tp = false;
			m->next = n;
			n->prev = m;
			if(s->subset != NULL) {
				Exp* a = s->subset;
				newExpressionHelp(a, a->symbol);
				n->subset = a;
			}
			m = n;
			s = s->next;
		}
	}
}

/**
 *	Finds the expression referred to in a function of first and rest operators
 *	@param word - input string that contains the operators and the referred funcion
 *	@return - Returns the name of the expression that is referred to
 */
char* firstAndRest(char* word) {
	word = word + 3;
	char* var = (char*) malloc((sizeof(char))*10);
	Exp* start = (Exp*) malloc((sizeof(Exp))*1);
	bool t = true;
	int l = 0;
	int num = 5;

	//	Finds the expression referred to ie (f (r (r (r b10)))) ---> finds b10
	while(*word != ')') {
		if(*word == 'r' || *word == 'f') {
			word++;
			if(*word == ' ')
				t = false;
			word--;
		}
		if(*word != ' ' && *word != '(' && t) {
			*var = *word;
			var = var - l;
			word++;
			l++;
			if(*word == ')') {
				// Once a valid expression is found, save it and break the loop
				LinkedNode* tmp = getLL(var);
				if(tmp != NULL) {
					start = tmp->point;
					word = word - l - 1;
					num = num - l;
					break;
				}
			}
			word--;
			var = var + l;
		}
		t = true;
		word++;
		num++;
	}

	//	Traverses the found expression and finds the desired expression
	traverse(start, word, num);
	return var;
}

/**
 * Evaluates the expression currently in the data structure
 * @param f - The starting node of the expression
 * @return - Returns the calculated result of the expression
 */
int evaluateExpression(Exp* f) {
	//Initializes the variable that will contain the final answer of the expression
	int ans = 0;
	//A pointer that points to a chracter which represents which operation the algorithm should perfrom
	char* sign;
	//An array that contains the integers that will later be operated on
	int len = count(f);
	int list[len];
	//An index variable that increments such that elements will not be overwritten
	int idx = 0;
	//Start from the first node of the desired expression
	Exp* curr = f;
	//While the node is not null
	while(curr != NULL) {
		//If the symbol of the node is an operation...
		if((*curr->symbol == '+') || (*curr->symbol == '-') || (*curr->symbol == '*')) {
			//Have sing point to this character
			sign = curr->symbol;
			//If the symbol is any integer...
		} else if((*curr->symbol == '0') || (*curr->symbol == '1') || (*curr->symbol == '2') 
			|| (*curr->symbol == '3') || (*curr->symbol == '4') || (*curr->symbol == '5') 
			|| (*curr->symbol == '6') || (*curr->symbol == '7') || (*curr->symbol == '8')
			|| (*curr->symbol == '9')) {
			//This character pointer will store the entire integer
			char* tmp = curr->symbol;
			//This variable keeps track of the length of the character pointer
			int tmpL = 0;
			//An Exp object that will be used to iterate the rest of the integer
			Exp* n = NULL;
			//Will keep track of if n has become a null node
			bool h = false;
			if(curr->next != NULL) {
				//Initializes n to the next node of curr
				n = curr->next;
				//n is not a null node
				h = true;
				//While n is not a space or a closing parentheses...
				while((*n->symbol != ' ') && (*n->symbol != ')')) {
					//Increment the pointer storing the integer
					tmp++;
					//Increments its length
					tmpL++;
					//Point to the next integer
					tmp = n->symbol;
					//Go to the next node
					n = n->next;
					//But if the null is now null...
					if(n == NULL) {
						//n is a null node
						h = false;
						//break out of the loop
						break;
					}
				}
			}
			if(h) {
				curr = n; 		//Iterate curr to be the appropriate node that has not been accounted for yet
			}
			//Decrement the pointer to point at the first digit of the integer
			tmp = tmp - tmpL;
			//Convert this character pointer to be an integer
			int t = atoi(tmp);
			//Add this integer to the list
			list[idx] = t;
			//Increase the index variable
			idx++;
		}
		//Checks to see if the current node has a subset
		if(curr->subset != NULL) {
			//Calls a helper method that solves the expression in the subset and saves it in a variable
			int subS = evaluateExpressionHelp(curr->subset);
			//Add this integer to the list
			list[idx] = subS;
			//Increase the index variable
			idx++;
		}
		//Increment to the next node in the expression
		curr = curr->next;
	}
	//This is where the final answer is computed

	ans = eval(*sign, list, idx);
	//Return the final answer
	return ans;
}

 /**
  * A helper method for evaluate and recursively calls itself when there is another subset
  * @param u - The Exp node pointer at which the subset begins
  * @return - Returns the result of the expression in the subset
  */
int evaluateExpressionHelp(Exp* u) {
	//Stores the answer of the subset that will be returned
	int ans = 0;
	//List that contains the integers that will be operated on
	int len = count(u);
	int y[len];
	//Character pointer points to the operation that will be performed
	char* s;
	//Index variable for the list
	int idx = 0;
	Exp* x = u;
	//While x is not null
	while(x != NULL) {
		//If the symbol is an appropriate operation...
		if((*x->symbol == '+') || (*x->symbol == '-') || (*x->symbol == '*')) {
			//Point to that symbol
			s = x->symbol;
			//Else if the symbol is any integer
		} else if((*x->symbol == '0') || (*x->symbol == '1') || (*x->symbol == '2') 
			|| (*x->symbol == '3') || (*x->symbol == '4') || (*x->symbol == '5') 
			|| (*x->symbol == '6') || (*x->symbol == '7') || (*x->symbol == '8') || (*x->symbol == '9')) {
			//This character pointer will store the entire integer
			char* tmp = x->symbol;
			//Keeps track of the number of digits in the integer
			int tmpL = 0;
			//Tracks if the node n is null or not
			bool h = false;
			Exp* n = NULL;
			//If x is not null...
			if(x->next != NULL) {
				//n now takes on the values of x
				n = x->next;
				//And n is not null
				h = true;
				//While n contains a character that is an integer...
				while((*n->symbol != ' ') && (*n->symbol != ')')) {
					//increment the character pointer
					tmp++;
					//And increase its length
					tmpL++;
					//point to the digit
					tmp = n->symbol;
					//Iterate to the next node
					n = n->next;
					//But if that node is null...
					if(n == NULL) {
						//Update h to indicate that and break the loop
						h = false;
						break;
					}
				}
			}
			//Make sure x is at the right place if a null node is not encountered
			if(h) {
				x = n;
				//If a null node is encountered this means there are no further elements in the subset
			} else if(!h) {
				//Decrement the character pointer to point to the first digit in the integer
				tmp = tmp - tmpL;
				//Convert this character pointer to be an integer
				int t = atoi(tmp);
				//Add this integer to the list
				y[idx] = t;
				//Increment the index variable
				idx++;
				//Break out of this outer loop
				break;
			}
			//If h is true, that means there may be more elements to evaluate so keep interating Exp structure
			tmp = tmp - tmpL;
			//Convert this character pointer to be an integer
			int t = atoi(tmp);
			//Add this integer to the list
			y[idx] = t;
			//Increment the index variable
			idx++;
		}
		//Checks if there is a subset
		if(x->subset != NULL) {
			//Recursively calls itself to solve the subset
			int subS = evaluateExpressionHelp(x->subset);
			//Add the evaluated value to the list
			y[idx] = subS;
			//Increment the index variable
			idx++;
		}
		//Iterate x to the next node in the data structure
		x = x->next;
	}
	//This solves the final solution to return to the last time this method was called
	//If s character pointer is pointing to a plus...
	ans = eval(*s, y, idx);
	//Return the final answer
	return ans;
}

/**
 *	Given the sign, list of integers, and how many integers are in the list, computes the answer
 *	@param s - The sign that indicates which operations, either + - *
 *	@param y - The list of integers that will be operated on
 *	@param idx - The number of relevant integers in the list
 *	@return - returns the final computed answer
 */
int eval(char s, int y[], int idx) {
	int ans = 0;
	if(s == '+') {
		//Initialize a variable to go through the list of integers
		int i = 1;
		//ans is the first element in the list
		ans = y[0];
		//Goes through the list of integers
		while(i < idx) {
			//Saves the ith integer to w
			int w = y[i];
			//Adds this integer to ans
			ans = ans + w;
			//Goes to the next value in the list
			i++;
		}
		//If s is pointing to a minus...
	} else if(s == '-') {
		//Initialize a variable to go through the list of integers
		int i = 1;
		//ans is now the first element in the list
		ans = y[0];
		//Go through the entire list of integers
		while(i < idx) {
			//Saves the ith integer to w
			int w = y[i];
			//Subtract w from ans
			ans = ans - w;
			//Goes to the next value in the list
			i++;
		}
		//If s is pointing to a multiplication sign...
	} else if(s == '*') {
		//Ans is the first element in the integer list
		ans = y[0];
		//Initialize variable to go through the list of integers
		int i = 1;
		//Go through the entier list of integers
		while(i < idx) {
			//Saves the ith integer to w
			int w = y[i];
			//If the current and next integer are both 0 that means there are no more relevant integers
			if((w == 0) && (y[i + 1] == 0) && (i != idx - 1)) {
				break;
			}
			//Multiply w with the ans
			ans = ans * w;
			//Go to the next integer
			i++;
		}
	}
	return ans;
}

/* 
 * Takes in as input a character pointer that indicates which subset of the expression should be printed
 * @param com - Character pointer that contains the information to print the correct subset
 */
void subsetExpression(char* com) {				//r r r r r f r r r f r r r f r r
	//Checks to see if the program has went to a subset of an expression or not
	bool sub = false;				//(* 2 (+ 3 4) (- 6 5) (* 7 8) (+ 9 10 (- 12 11 (+ 13 14) (+ 16 15))) 17)
	Exp* tmp = first;
	//The first Exp node points to an opening parentheses which doesn't matter in this method
	tmp = tmp->next;
	//Searches for an f in the input character pointer
	while(*com != 'f') {
		//If the pointer is pointing to an 'r', skip to the next node in the data structure
		if(*com == 'r') {
			tmp = tmp->next;
			//There could be instances where there are integers larger than one digit
			//Keep skipping until a space is reached
			while(*tmp->symbol != ' ') {
				tmp = tmp->next;
			}
			//Increments to the value after the space
			tmp = tmp->next;
			//Skips the 'r' in the input character pointer
			com++;
		}
		//Skips the space in the input character pointer
		com++;
	}
	//Saves the place where the tmp Exp pointer was
	Exp* find = tmp;
	//Goes back one
	find = find->prev;
	//Checks if there is a subset of the expression
	if(find->subset != NULL) {
		//Uses a helper method to print those values
		subsetExpressionHelp(find->subset, com, false);
		//The program has gone into a subset
		sub = true;
	}
	//This part of the program is used to print out elements that are not in the subset
	while(tmp != NULL) {
		if((*tmp->symbol == ')') || (*tmp->symbol == ' ')) {
			break;
		}
		//Prints the character saved into the node
		printf("%c", *tmp->symbol);
		//Goes to the next node
		tmp = tmp->next;
		//If there is an opening parentheses, a subset expression exists
		if(*tmp->symbol == '(') {
			sub = true;
		}
	}
	//Need to print the closing parentheses since the closing parentheses is not part of subset expression
	if(sub) {
		printf("%c", ')');
	}
	//Skips the line for aesthetic
	printf("\n");
}

/*
 * A helper method that helps the subset method with printing out values in the subset expression
 * @param x - The Exp node pointer at which the subset begins
 * @param in - The subset that is desired by the user
 * @param rec - When there is a subset expression within a subsetexpression, helps the recursion figure out
 * 				whether it should print out a single data element or multiple (f or r)
 */
void subsetExpressionHelp(Exp* x, char* in, bool rec) {
	//Determines whether 'f' is the first character that in points to
	bool more = false;
	//Tracks whether the first while loop is broken or not
	bool broke = false;
	//Prints the first element of the subset expression
	printf("%c", *x->symbol);
	//Iterates to the next node in the subset expression
	x = x->next;
	Exp* tmp = x;
	//The above x = x->next statement might not get printed so these next few lines print and add a space
	char* k = x->symbol;
	if((*k != ' ') && (*k != ')') && (*k != '(') && rec) {
		printf("%c", *k);
		printf("%c", ' ');
	}
	//While the character in is pointing at is not a null character...
	while(*in != '\0') {
		if(*in == 'r') {
			//Increment to the next Exp node
			tmp = tmp->next;
			//If tmp is not a space that means the integer is more than one digit and this needs to be skipped
			while(*tmp->symbol != ' ') {
				tmp = tmp->next;
			}
			//Skip the space
			tmp = tmp->next;
			//Increment the in character pointer
			in++;
			//If the first thing in points to is f break out of this loop
		} else if((*in == 'f') && more) {
			broke = true;
			break;
		}
		//no longer the first iteration
		more = true;
		//Increment the character pointer
		in++;
	}
	//Decrements in to equal 'r'
	if(!broke) {
		in = in - 2;
	}
	if(*in == 'r') {
		//Prints all necessary data elements
		while(tmp != NULL) {
			//Checks to see whether the Exp node contains a subset expression
			Exp* u = tmp->prev;
			if(u->subset != NULL) {
				subsetExpressionHelp(u->subset, in, true);
			}
			//Prints the value of that Exp node
			printf("%c", *tmp->symbol);
			//Iterates to the next Exp node
			tmp = tmp->next;
		}
	} else if(*in == 'f') {
		//Makes sure program doesn't print other subsets
		bool p = true;
		//When in points to f it will go to a subset expression
		while(tmp != NULL) {
			//Checks to see whether the u contains a subset expression 
			Exp* u = tmp->prev;
			//Checks to see whether the Exp node has a subset expression and whether it is the correct subset
			if(u->subset != NULL && p) {
				subsetExpressionHelp(u->subset, in, false);
				//Once a subset is found the other subsets are no longer valid in this case
				p = false;
			}
			//Prints the data element
			printf("%c", *tmp->symbol);
			//Iterates to the next Exp node
			tmp = tmp->next;
		}
	}
}

/**
 * Prints the expression currently stored in the data structure given the first Exp node
 * @param f - The first Exp node of the expression
 */
void printExpression(Exp* f) {
	//Starts at the first node in the data structure
	Exp* curr = f;
	//Stores the size of the data so that the global variable is unaffected
	int tmp = count(curr);
	//For every element in this data structure...
	while(tmp > 0) {
		//Save the data the Exp node is pointing to
		char* data = curr->symbol;
		char op = *data;
		int track = 0;
		while(!isdigit(op) && op != '\0' && op != ' ' && op != ')' && op != '(' && op != '\n' && op != '+'
			&& op != '*' && op != '-') {
			tmp--;
			printf("%c", op);
			curr->symbol++;
			op = *curr->symbol;
			track++;
		}
		if(track == 1)
			tmp++;
		data++;
		if(*data != ' ' && *data != '\n' && *data != '\0' && isdigit(*data) && curr->tp) {
			data--;
			tmp = tmp - strlen(data) + 1;
			printf("%s", data);
		} else {
		//Print it
			printf("%c", op);
		}
		//Check if that node has a subset expression
		if(curr->subset != NULL) {
			tmp = printExpressionHelp(curr, tmp);
		}
		//Iterate to the next node in the data structure
		curr = curr->next;
		//Decrement the size
		tmp--;
	}
}

/* 
 * A helper method that assists the print method in printing the elements in the subset expressions
 * @param x - The starting node of the subset expression
 * @param lgth - The current size left in the data structure
 * @return - Returns how many elements left to be printed so that tmp can be updated in the print method
 */
int printExpressionHelp(Exp* x, int lgth) {
	Exp* y = x->subset;
	bool t = false;
	//For ever element in the subset expression...
	while(y != NULL) {
		//Save the data the Exp node is pointing to
		char* data = y->symbol;
		char op = *data;
		int track = 0;
		while(!isdigit(op) && op != '\0' && op != ' ' && op != ')' && op != '(' && op != '\n' && op != '+'
			&& op != '*' && op != '-') {
			printf("%c", op);
			y->symbol++;
			op = *y->symbol;
			lgth--;
			t = true;
			track++;
		}
		if(track == 1)
			lgth++;
		data++;
		if(*data != ' ' && *data != '\n' && *data != '\0' && isdigit(*data) && y->tp) {
			data--;
			lgth = lgth - strlen(data) + 1;
			printf("%s", data);
		} else {
		//Print it
			printf("%c", op);
		}
		//Check for further subset expressions
		if(y->subset != NULL) {
			//Recursively call this method to deal with more subset expressions
			int d = printExpressionHelp(y, lgth);
			//Update the size of the data structure
			lgth = d;
		}
		//Iterate to the next node in the data structure
		y = y->next;
		//Decrement the size
		lgth--;
	}
	if(t)
		lgth = lgth + 2;
	//Return the number of elements left to print out
	return lgth;
}