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
	if (newCell == NULL) ;//------------------------------------------------------------------------------------ MEMORY ERROR
	newCell->digit = digit;
	newCell->next_p = NULL;
	return newCell;
}

Number* createNumber(){
	Number *result = malloc(sizeof(Number));
	result->first_p = NULL;
	result->sign = 1;
	return result;
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


/* Recursive function...
void printCells(struct cell *cell_p){
	if (cell_p->next_p)
		printCells(cell_p->next_p);
	printf("%d", cell_p->digit);
}

void printNumber(Number *number){
	if ((number->sign) < 0) 
		printf("-");
	printCells(number->first_p);
	printf("\n");
}
*/
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

	//Necessary variables for calculations
	int carry = 0,
		tempValue,
		n1Sign, n2Sign;
	int c1_done = 0, c2_done = 0;
	struct cell *c1_current, 
				*c2_current,
				*cr_current,
				*ctemp;
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
		result->first_p = createCell(0);
		return result;
	}
	n2->sign = -(n2->sign);
	result = addNumbers(n1,n2);
	n2->sign = -(n2->sign);
	return result;
}

Number* multiplyNumbers(Number *n1, Number *n2){

	Number *result = createNumber();
	result->first_p = createCell(0);
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

// TODO check if the reading is really an integer
Number* createNumberFromWordCommandLine(char c){
	// Create first cell
	struct cell *currentCell_p = createCell(charToInt(c));

	//Continue reading line until invalid character
	while ((c = getchar()) >= '0' && c <= '9'){
		struct cell *newCell_p = createCell(charToInt(c));
		newCell_p->next_p = currentCell_p;
		currentCell_p = newCell_p;
	}
	//
	ungetc(c,stdin);
	// Set up Number and return
	Number *number = createNumber();
	number->first_p = currentCell_p;

	trimZeros(number);
	return number;
}

/*		-----UNIT TESTING----			


int main(){
	Number *n1 = createNumberFromWordCommandLine(-1, getchar());
	Number *n2 = createNumberFromWordCommandLine(-1, getchar());
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
*/ 