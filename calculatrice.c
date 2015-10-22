#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_END 0
#define PROGRAM_CONTINUE 1
#define SYNTAX_ERROR 2
#define MALLOC_ERROR 3
#define VAR_ERROR 4

#include "number.c"

struct VariableNode {
	Number *number;
	char name;
	struct VariableNode *next;
};

struct VariableNode *listVariable_head = NULL;

void deleteVariableList(){
	struct VariableNode *headNode = listVariable_head,
						*currentNode, 
						*tempNode;

	while(headNode != NULL){
		// Check if many variables point to the same number
		tempNode = headNode;
		currentNode = headNode->next;
		while(currentNode != NULL){
			if (currentNode->number == headNode->number){
				tempNode->next = currentNode->next;
				free(currentNode);
				currentNode = tempNode->next;
			}
			else{
				tempNode = currentNode;
				currentNode = currentNode->next;
			}
		}
		// Set up next iteration and free headNode
		tempNode = headNode;
		headNode = headNode->next;
		deleteNumber(tempNode->number);
		free(tempNode);
		tempNode = NULL;
	}
	listVariable_head = NULL;
}

void deleteNumberIfNotAVariable(Number *number){
	struct VariableNode *currentNode = listVariable_head;
	// Check if the number is referenced by any variable
	while(currentNode != NULL){
		if(currentNode->number == number)
			return;
		currentNode = currentNode->next;
	}
	// Delete number if it isn't referenced by any variable
	deleteNumber(number);
}

Number* getVariable(char c){
	struct VariableNode *currentNode = listVariable_head;
	while (currentNode != NULL){
		if (currentNode->name == c)
			return currentNode->number;
		currentNode = currentNode->next;
	}
	return NULL;
}

struct VariableNode* createVariableNode(Number *n, char c){
	struct VariableNode *newNode = malloc(sizeof(struct VariableNode));
	if (newNode == NULL){
		printf("ERREUR MEMOIRE !!!!!!!!!!!!!!\n");
		return NULL;
	}
	newNode->number = n;
	newNode->name = c;
	newNode->next = NULL;
	return newNode;
}

void assignNumberToVar(Number *n, char c){
	struct VariableNode *currentNode = listVariable_head;
	Number *tempNumber;
	// Is the variable list empty?
	if (currentNode == NULL){
		listVariable_head = createVariableNode(n,c);
		return;
	}
	// Variable already assigned to something?
	while (1){
		if (currentNode->name == c){
			tempNumber = currentNode->number;
			currentNode->number = n;
			deleteNumberIfNotAVariable(tempNumber);
			return;
		}
		if (currentNode->next == NULL){
			currentNode->next = createVariableNode(n,c);
			return;
		}
		currentNode = currentNode->next;
	}
}

#include "stack.c"

int correctSyntax(char c){
	if (c != ' ' || c != '\0' || c != '\n'){
		printf("Erreur de syntaxe: Les variables doivent etre composees d'un seul charactere.\n");
		return 0;
	}
	return 1;
}

int readCommandLine(){
	initNumberStack();
	printf("> ");

	char c;
	Number *n1, *n2, *ntemp;
	while ((c = getchar()) != '\0'){
		if ((c >= '0') && (c <= '9')) {
			push(createNumberFromWordCommandLine(c));
		}
		else if (c == ' ');
		else if (c == '+') {
			n2 = pop();
			n1 = pop();
			ntemp = addNumbers(n1,n2);
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c == '-') {
			n2 = pop();
			n1 = pop();
			ntemp = substractNumbers(n1,n2);
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c == '*') {
			n2 = pop();
			n1 = pop();
			ntemp = multiplyNumbers(n1,n2);
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c >= 'a' && c <= 'z') {
			ntemp = getVariable(c);
			push(ntemp);
		}
		else if (c == '=') {
			c = getchar();
			ntemp = pop();
			assignNumberToVar(ntemp, c);
			push(ntemp);
		}
		else if (c == EOF)
			return PROGRAM_END;
		else if (c == '\n')
			break;
		else
		{
			printf("Erreur syntaxe: Symbole %c invalide.\n", c);
			break;
		}
	}

	n1 = pop();
	ntemp = pop();
	if (n1 == NULL){
		printf("Erreur de syntaxe: Pas assez d'arguments\n");
		return SYNTAX_ERROR;
	}
	if (ntemp != NULL){
		printf("Erreur de syntaxe: Trop d'arguments\n");
		deleteNumberIfNotAVariable(ntemp);
		deleteNumberIfNotAVariable(n1);
		return SYNTAX_ERROR;
	}

	printNumber(n1);
	deleteNumberIfNotAVariable(n1);
	return PROGRAM_CONTINUE;
}

int main(){
	int code;
	while(code = readCommandLine())
	{

	}
	deleteVariableList();
	deleteStack();
	return 0;
}