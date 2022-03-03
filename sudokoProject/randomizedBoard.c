#include "defenitions.h"

void startingBoard(short sudokuBoard[][SIZE], int* numOfStartingCells)
/*this function generate a random starting board according to the project instructions*/
{
	int i, K, listSize = 81;
	short row, col, randNum;
	CellPossibilities*** possibilities = PossibleDigits(sudokuBoard);
	CellLocation location;

	srand(time(NULL));

	int N = 1 + rand() % 20;           
	*numOfStartingCells = N;

	CellList lst;

	makeEmptyList(&lst);
	createLocationList(&lst);

	for (i = 0; i < N; i++) 
	{
		K = 1 + rand() % listSize;
		location = deleteCellFromList(&lst, K, listSize);
		listSize--;
		row = location.row;
		col = location.col;
		randNum = randNumFromPos(possibilities, location);
		sudokuBoard[row][col] = randNum;
		newPossibilities(possibilities, row, col, randNum);
	}
}

short randNumFromPos(CellPossibilities*** possibilities, CellLocation location)
/*this function returns the number from the random index in the poss array*/
{
	short size;
	int index;

	size = possibilities[location.row][location.col]->size;
	index = rand() % size;

	return possibilities[location.row][location.col]->arr[index];
}

/*list functions*/

void makeEmptyList(CellList* lst)
/*this function creates empty Cell list*/
{
	lst->head = lst->tail = NULL;
}

int isEmptyList(CellList lst)
/*this function checks if the cell list is empty or not*/
{
	return (lst.head == NULL);
}

void freeList(CellList lst)
{
	CellListNode* curr, * next;
	curr = lst.head;

	while (curr != NULL)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
}

void insertNodeToEndList(CellList* lst, CellListNode* newTail)
/*this function inserts cell list node to the list tail*/
{
	if (isEmptyList(*lst))
	{
		newTail->next = newTail->prev = NULL;
		lst->head = lst->tail = newTail;
	}
	else
	{
		newTail->prev = lst->tail;
		newTail->next = NULL;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

CellListNode* createNewCellListNode(CellLocation location, CellListNode* next, CellListNode* prev)
/*this function creates new cell node*/
{
	CellListNode* res = (CellListNode*)malloc(sizeof(CellListNode));
	checkMemoryAllocation(res);

	res->location = location;
	res->next = next;
	res->prev = prev;

	return res;
}

void insertLocationToEndList(CellList* lst, CellLocation location)
/*this function inserts location as a node to the end of the list*/
{
	CellListNode* res = createNewCellListNode(location, NULL, NULL);
	insertNodeToEndList(lst, res);
}

void createLocationList(CellList* lst)
/*this function creates the initial list with all the cells in it*/
{
	short i, j;
	CellLocation currLocation;

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			currLocation.row = i;
			currLocation.col = j;

			insertLocationToEndList(lst, currLocation);
		}
}

CellLocation deleteCellFromList(CellList* lst, int K, int listSize)
/*this function is for deleting the cell in index k from the list*/
{
	CellListNode* curr, * temp1, * temp2;
	int i;
	CellLocation location;

	if (K == 1) /*delete from head*/
	{
		temp1 = lst->head;
		lst->head = lst->head->next;

		location.row = temp1->location.row;
		location.col = temp1->location.col;

		free(temp1);
		lst->head->prev = NULL;
	}

	else if (K == listSize) /*delete from tail*/
	{
		temp1 = lst->tail;
		lst->tail = lst->tail->prev;

		location.row = temp1->location.row;
		location.col = temp1->location.col;

		free(temp1);
		lst->tail->next = NULL;
	}

	else /*delete from mid*/
	{
		curr = lst->head;

		for (i = 1; i < K; i++)
			curr = curr->next;

		temp1 = curr->prev;
		temp1->next = curr->next;

		temp2 = curr->next;
		temp2->prev = curr->prev;

		location.row = curr->location.row;
		location.col = curr->location.col;

		free(curr);
	}

	return location;
}
