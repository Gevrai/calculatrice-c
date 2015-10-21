#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_END 0
#define SYNTAX_ERROR 1
#define MALLOC_ERROR 2
#define VAR_ERROR 3

#include "stack.c"

struct VariableNode {
	Number *number;
	char name;
	struct VariableNode *next;
};

struct VariableNode *listVariable_head = NULL;

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
	// Is the variable list empty?
	if (currentNode == NULL){
		listVariable_head = createVariableNode(n,c);
		return;
	}
	// Variable already assigned to something?
	while (1){
		if (currentNode->name == c){
			currentNode->number = n;
			return;
		}
		if (currentNode->next == NULL){
			currentNode->next = createVariableNode(n,c);
			return;
		}
		currentNode = currentNode->next;
	}
}

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
		}
		else if (c == '-') {
			n2 = pop();
			n1 = pop();
			ntemp = substractNumbers(n1,n2);
			push(ntemp);
		}
		else if (c == '*') {
			n2 = pop();
			n1 = pop();
			ntemp = multiplyNumbers(n1,n2);
			push(ntemp);
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

	ntemp = pop();
	if (ntemp == NULL){
		printf("Erreur de syntaxe: Pas assez d'arguments\n");
		return SYNTAX_ERROR;
	}
	if (pop() != NULL){
		printf("Erreur de syntaxe: Trop d'arguments");
		return SYNTAX_ERROR;
	}

	printNumber(ntemp);
}

int main(){
	int code;
	while((code = readCommandLine()) != PROGRAM_END)
	{

	}
	return 0;
}