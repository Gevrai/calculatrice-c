struct cell {
	int figure;
	struct cell *next_p
};

typedef struct {
	int isPositive;
	struct cell *first_p;
} Number;

int charToInt(char c){
	return c - '0';
}

void printNumber(Number* number){
	if (!(number->isPositive)) printf("-");
	struct cell *currentCell_p = number->first_p;

	while(currentCell_p != NULL){
		printf("%d", currentCell_p->figure);
		currentCell_p = currentCell_p->next_p;
	}
	printf("\n");
}

// TODO check if the reading is really an integer
Number* createNumberFromWordCommandLine(int isPositive, char c){
	Number *number = malloc(sizeof(Number));
	if (number == NULL);	// --------------------------------------------------------------------------------------------MEMORY ERROR

	//Set up number and allocate first cell
	number->isPositive = isPositive;
	struct cell *currentCell_p = number->first_p = malloc(sizeof(struct cell));
	if (currentCell_p == NULL);// ERROR -----------------------------------------------------------------------------------MEMORY ERROR

	currentCell_p->figure = charToInt(c);
	currentCell_p->next_p = NULL;

	//Continue reading line until invalid character
	while ((c = getchar()) >= '0' && c <= '9'){
		struct cell *newCell_p = malloc(sizeof(struct cell));
		newCell_p->figure = charToInt(c);
		newCell_p->next_p = NULL;
		currentCell_p->next_p = newCell_p;
		currentCell_p = newCell_p;
	}

	return number;
}

/* UNIT TESTING

int main(){
	Number *number = createNumberFromWordCommandLine(1, getchar());
	printNumber(number);
	return 0;
}
*/