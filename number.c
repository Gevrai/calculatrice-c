#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cell {
	int figure;
	struct cell *next_p;
};

typedef struct {
	int isPositive;
	struct cell *first_p;
} Number;

struct cell* createCell(int figure){
	struct cell *newCell = malloc(sizeof(struct cell));
	newCell->figure = figure;
	newCell->next_p = NULL;
	return newCell;
}

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
	// Init the new number (result of the sum)
	Number *result = malloc(sizeof(Number));
	result->first_p = NULL;

	//Necessary variables for calculations
	int carry = 0,
		tempValue,
		n2_isPositive;
	int c1_done = 0, c2_done = 0;
	struct cell *c1_current, 
				*c2_current,
				*cresult_current,
				*ctemp;
	struct cell czero;
	czero.figure = 0;
	czero.next_p = &czero;

	//Case 1: +/+ or -/-
	if (n1-> isPositive == n2->isPositive){
		// Since (-n1+-n2) = -(n1+n2), the result takes either n1 or n2's sign
		result->isPositive = n1->isPositive;

		c1_current = n1->first_p;
		c2_current = n2->first_p;
		while ((!c1_done) || (!c2_done)){
			// Init new cell
			ctemp = createCell(0);

			//DEGUEU: initialise numbers head figure
			if (result->first_p == NULL)
				result->first_p = ctemp;
			else
				cresult_current->next_p = ctemp;

			//Addition with carry
			tempValue = carry + (c1_current->figure) + (c2_current->figure);
			if (tempValue > 9){
				carry = 1;
				ctemp->figure = tempValue - 10;
			} else if (tempValue < 0){
				carry = -1;
				ctemp->figure = tempValue + 10;
			} else {
				carry = 0;
				ctemp->figure = tempValue;
			}

			//Set up next iteration
			cresult_current = ctemp;
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

		if (carry){
			ctemp = createCell(carry);
			cresult_current->next_p = ctemp;
		}
	}

	return result;
}

Number* multiplyNumbers(Number *n1, Number *n2){

	Number *result = malloc(sizeof(Number));
	result->first_p = createCell(0);
	// Same sign means result is positive, otherwise it's negative.
	result->isPositive = (n1->isPositive == n2->isPositive) ? 1 : 0;

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
					cr_current->next_p = ctemp;
				}
				cr_current = ctemp;
			}
			// Cell multiplication (mitosis?)
			cr_current->figure += (c1_current->figure) * (c2_current->figure);
			// Take care of carry
			while(cr_current->figure > 9){
				ctemp = cr_current->next_p;
				if (ctemp == NULL){
					ctemp = createCell(0);
					cr_current->next_p = ctemp;
				}
				ctemp->figure += cr_current->figure / 10;// ctemp <=> cr_current->next_p
				cr_current->figure %= 10;
				cr_current = ctemp;
			}
			c2_current = c2_current->next_p;
			j++;
		}
		c1_current = c1_current->next_p;
		i++;
	}
	return result;
}

// TODO check if the reading is really an integer
Number* createNumberFromWordCommandLine(int isPositive, char c){
	// Create first cell
	struct cell *currentCell_p = createCell(charToInt(c));

	//Continue reading line until invalid character
	while ((c = getchar()) >= '0' && c <= '9'){
		struct cell *newCell_p = malloc(sizeof(struct cell));
		newCell_p->figure = charToInt(c);
		newCell_p->next_p = currentCell_p;
		currentCell_p = newCell_p;
	}

	// Set up Number and return
	Number *number = malloc(sizeof(Number));
	if (number == NULL);	// --------------------------------------------------------------------------------------------MEMORY ERROR
	number->isPositive = isPositive;
	number->first_p = currentCell_p;
	return number;
}


// Don't forget to comment this once UNIT TESTING is done.
int main(){
	Number *n1 = createNumberFromWordCommandLine(1, getchar());
	Number *n2 = createNumberFromWordCommandLine(1, getchar());
	Number *n3 = multiplyNumbers(n1,n2);

	printNumber(n1);
	printf("* ");
	printNumber(n2);
	printf("= ");
	printNumber(n3);

	deleteNumber(n1);
	deleteNumber(n2);
	deleteNumber(n3);
	return 0;
}
