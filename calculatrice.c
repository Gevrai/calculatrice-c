#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell {int figure; struct cell *next; };

typedef struct num {int isPositive; struct cell *number; };

struct stackNode {char content; struct stackNode *next;};

char* pop(struct stackNode *stackPointer){
	if (stackPointer != NULL){
		char* content = (*stackPointer).content;
		struct stackNode* temp = stackPointer;
		stackPointer = (*stackPointer).next;
		free(temp);
		return content;
	} else {
		return NULL;
	}
}

void push(struct stackNode* stackPointer, char content){
	struct stackNode *newStackTop = malloc(sizeof(struct stackNode));
	(*newStackTop).content = content;
	(*newStackTop).next = stackPointer;
	stackPointer = newStackTop;
	printf("New element %s added to stack\n", content);
}

int charToInt(char c){
	return c - '0';
}

/*
stackNode readCommandLineActive(stackNode *stack){
	printf("> ");
	char c;
	cell prevCell;
	int guard = 0; //variable sentinelle pour le fin d<un traitement
	while ((c = getchar()) != '\0'){
		if ((c >= '0') && (c <= '9')) {
			if(guard == 0){
				//create a new number
				guard = 1;
				num number = malloc(sizeof(num));
				cell numeral = malloc(sizeof(numeral));

				numeral.chiffre = charToInt(c);
				number.positif = 1;
				number.chiffres = &numeral;
			}
			else{
				//add a character to the current number
				cell next = malloc(sizeof(next));
				numeral.chiffre = charToInt(c);
				numeral.suivant = &next;
			}

		}
		else if (c == ' '){
			guard = 0;
		}; //traitement ' '
		else if (c == '+') ;//traitement '+'
		else if (c == '-') ;//traitement '-'
		else if (c == '*') ;//traitement '*'
		else if (c >= 'a' && c >= 'z') ;//traitement variables
		else if (c == '=') ;//traitement assignation
		else if (c == EOF) return;
		else break;
	}
	printf("\n");
}
*/

int main(){
	struct stackNode *stackPointer = NULL;
	char c;
	//stackPointer = readCommandLineActive(stack);
	while ((c = getchar()) != EOF){
		if (c == 'p'){
			char *elementPointer = pop(stackPointer);
			if (elementPointer != NULL){
				printf("%s\n", *elementPointer);
			} else {
				printf("No more elements in stack\n");
			}
		} else {
			push(stackPointer, c);
		}
	}
	return 0;
}