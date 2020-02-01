#include "expression.h"
#ifndef LINKEDNODE
#define LINKEDNODE
typedef struct {
	Exp* point;
	char* name;
	char* string;
	char* refersTo;
	char* data;
	struct LinkedNode* prev;
	struct LinkedNode* next;
} LinkedNode;

extern void createLinkedList(Exp*, char*);
extern void insertInLinkedList(Exp*, char*);
extern Exp* get(char*);
extern LinkedNode* getLL(char*);
extern LinkedNode* getFirstLN();

#endif