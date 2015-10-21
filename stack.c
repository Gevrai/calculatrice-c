#include "number.c"

struct stackNode {Number* number; struct stackNode *next;};

typedef struct stack_t{
	struct stackNode* topNode;
} NumberStack;

NumberStack *stack = NULL;

void initNumberStack(){
	struct stackNode *currentStackNode, *tempNode;
	// Stack already exists?
	if (stack != NULL){
		currentStackNode = stack->topNode;
		while (currentStackNode != NULL){
			deleteNumber(currentStackNode->number);
			tempNode = currentStackNode;
			currentStackNode = currentStackNode->next;
			free(tempNode);
		}
		stack->topNode = NULL;
		return;
	}
	// Stack doesn't exist
	stack = malloc(sizeof(NumberStack));
	if (stack == NULL){
		printf("Can't allocate memory for stack!\n"); //------------------------------------------------------------- ERROR MESSAGE
		return;
	}
	
	stack->topNode = NULL;
	return;
}

void push(Number* elementToAdd){
	//Allocate memory for new Node
	struct stackNode *newStackTop = malloc(sizeof(struct stackNode));
	if (newStackTop == NULL){
		printf("le malloc a foirer");
	}
	//Construct the new top Node
	newStackTop->number = elementToAdd;
	newStackTop->next = stack->topNode;
	//Update the stack
	stack->topNode = newStackTop;
}

Number* pop(){
	struct stackNode *currentTopNode = stack->topNode;
	// Empty Stack?
	if (currentTopNode == NULL){
		return NULL;
	}
	// Fetch top value
	Number* number = currentTopNode->number;
	// Update the stack
	stack->topNode = currentTopNode->next;
	free(currentTopNode);
	return number;
}