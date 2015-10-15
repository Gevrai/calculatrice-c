#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell {
	int figure;
	struct cell *next_p;
};

typedef struct {
	int isPositive;
	int length;
	struct cell *first_p;
} Number;

void deleteNumber(Number* number){
	struct cell *tempCell,
				*currentCell = number->first_p;
	while(currentCell != NULL){
		tempCell = currentCell->next_p;
		free(currentCell);
		currentCell = tempCell;
	}
	free(number);
	number = NULL;
}

int charToInt(char c){
	return c - '0';
}

char intToChar(int i){
	return i +'0';
}

// Recursive function...
void printCells(struct cell *cell_p){
	if (cell_p->next_p) printCells(cell_p->next_p);
	printf("%d", cell_p->figure);
}

void printNumber(Number* number){
	if (!(number->isPositive)) printf("-");
	printCells(number->first_p);
	printf("\n");
}

Number* addNumbers(Number *n1, Number *n2){
	// This function needs that the first number has more or the same ammount of numeral than the second
	// If this is not the case we need to swap them
	struct cell *c1_current, *c2_current;
	c1_current = n1->first_p;
	c2_current = n2->first_p;
	while(c2_current != NULL){
		if (c1_current == NULL) {
			return addNumbers(n2,n1);
		}
		c1_current = c1_current->next_p;
		c2_current = c2_current->next_p;
 	}

	// Init the new number (result of the sum)
	Number *result = malloc(sizeof(Number));
	result->first_p = NULL;
	result->length = 0;

	//Necessary variables for calculations (as well as c1/c2_current already created)
	struct cell *cresult_current, *ctemp;
	struct cell czero;
	czero.figure = 0;
	czero.next_p = &czero;

	int carry = 0, tempValue, lastc2Value;
	//Case 1: +/+
	if (n1-> isPositive && n2->isPositive){
		result->isPositive = 1;
		c1_current = n1->first_p;
		c2_current = n2->first_p;
		while (c1_current != NULL){
			// Init new cell
			ctemp = malloc(sizeof(struct cell));
			ctemp->next_p = NULL;
			//DEGUEU: initialise numbers head figure
			if (result->first_p == NULL)
				result->first_p = ctemp;
			else
				cresult_current->next_p = ctemp;
			//Addition with carry
			tempValue = carry + (c1_current->figure) + (c2_current->figure);
			if (tempValue > 9){
				carry = 1;
				ctemp->figure = tempValue%10;
			} else {
				carry = 0;
				ctemp->figure = tempValue;
			}

			//Set up next iteration
			cresult_current = ctemp;
			c1_current = c1_current->next_p;
			// Is n2 done?
			if ((c2_current->next_p) != NULL){
				c2_current = c2_current->next_p;
			} 
			// Introduce dummy cell to continue calculations
			else {
				c2_current = &czero;
			}

			(result->length)++;
		}
		if (carry){
			ctemp = malloc(sizeof(struct cell));
			cresult_current->next_p = ctemp;
			ctemp->next_p = NULL;
			ctemp->figure = carry;
			(result->length)++;
		}
	}

	return result;
}

// TODO check if the reading is really an integer
Number* createNumberFromWordCommandLine(int isPositive, char c){
	// Create first cell
	struct cell *currentCell_p = malloc(sizeof(struct cell));
	if (currentCell_p == NULL);// ERROR -----------------------------------------------------------------------------------MEMORY ERROR
	currentCell_p->figure = charToInt(c);
	currentCell_p->next_p = NULL;

	int length = 1;
	//Continue reading line until invalid character
	while ((c = getchar()) >= '0' && c <= '9'){
		struct cell *newCell_p = malloc(sizeof(struct cell));
		newCell_p->figure = charToInt(c);
		newCell_p->next_p = currentCell_p;
		currentCell_p = newCell_p;
		length++;
	}

	// Set up Number and return
	Number *number = malloc(sizeof(Number));
	if (number == NULL);	// --------------------------------------------------------------------------------------------MEMORY ERROR
	number->isPositive = isPositive;
	number->length = length;
	number->first_p = currentCell_p;
	return number;
}

int main(){
	Number *n1 = createNumberFromWordCommandLine(1, getchar());
	Number *n2 = createNumberFromWordCommandLine(1, getchar());
	Number *n3 = addNumbers(n1,n2);
	printNumber(n1);
	printf("+ ");
	printNumber(n2);
	printf("= ");
	printNumber(n3);
	deleteNumber(n1);
	deleteNumber(n2);
	deleteNumber(n3);
	return 0;
}
