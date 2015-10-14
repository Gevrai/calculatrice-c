#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "number.c"

struct stackNode {Number* content; struct stackNode *next;};

typedef struct stack_t{
	struct stackNode* topNode;
	int nbElements;
} Stack;

Stack* createStack(){
	Stack *stack_p = malloc(sizeof(Stack));
	if (stack_p == NULL){
		printf("Can't allocate memory for stack!\n"); //------------------------------------------------------------- ERROR MESSAGE
		return NULL;
	} else {	
		stack_p->topNode = NULL;
		stack_p->nbElements = 0;
		return stack_p;
	}
}

void push(Stack *stack_p, Number* elementToAdd){
	//Allocate memory for new Node
	struct stackNode *newStackTop = malloc(sizeof(struct stackNode));
	//Construct the new top Node
	newStackTop->content = elementToAdd;
	newStackTop->next = stack_p->topNode;
	//Update the stack
	stack_p->topNode = newStackTop;
	(stack_p->nbElements)++;
}

Number* pop(Stack *stack_p){
	struct stackNode *currentTopNode = stack_p->topNode;

	// Empty Stack?
	if (currentTopNode == NULL){
		printf("Stack is currently empty.\n");
		return NULL;
	}

	// Fetch top value
	Number* c = currentTopNode->content;
	// Update the stack
	stack_p->topNode = currentTopNode->next;
	(stack_p->nbElements)--;
	free(currentTopNode);
	return c;
}

int main(){
	Stack *stack_p = createStack();
	if (stack_p == NULL) return 0;

	Number* number_p;
	char c;
	while ((c = getchar()) != EOF){
		if (c =='p'){
			number_p = pop(stack_p);
			if (number_p != NULL) printNumber(number_p);
		}
		else if (c != '\n'){
			number_p = createNumberFromWordCommandLine(1, c);
			push(stack_p, number_p);
		}
	}
	return 0;
}