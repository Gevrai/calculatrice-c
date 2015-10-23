#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_CONTINUE 0
#define CRITICAL_ERROR 1
#define PROGRAM_END 2
#define SYNTAX_ERROR 3
#define MALLOC_ERROR 4
#define VAR_ERROR 5
#define NUMBER_ERROR 6
#define ARGUMENT_ERROR 7

int errorCode = PROGRAM_CONTINUE;
char errMsg[64] = "";

#include "number.c"

int throwError(){
	/*send an error name and message here to have them printed out
	Also check if the error is trivial (ret 0) or if the memory had to be dumped (ret 1)
	format for error throwing in the code is:
	
		errorCode = ERROR_TYPE;
		strcpy(errMsg, "Error message\n");
		
	The actual throwing is done in readCommandLine() automatically.
	*/
	switch (errorCode)
	{
		//Program end
		case PROGRAM_END:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return PROGRAM_END;
		//Syntax error
		case SYNTAX_ERROR:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return PROGRAM_CONTINUE;
		//Memory error
		case MALLOC_ERROR:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return CRITICAL_ERROR;
		//Variable error
		case VAR_ERROR:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return PROGRAM_CONTINUE;
		//Stack argument error
		case ARGUMENT_ERROR:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return PROGRAM_CONTINUE;
		//Number error
		case NUMBER_ERROR:
			printf("%s\n", errMsg);
			errorCode = PROGRAM_CONTINUE;
			return CRITICAL_ERROR;
	}
	return PROGRAM_CONTINUE;
}

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
		errorCode = MALLOC_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer la variable '%c'");
		sprintf(errMsg, c);
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
		errorCode = SYNTAX_ERROR;
		strcpy(errMsg, "Erreur de syntaxe: Variables doivent etre un seul charactere");
		return 0;
	}
	return 1;
}

int readCommandLine(){
	if (!initNumberStack())
		throwError();
	printf("> ");

	char c;
	Number *n1, *n2, *ntemp;
	while ((c = getchar()) != '\0'){
		if ((c >= '0') && (c <= '9')) {
			n1 = createNumberFromWordCommandLine(c);
			if (n1 == NULL)
				break;
			push(n1);
		}
		else if (c == ' ');
		else if (c == '+') {
			n2 = pop();
			n1 = pop();
			ntemp = addNumbers(n1,n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c == '-') {
			n2 = pop();
			n1 = pop();
			ntemp = substractNumbers(n1,n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c == '*') {
			n2 = pop();
			n1 = pop();
			ntemp = multiplyNumbers(n1,n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
		}
		else if (c >= 'a' && c <= 'z') {
			if(!correctSyntax(getchar()))
				break;
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
			errorCode = SYNTAX_ERROR;
			strcpy(errMsg, "Erreur de syntaxe: Symbole '%c' invalide");
			sprintf(errMsg, c);
			break;
		}
	}
	
	if (errorCode != PROGRAM_CONTINUE)
		return throwError();

	n1 = pop();
	ntemp = pop();
	if (n1 == NULL){
		errorCode = ARGUMENT_ERROR;
		strcpy(errMsg, "Erreur de syntaxe: Pas assez d'arguments");
		return throwError();
	}
	if (ntemp != NULL){
		errorCode = ARGUMENT_ERROR;
		strcpy(errMsg, "Erreur de syntaxe: Trop d'arguments");
		deleteNumberIfNotAVariable(ntemp);
		deleteNumberIfNotAVariable(n1);
		return throwError();
	}

	printNumber(n1);
	deleteNumberIfNotAVariable(n1);
	return PROGRAM_CONTINUE;
}

int main(){
	int code;
	while(1)
	{
		code = readCommandLine();
		if (code == PROGRAM_CONTINUE)
			continue;
		if (code == CRITICAL_ERROR)
			deleteStack();
		if (code == PROGRAM_END)
			break;
	}

	deleteVariableList();
	deleteStack();
	return 0;
}
