#include "defenitions.h"

/*Seif 1*/

Array*** PossibleDigits(short sudokuBoard[][9])
{
	Array*** possibleDigitsArray;
	Array* res;
	short i, j;

	/*allocating the possibleDigitsArray*/

	possibleDigitsArray = (Array***)malloc(SIZE * sizeof(Array**));
	checkMemoryAllocation(possibleDigitsArray);

	/*allocating each cell of the possibleDigitsArray*/

	for (i = 0; i < SIZE; i++)
	{
		possibleDigitsArray[i] = (Array**)malloc(9 * sizeof(Array*));
		checkMemoryAllocation(possibleDigitsArray[i]);
	}

	/*two for loops to check and oparte (if needed) the function on each cell*/

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			if (sudokuBoard[i][j] != -1)
				possibleDigitsArray[i][j] = NULL;

			else
			{
				res = (Array*)malloc(sizeof(Array));
				checkMemoryAllocation(res);

				res = possibleDigitsPerCell(sudokuBoard, i, j);
				possibleDigitsArray[i][j] = res;
			}
		}

	return possibleDigitsArray;
}

Array* possibleDigitsPerCell(short sudokuBoard[][SIZE], short i, short j)
/*this function returns a pointer to Array struct for specific cell*/
{
	Array* res;
	res = (Array*)malloc(sizeof(Array));
	checkMemoryAllocation(res);

	short bucket[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	/*this part checks the 3x3 block of the cell*/

	short startingRow, startingCol, k, count = 0, bucketIndex, l = 0;

	for (startingRow = i - (i % 3); startingRow <= (i - (i % 3)) + 2; startingRow++)
		for (startingCol = j - (j % 3); startingCol <= (j - (j % 3)) + 2; startingCol++)
		{
			if (sudokuBoard[startingRow][startingCol] != -1)
			{
				bucket[sudokuBoard[startingRow][startingCol] - 1]--;
				count++;
			}
		}

	/*this part checks the row and the col of the cell*/

	for (k = 0; k < SIZE; k++)
	{
		if (sudokuBoard[i][k] != -1 && bucket[sudokuBoard[i][k] - 1] != 0)
		{
			bucket[sudokuBoard[i][k] - 1]--;
			count++;
		}
	}

	for (k = 0; k < SIZE; k++)
	{
		if (sudokuBoard[k][j] != -1 && bucket[sudokuBoard[k][j] - 1] != 0)
		{
			bucket[sudokuBoard[k][j] - 1]--;
			count++;
		}
	}

	/*updatind the res Array struct*/

	res->size = SIZE - count;
	res->arr = (short*)malloc((res->size) * sizeof(short));
	checkMemoryAllocation(res->arr);

	/*filling the res arr*/

	for (bucketIndex = 0; bucketIndex < SIZE; bucketIndex++)
	{
		if (bucket[bucketIndex] == 1)
		{
			(res->arr)[l] = bucketIndex + 1;
			l++;
		}
	}

	return res;
}

/*Seif 2*/

int OneStage(short board[][9], Array*** possibilities, int* x, int* y)
{
	short i, j, count = 0, minPos = 9, res;
	short anotherMove;

	while (1)
	{

		count = 0;
		anotherMove = 0;
		minPos = 9;

		/*Running on the possibilities matrix and searching for a cell with arr size = 1*/

		for (i = 0; i < SIZE; i++)
			for (j = 0; j < SIZE; j++)
			{
				if (possibilities[i][j] != NULL)
				{
					if (possibilities[i][j]->size == 1) /*if there is only one option for a number to fill*/
					{
						count++;
						board[i][j] = possibilities[i][j]->arr[0];

						free(possibilities[i][j]->arr); /*free the memory of the cell after the update*/
						free(possibilities[i][j]);
						possibilities[i][j] = NULL;

						newPossibilities(possibilities, i, j, board[i][j]); /*updating the possibilities matrix*/
						res = isDuplicate(board); /*checking if there are duplications*/

						if (res == -1)
							return FINISH_FAILURE;
					}

					else
					{
						if (possibilities[i][j]->size < minPos) /*updatind the cell with the minimum possibilities*/
						{
							minPos = possibilities[i][j]->size;
							*x = i;
							*y = j;
						}
						anotherMove = 1;
					}
				}
			}
		if (count == 0)
			break;
	}

	if (anotherMove == 0)
		return FINISH_SUCCESS;

	sudokoPrintBoard(board); /*printing the board*/
	printf("\n");

	return NOT_FINISH;
}

void newPossibilities(Array*** possibilities, short i, short j, short val)
/*this is the general function that updates the possibilities matrix with sub functions*/
{
	newPossibilitiesSquare(possibilities, i, j, val);
	newPossibilitiesRow(possibilities, i, val);
	newPossibilitiesCol(possibilities, j, val);
}

void newPossibilitiesSquare(Array*** possibilities, short i, short j, short val)
/*this function updates the 3x3 square of the cell*/
{
	short startingRow, startingCol;

	for (startingRow = i - (i % 3); startingRow <= (i - (i % 3)) + 2; startingRow++)
		for (startingCol = j - (j % 3); startingCol <= (j - (j % 3)) + 2; startingCol++)
			if (possibilities[startingRow][startingCol] != NULL)
			{
				newPossibilitiesArray(possibilities[startingRow][startingCol], val);
			}

}

void newPossibilitiesRow(Array*** possibilities, short i, short val)
/*this function updates the row of the cell*/
{
	short j;

	for (j = 0; j < SIZE; j++)
		if (possibilities[i][j] != NULL)
			newPossibilitiesArray(possibilities[i][j], val);
}

void newPossibilitiesCol(Array*** possibilities, short j, short val)
/*this function updates the col of the cell*/
{
	short i;

	for (i = 0; i < SIZE; i++)
		if (possibilities[i][j] != NULL)
			newPossibilitiesArray(possibilities[i][j], val);
}

void newPossibilitiesArray(Array* posArr, short val)
/*this function searches for the value that need to be deleted in each array of the cells in possibilities*/
{
	short index, i;
	bool isFound = false;

	if (posArr != NULL) /*if the cell points to NULL, we dont need to do something*/
	{
		for (i = 0; i < posArr->size; i++) /*here we check if the value is in the array, and saves the index of it*/
		{

			if (posArr->arr[i] == val)
			{
				isFound = true;
				index = i;
			}
		}

		if (isFound)/*if we found it, we update the array and the size*/
		{
			if (posArr->size == 1)
			{
				/*free(posArr->arr);
				free(posArr);*/
				posArr = NULL;
			}
			else
			{
				for (i = index; i < (posArr->size) - 1; i++) /*here we are moving the data backwards to the 'empty place'*/
					posArr->arr[i] = posArr->arr[i + 1];

				posArr->arr = (short*)realloc(posArr->arr, (posArr->size - 1) * sizeof(short)); /*reallocating the size of the array*/
				checkMemoryAllocation(posArr->arr);
				(posArr->size)--; /*updating the new size of the array*/
			}
		}
	}
}

short isDuplicate(short sudokuBoard[][SIZE])
/*this function checks if there are any duplications all over the board*/
{
	short i, j, resRow, resSquare, resCol;

	for (i = 0; i < SIZE; i++)
	{
		resRow = checkDupRow(sudokuBoard, i); /*checking for row duplications*/
		resCol = checkDupCol(sudokuBoard, i); /*checking for col duplications*/

		if (i % 3 == 0)
			for (j = 0; j < SIZE; j += 3)
			{
				resSquare = checkDupSquare(sudokuBoard, i, j); /*checking for 3x3 square duplications*/

				if (resSquare == -1)
					break;
			}

		if (resRow == -1 || resCol == -1 || resSquare == -1)
			return -1;
	}

	return 1;
}

short checkDupSquare(short sudokuBoard[][SIZE], short i, short j)
/*this function checks the 3x3 square of the cell and search for duplications*/
{
	short bucket[SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	short startingRow, startingCol, bucketIndex;

	for (startingRow = i; startingRow <= i + 2; startingRow++)
		for (startingCol = j; startingCol <= j + 2; startingCol++)
		{
			if (sudokuBoard[startingRow][startingCol] != -1)
				bucket[sudokuBoard[startingRow][startingCol] - 1]--;
		}

	for (bucketIndex = 0; bucketIndex < SIZE; bucketIndex++)
		if (bucket[bucketIndex] == -1)
			return -1;

	return 1;
}

short checkDupRow(short sudokuBoard[][SIZE], short i)
/*this function checks the row and search for duplications*/
{
	short bucket[SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	short j;

	for (j = 0; j < SIZE; j++)
	{
		if (sudokuBoard[i][j] != -1)
			bucket[(sudokuBoard[i][j]) - 1]--;
	}

	for (j = 0; j < SIZE; j++)
		if (bucket[j] == -1)
			return -1;

	return 1;
}

short checkDupCol(short sudokuBoard[][SIZE], short i)
/*this function checks the col and search for duplications*/
{
	short bucket[SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	short j;

	for (j = 0; j < SIZE; j++)
	{
		if (sudokuBoard[j][i] != -1)
			bucket[(sudokuBoard[j][i]) - 1]--;
	}

	for (j = 0; j < SIZE; j++)
		if (bucket[j] == -1)
			return -1;

	return 1;
}

/*Seif 3*/

int FillBoard(short board[][9], Array*** possibilities)
{
	int gameStatus = NOT_FINISH;
	int x, y, i;
	short numOptions, userVal;

	while (gameStatus == NOT_FINISH)
	{
		gameStatus = OneStage(board, possibilities, &x, &y);

		if (gameStatus == FINISH_FAILURE)
			break;

		else if (gameStatus == FINISH_SUCCESS)
			break;

		printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", x, y);

		if (possibilities[x][y] != NULL)
			numOptions = possibilities[x][y]->size;

		else
			continue;

		for (i = 0; i < numOptions; i++)
			printf("%d. %d \n", i + 1, possibilities[x][y]->arr[i]);

		scanf("%hd", &userVal);

		while (userVal < 1 || userVal > 9)
		{
			printf("Try again, please enter a valid number\n");
			scanf("%hd", &userVal);
		}

		printf("Updating cell %d with value %hd\n", ((x * 9) + y), userVal);
		board[x][y] = userVal;
		free(possibilities[x][y]->arr);
		free(possibilities[x][y]);
		possibilities[x][y] = NULL;
		newPossibilities(possibilities, x, y, userVal);
	}
	return gameStatus;
}

/*general functions*/

void checkMemoryAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Memory allocation failed! Exiting...");
		exit(1);
	}
}

void sudokoPrintBoard(short sudokuBoard[][SIZE])
{
	short value, i, j;

	printTopPart();
	printSeparators();

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			value = sudokuBoard[i][j];

			if (j == 0)
				printf(" %d|", i);

			if (j % 3 == 2 && value != -1)
				printf(" %d|", value);

			else if (j % 3 == 2 && value == -1)
				printf("  |");

			else if (value == -1)
				printf("  ");

			else
				printf(" %d", value);
		}

		printf("\n");

		if (i % 3 == 2)
			printSeparators();
	}
}

void printTopPart()
{
	short i;

	printf("  |");

	for (i = 0; i < SIZE; i++)
	{
		if (i % 3 == 2 && i != 8)
			printf(" %d|", i);

		else
			printf(" %d", i);
	}

	printf("\n");
}

void printSeparators()
{
	short i;

	for (i = 0; i < 23; i++)
		printf("-");

	printf("\n");
}
