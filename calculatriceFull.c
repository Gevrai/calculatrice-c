/*
|----------------------------------------------------------|
|                     IFT 2035 TP #1                       |
| @Author: Gevrai Jodoin Tremblay                          |
| @Author: Benoit Rainville                                |
! Last edited: 10/23/2015                                  |
|----------------------------------------------------------|
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros to make error throwing more obvious
#define PROGRAM_CONTINUE 0
#define CRITICAL_ERROR 1
#define PROGRAM_END 2
#define SYNTAX_ERROR 3
#define MALLOC_ERROR 4
#define VAR_ERROR 5
#define ARGUMENT_ERROR 6
#define STACK_ERROR 7

int errorCode = PROGRAM_CONTINUE;
char errMsg[128] = "";

struct cell {
	int digit;
	struct cell *next_p;
};

typedef struct {
	int sign;
	struct cell *first_p;
} Number;

struct cell* createCell(int digit){
	struct cell *newCell = malloc(sizeof(struct cell));
	if (newCell == NULL){
		errorCode = MALLOC_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer un nombre");
		return NULL;
	}
	newCell->digit = digit;
	newCell->next_p = NULL;
	return newCell;
}

Number* createNumber(){
	Number *result = malloc(sizeof(Number));
	if (result == NULL){
		errorCode = MALLOC_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer un nombre");
		return NULL;
	}
	result->first_p = NULL;
	result->sign = 1;
	return result;
}

void deleteCells(struct cell *currentCell){
	struct cell *tempCell;
	while(currentCell != NULL){
		tempCell = currentCell->next_p;
		free(currentCell);
		currentCell = tempCell;
	}
}

void deleteNumber(Number* number){
	if (number == NULL)
		return;
	deleteCells(number->first_p);
	free(number);
	number = NULL;
}

int charToInt(char c){
	return c - '0';
}

char intToChar(int i){
	return i + '0';
}

void trimZeros(Number *number){
	struct cell *currentCell, *previousCell;
	for(;;){
		previousCell = currentCell = number->first_p;
		while(currentCell->next_p != NULL){
			previousCell = currentCell;
			currentCell = currentCell->next_p;
		}
		// Currentcell is now the most significant digit
		if (currentCell->digit == 0){
			if (previousCell == currentCell)
				break;
			free(currentCell);
			previousCell->next_p = NULL;
		}
		else break;
	}
}

void printNumber(Number *number){
	if ((number->sign) < 0) 
		printf("-");
	//init variables
	struct cell *startCell, *currentCell, *endCell;
	startCell = number -> first_p;
	endCell = number -> first_p;
	currentCell = number -> first_p;
	
	// point to the last cell in the list
	while(endCell -> next_p != NULL)
		endCell = endCell -> next_p;
	
	//Double loop that reads the list in reverse order
	while(endCell != startCell)
	{
		currentCell = startCell;
		while(currentCell->next_p != endCell)
			currentCell = currentCell->next_p;
		printf("%d", endCell->digit);
		endCell = currentCell;
	}
	printf("%d\n", endCell->digit);
}

Number* addNumbers(Number *n1, Number *n2){
	Number *result = createNumber();
	if (result == NULL) return NULL;

	//Necessary variables for calculations
	int carry = 0,
		tempValue,
		n1Sign, n2Sign;
	int c1_done = 0, c2_done = 0;
	struct cell *c1_current, 
				*c2_current,
				*cr_current,
				*ctemp;
	// Dummy variable to continue treatment when a number is bigger than the other
	struct cell czero;
	czero.digit = 0;
	czero.next_p = &czero;

	//Case 1: +/+ or -/-
	if (n1->sign == n2->sign){
		n1Sign = n2Sign = 1;
	} else
	// Case 2: +/- or -/+
	{
		n1Sign = n1->sign;
		n2Sign = n2->sign;
	}

	c1_current = n1->first_p;
	c2_current = n2->first_p;
	while ((!c1_done) || (!c2_done)){
		// Init new cell
		ctemp = createCell(0);
		if (ctemp == NULL){
			deleteCells(cr_current);
			return NULL;
		}
		//DEGUEU: initialise numbers head digit
		if (result->first_p == NULL)
			result->first_p = ctemp;
		else
			cr_current->next_p = ctemp;

		//Addition with carry
		tempValue = carry + n1Sign*(c1_current->digit) + n2Sign*(c2_current->digit);
		if (tempValue > 9){
			carry = 1;
			ctemp->digit = tempValue - 10;
		} else if (tempValue < 0){
			carry = -1;
			ctemp->digit = tempValue + 10;
		} else {
			carry = 0;
			ctemp->digit = tempValue;
		}

		//Set up next iteration
		cr_current = ctemp;
		// Does n1 and n2 have more digit?
		// If so, introduce dummy cell (0) to continue calculations
		if((c1_current->next_p) != NULL) 
			c1_current = c1_current->next_p;
		else {
			c1_current = &czero;
			c1_done = 1;
		}
		if ((c2_current->next_p) != NULL) 
			c2_current = c2_current->next_p;
		else {
			c2_current = &czero;
			c2_done = 1;
		}
	}

	if (carry > 0){
		// Case 1 only : result take either n1 or n2's sign
		result->sign = n1->sign;
		ctemp = createCell(carry);
		if (ctemp == NULL){
			deleteCells(cr_current);
			return NULL;
		}
		cr_current->next_p = ctemp;
	}
	else if (carry < 0){
		result->sign = -1;
		cr_current = result->first_p;
		// Nine's complement
		while (cr_current != NULL){
			cr_current->digit = 9 - (cr_current->digit);
			cr_current = cr_current->next_p;
		}
		// Add one and carry it
		cr_current = result->first_p;
		cr_current->digit += 1; 
		while(cr_current->digit > 9){
			ctemp = cr_current->next_p;
			if (ctemp == NULL){
				ctemp = createCell(0);
				if (ctemp == NULL){
					deleteCells(cr_current);
					return NULL;
				}
				cr_current->next_p = ctemp;
			}
			ctemp->digit += 1; // ctemp <=> cr_current->next_p
			cr_current->digit %= 10;
			cr_current = ctemp;
		}
	} 
	else // (carry == 0)
	{
		result->sign = (n1Sign == n2Sign) ? n1->sign : 1;
	}

	trimZeros(result);
	return result;
}

Number* substractNumbers(Number *n1, Number *n2){
	Number *result;
	if (n1 == n2){
		result = createNumber();
		if (result == NULL)
			return NULL;
		result->first_p = createCell(0);
		if (result->first_p == NULL){
			deleteNumber(result);
			return NULL;
		}
		return result;
	}
	n2->sign = -(n2->sign);
	result = addNumbers(n1,n2);
	n2->sign = -(n2->sign);
	return result;
}

Number* multiplyNumbers(Number *n1, Number *n2){
	
	Number *result = createNumber();

	if (result == NULL) return NULL;
	result->first_p = createCell(0);
	if (result->first_p == NULL){
		deleteNumber(result);
		return NULL;
	}

	// Same sign means result is positive, otherwise it's negative.
	result->sign = (n1->sign == n2->sign) ? 1 : -1;

	struct cell *c1_current, *c2_current, *cr_current, *ctemp;
	int i,j,k;

	c1_current = n1->first_p;
	i = 0;
	while(c1_current != NULL){
		c2_current = n2->first_p;
		j = 0;
		while(c2_current != NULL){
			cr_current = result->first_p;
			// Move to the correct cell while creating them if need be
			for(k = 0; k < i+j; k++){
				ctemp = cr_current->next_p;
				if(ctemp == NULL){
					ctemp = createCell(0);
					if (ctemp == NULL){
						deleteCells(cr_current);
						return NULL;
					}
					cr_current->next_p = ctemp;
				}
				cr_current = ctemp;
			}
			// Cell multiplication (mitosis?)
			cr_current->digit += (c1_current->digit) * (c2_current->digit);
			// Take care of carry
			while(cr_current->digit > 9){
				ctemp = cr_current->next_p;
				if (ctemp == NULL){
					ctemp = createCell(0);
					if (ctemp == NULL){
						deleteCells(cr_current);
						return NULL;
					}
					cr_current->next_p = ctemp;
				}
				ctemp->digit += cr_current->digit / 10;// ctemp <=> cr_current->next_p
				cr_current->digit %= 10;
				cr_current = ctemp;
			}
			c2_current = c2_current->next_p;
			j++;
		}
		c1_current = c1_current->next_p;
		i++;
	}

	trimZeros(result);
	return result;
}

Number* createNumberFromWordCommandLine(char c){
	// Create first cell
	struct cell *currentCell_p = createCell(charToInt(c));
	if (currentCell_p == NULL) return NULL;

	//Continue reading line until invalid character
	while ((c = getchar()) >= '0' && c <= '9'){
		struct cell *newCell_p = createCell(charToInt(c));
		if (newCell_p == NULL){
			deleteCells(currentCell_p);
			return NULL;
		}
		newCell_p->next_p = currentCell_p;
		currentCell_p = newCell_p;
	}
	//
	ungetc(c,stdin);
	// Set up Number and return
	Number *number = createNumber();
	if (number == NULL){
		deleteCells(currentCell_p);
		return NULL;
	}
	number->first_p = currentCell_p;

	trimZeros(number);
	return number;
}

int throwError(){
	/*send an error name and message here to have them printed out
	Also check if the error is trivial (ret 0) or if the memory had to be dumped (ret 1)
	format for error throwing in the code is:
	
		errorCode = ERROR_TYPE;
		strcpy(errMsg, "Error message\n");
		
	The actual throwing is done in readCommandLine() automatically.
	*/

	//while (getchar() != '\n');  //empties command line

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
			return PROGRAM_CONTINUE;
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
		case STACK_ERROR:
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
	if (number == NULL)
		return;
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
	if (currentNode == NULL)
	{
		errorCode = VAR_ERROR;
		strcpy(errMsg, "Erreur de variable: impossible de trouver la variable '%c'");
		sprintf(errMsg, errMsg, c);
	}
	return NULL;
}

struct VariableNode* createVariableNode(Number *n, char c){
	struct VariableNode *newNode = malloc(sizeof(struct VariableNode));
	if (newNode == NULL){
		errorCode = MALLOC_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer la variable '%c'");
		sprintf(errMsg, errMsg, c);
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

struct stackNode {Number* number; struct stackNode *next;};

typedef struct stack_t{
	struct stackNode* topNode;
} NumberStack;

NumberStack *stack = NULL;

int initNumberStack(){
	struct stackNode *currentStackNode, *tempNode;
	Number *tempNumber;
	// Stack already exists?
	if (stack != NULL){
		currentStackNode = stack->topNode;
		while (currentStackNode != NULL){
			deleteNumberIfNotAVariable(currentStackNode->number);
			tempNode = currentStackNode;
			currentStackNode = currentStackNode->next;
			free(tempNode);
		}
		stack->topNode = NULL;
		return 1;
	}
	// Stack doesn't exist
	stack = malloc(sizeof(NumberStack));
	if (stack == NULL){
		errorCode = STACK_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer la pile");
		return 0;
	}
	
	stack->topNode = NULL;
	return 1;
}

void deleteStack(){
	if (stack != NULL){
		initNumberStack();
		free(stack);
		stack = NULL;
	}
}

void push(Number* elementToAdd){
	if (stack == NULL){
		errorCode = STACK_ERROR;
		strcpy(errMsg, "Erreur du stack: Stack inexistant");
		return;
	}
	//Allocate memory for new Node
	struct stackNode *newStackTop = malloc(sizeof(struct stackNode));
	if (newStackTop == NULL){
		errorCode = MALLOC_ERROR;
		strcpy(errMsg, "Erreur d'allocation: Pas assez de memoire pour creer un nouvel element sur la pile");
		return;
	}
	//Construct the new top Node
	newStackTop->number = elementToAdd;
	newStackTop->next = stack->topNode;
	//Update the stack
	stack->topNode = newStackTop;
}

Number* pop(){
	if (stack == NULL){
		errorCode = STACK_ERROR;
		strcpy(errMsg, "Erreur du stack: Stack inexistant");
		return NULL;
	}
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

int correctSyntax(char c){
	if (c != ' ' && c != '\0' && c != '\n'){
		errorCode = SYNTAX_ERROR;
		strcpy(errMsg, "Erreur de syntaxe: Variables doivent etre un et un seul charactere");
		return 0;
	}
	ungetc(c, stdin);
	return 1;
}

int correctArgs(Number *n1, Number *n2)
{
	if ((n1 == NULL) || (n2 == NULL))
	{
		errorCode = ARGUMENT_ERROR;
		strcpy(errMsg, "Erreur de syntaxe: Pas assez d'arguments");
		deleteNumberIfNotAVariable(n1);
		deleteNumberIfNotAVariable(n2);
		return 0;
	}
	return 1;		
}

int readCommandLine(){
	if (!initNumberStack())
		return throwError();
	printf("> ");
	char c;
	Number *n1, *n2, *ntemp;
	
	//Reads the command line until some condition is met
	while ((c = getchar()) != '\0'){
		if ((c >= '0') && (c <= '9')) 
		//If the current character is a number, memorize it
		{
		
			n1 = createNumberFromWordCommandLine(c);
			if (n1 == NULL)
				break;
			push(n1);
		}
		else if (c == ' '); //If it's a space, do nothing
		else if (c == '+') 
		//If we have a "+", we sum the last 2 numbers
		{
			n2 = pop();
			n1 = pop();
		
			if(!correctArgs(n1, n2))
				break;

			ntemp = addNumbers(n1,n2);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
		}
		else if (c == '-') 
		//If we have a "-", substract the last two numbers
		{
			n2 = pop();
			n1 = pop();

			if(!correctArgs(n1, n2))
				break;

			ntemp = substractNumbers(n1,n2);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
		}
		else if (c == '*') 
		//If we have a "*", multiply the last two numbers
		{
			n2 = pop();
			n1 = pop();

			if(!correctArgs(n1, n2))
				break;

			ntemp = multiplyNumbers(n1,n2);
			deleteNumberIfNotAVariable(n1);
			deleteNumberIfNotAVariable(n2);
			if (ntemp == NULL)
				break;
			push(ntemp);
		}
		else if (c >= 'a' && c <= 'z') 
		//If we have a letter, get the number inside a variable
		{
			if(!correctSyntax(getchar()))
				break;
			ntemp = getVariable(c);
			if (ntemp == NULL)
				break;
			push(ntemp);
		}
		else if (c == '=') 
		//If we have an "=", assign the current number to the next variable
		{
			c = getchar();

			if(!(c >= 'a' && c <= 'z'))
			{	
				errorCode = SYNTAX_ERROR;
				strcpy(errMsg, "Erreur de syntaxe: assignation impossible");
				break;
			}

			if(!correctSyntax(getchar()))
				break;
			ntemp = pop();
			assignNumberToVar(ntemp, c);
			push(ntemp);
		}
		else if (c == EOF) //On a EOF character, exit the program
			return PROGRAM_END;
		else if (c == '\n') //On a new line character, break the reading loop
			break;
		else //error throwing
		{
			errorCode = SYNTAX_ERROR;
			strcpy(errMsg, "Erreur de syntaxe: Symbole '%c' invalide");
			sprintf(errMsg, errMsg, c);
			break;
		}
	}
	
	ungetc(c, stdin);
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

int main(int argc, char **argv){
	int code;
	char c;
	while(1)
	{
		code = readCommandLine();
		if (code == CRITICAL_ERROR){
			printf("Aucune memoire disponible: Programme inutilisable sans memoire, voulez-vous quitter? (o/..)\n");
			c = getchar();
			if (c == 'o')
				code = PROGRAM_END;
		}
		while(getchar() != '\n'); //empty command line if it still has characters	
		if (code == PROGRAM_CONTINUE)
			continue;
		else if (code == PROGRAM_END)
			break;
	}

	deleteVariableList();
	deleteStack();
	return 0;
}