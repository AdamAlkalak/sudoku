#include "defenitions.h"

void createActivePlayersList()
{
	int X, i, j, n, k;
	PlayersListNode* playerNode;
	PlayersListNode** pArr;
	Tree tr;

	PlayersList activePlayersLst, winnersLst;
	makeEmptyPlayersList(&activePlayersLst);
	makeEmptyPlayersList(&winnersLst);

	printf("Enter the number of players:\n");
	scanf("%d", &X);

	for (i = 0; i < X; i++) /*this for loop alocate and create the players nodes*/
	{
		Player* currPlayer = (Player*)malloc(sizeof(Player));
		checkMemoryAllocation(currPlayer);

		for (k = 0; k < SIZE; k++)
			for (j = 0; j < SIZE; j++)
				currPlayer->board[k][j] = -1;

		printf("Enter player #%d name:\n", i + 1);
		scanf("%s", &(currPlayer->name));

		startingBoard(currPlayer->board, &(currPlayer->numOfStartingCells));

		sudokoPrintBoard(currPlayer->board);
		printf("\n");

		currPlayer->possibilities = PossibleDigits(currPlayer->board);

		playerNode = createPlayerNode(currPlayer, NULL, NULL);
		insertPlayerNodeToEndList(&activePlayersLst, playerNode);
	}


	pArr = createPointerArrOfPlayersList(activePlayersLst, X);
	mergeSort(pArr, X); /*sorting the array*/

	n = calcSizeOfArray(X); /*the new size of the array*/

	pArr = (PlayersListNode**)realloc(pArr, n * sizeof(PlayersListNode*)); /*reallocating the array to the new size 'n'*/
	checkMemoryAllocation(pArr);

	for (i = X; i < n; i++) /*updating the new cells of the array to NULL*/
		pArr[i] = NULL;

	tr = buildTreeFromArrayRec(pArr, n); /*building the tree from the array*/

	while (!isEmptyPlayerList(&activePlayersLst))
		game(tr, &activePlayersLst, &winnersLst);

	printPlayersList(&winnersLst);


	free(tr.root);
	free(pArr);
	freeWinnersList(&winnersLst);
}

void mergeSort(PlayersListNode** arr, int size)
/*this function is for sorting the nodes arr with the mergeSort algo*/
{
	PlayersListNode** temp;
	int i;

	if (size == 1)
		return;

	else
	{
		mergeSort(arr, size / 2);
		mergeSort(arr + size / 2, size - size / 2);

		temp = (PlayersListNode**)malloc(sizeof(PlayersListNode*) * size);
		checkMemoryAllocation(temp);

		merge(arr, size / 2, arr + size / 2, size - size / 2, temp);

		for (i = 0; i < size; i++)
			arr[i] = temp[i];

		free(temp);
	}
}

void merge(PlayersListNode** arr1, int size1, PlayersListNode** arr2, int size2, PlayersListNode** res)
/*this is the merge function used in mergeSort*/
{
	int i = 0, j = 0, k = 0;

	while (i < size1 && j < size2)
	{

		if (arr1[i]->player->numOfStartingCells < arr2[j]->player->numOfStartingCells)
			res[k++] = arr1[i++];

		else if (arr1[i]->player->numOfStartingCells > arr2[j]->player->numOfStartingCells)
			res[k++] = arr2[j++];

		else /*the same number of starting cells, comparing names*/
		{
			if (strcmp(arr1[i]->player->name, arr2[j]->player->name) < 0)
				res[k++] = arr1[i++];

			else
				res[k++] = arr2[j++];
		}
	}

	while (i < size1)
		res[k++] = arr1[i++];

	while (j < size2)
		res[k++] = arr2[j++];

}

PlayersListNode* createPlayerNode(Player* player, PlayersListNode* prev, PlayersListNode* next)
/*this function creates a player listNode*/
{
	PlayersListNode* res = (PlayersListNode*)malloc(sizeof(PlayersListNode));
	checkMemoryAllocation(res);

	res->player = player;
	res->prev = prev;
	res->next = next;

	return res;
}

void insertPlayerNodeToEndList(PlayersList* lst, PlayersListNode* toAdd)
/*this function is for adding a new node to the players list*/
{
	if (isEmptyPlayerList(lst))
	{
		toAdd->prev = toAdd->next = NULL; /*toAdd is now the only node in the list*/
		lst->head = toAdd;  /*updating the head and the tail of the list*/
		lst->tail = toAdd; /*updating the head and the tail of the list*/
	}

	else
	{
		toAdd->prev = lst->tail;
		toAdd->next = NULL;
		lst->tail->next = toAdd;
		lst->tail = toAdd; /*updating the tail of the list*/
	}
}

void makeEmptyPlayersList(PlayersList* lst)
/*this function creates an empty players list*/
{
	lst->head = lst->tail = NULL;
}

bool isEmptyPlayerList(PlayersList* lst)
/*this function checks if a list is empty or not*/
{
	if (lst->head == NULL && lst->tail == NULL)
		return true;
	else
		return false;
}

void printPlayersList(PlayersList* lst)
/*this function prints players list to the screen according to the project instructions*/
{
	PlayersListNode* currPlayer;

	if (!isEmptyPlayerList(lst))
	{
		printf("\n\t\t\t\t\t\tAPPLAUSE TO THE WINNERS: \n\n");
		currPlayer = lst->head;

		while (currPlayer != NULL)
		{
			printPlayer(currPlayer->player);
			currPlayer = currPlayer->next;
		}
	}

	else
		printf("\n\t\t\tGAME OVER!!! There are no winners this round! GOOD BYE!\n\n");
}

void printPlayer(Player* player)
/*this function prints a player struct to the screen*/
{
	if (player != NULL)
	{
		printf("Player's name: %s\n\n", player->name);
		printf("%s's full board:\n\n", player->name);
		sudokoPrintBoard(player->board);
		printf("\n");
	}
}

int calcSizeOfArray(int playerNum)
/*this function calculates the size with the equation:  2^(log(x+1)) - 1*/
{
	int res;
	double logCalc, logRoundUp;

	logCalc = log2((double)playerNum + 1);
	logRoundUp = ceil(logCalc); /*calculating the log and round it up*/

	res = ((int)pow(2, logRoundUp)) - 1; /*casting and calculating the res size*/

	return res;
}

PlayersListNode** createPointerArrOfPlayersList(PlayersList lst, int listSize)
/*this function allocate and create a pointers array to the list nodes*/
{
	int i;
	PlayersListNode* curr = lst.head;

	PlayersListNode** pArr = (PlayersListNode**)malloc(listSize * sizeof(PlayersListNode*));
	checkMemoryAllocation(pArr);

	for (i = 0; i < listSize; i++)
	{
		pArr[i] = curr;
		curr = curr->next;
	}

	return pArr;
}

Tree buildTreeFromArrayRec(PlayersListNode** pArr, int size)
/*this function builds the tree from the array*/
{
	Tree res, rightTree, leftTree;

	res.root = (TreeNode*)malloc(sizeof(TreeNode));
	checkMemoryAllocation(res.root);

	if (size == 1)
	{
		res.root->data = &pArr[0];
		res.root->left = NULL;
		res.root->right = NULL;
	}

	else
	{
		res.root->data = &pArr[size / 2];

		leftTree = buildTreeFromArrayRec(pArr, size / 2);
		res.root->left = leftTree.root;

		rightTree = buildTreeFromArrayRec(pArr + (size / 2) + 1, size / 2);
		res.root->right = rightTree.root;
	}

	return res;
}

void game(Tree tr, PlayersList* activePlayersList, PlayersList* winnersList)
/*this function calls the recursion*/
{
	gameRec(tr.root, activePlayersList, winnersList);
}

void gameRec(TreeNode* root, PlayersList* activePlayersList, PlayersList* winnersList)
/*this function runs on the players tree and activates oneStage on each player*/
{
	PlayersListNode* currNode;
	int gameStatus;
	int x, y;

	if (root == NULL)
		return;

	gameRec(root->left, activePlayersList, winnersList);

	if (root->data != NULL)
	{
		currNode = *(root->data);

		if (currNode != NULL)
		{
			gameStatus = OneStage(currNode->player->board, currNode->player->possibilities, &x, &y);
			handleGameStatus(gameStatus, root, currNode, activePlayersList, winnersList, x, y);
		}
	}

	gameRec(root->right, activePlayersList, winnersList);
}

void handleGameStatus(int gameStatus, TreeNode* root, PlayersListNode* node, PlayersList* activePlayersList, PlayersList* winnersList, int x, int y)
/*this functions handles each gameStatus case according to the project instructions*/
{
	if (gameStatus == FINISH_FAILURE)
	{
		printf("\n\t\t\t%s, your choice led to duplication! You are out of the game!\n\n", node->player->name);
		sudokoPrintBoard(node->player->board);
		printf("\n");
		removeFromActivePlayersList(activePlayersList, node);
		root->data = NULL;
	}

	else if (gameStatus == FINISH_SUCCESS)
	{
		movePlayerFromActiveToWinner(node, activePlayersList, winnersList);
		root->data = NULL;
	}

	else /*NOT_FINISH*/
	{
		int i, numOptions;
		short userVal;

		printf("%s, it is your turn!\n\n", node->player->name);
		printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", x, y);

		if (node->player->possibilities[x][y] != NULL)
			numOptions = node->player->possibilities[x][y]->size;

		for (i = 0; i < numOptions; i++)
			printf("%d. %d \n", i + 1, node->player->possibilities[x][y]->arr[i]);

		scanf("%hd", &userVal);

		while (userVal < 1 || userVal > 9)
		{
			printf("Try again, please enter a valid number\n");
			scanf("%hd", &userVal);
		}

		printf("Updating cell %d with value %hd\n", ((x * 9) + y), userVal);
		node->player->board[x][y] = userVal;

		if (isDuplicate(node->player->board) == -1)
		{
			printf("\n\t\t\t%s, your choice led to duplication! You are out of the game!\n\n", node->player->name);
			sudokoPrintBoard(node->player->board);
			printf("\n");
			removeFromActivePlayersList(activePlayersList, node);
			root->data = NULL;
		}

		else
		{
			free(node->player->possibilities[x][y]->arr);
			free(node->player->possibilities[x][y]);
			node->player->possibilities[x][y] = NULL;
			newPossibilities(node->player->possibilities, x, y, userVal);
		}
	}
}

void movePlayerFromActiveToWinner(PlayersListNode* player, PlayersList* activePlayersList, PlayersList* winnersList)
/*this function moves a player to the winners list*/
{
	PlayersListNode* newWinner;
	newWinner = createPlayerNode(player->player, NULL, NULL);
	insertPlayerNodeToEndList(winnersList, newWinner);
	removeFromActivePlayersList(activePlayersList, player);
}

void removeFromActivePlayersList(PlayersList* lst, PlayersListNode* toDelete)
/*this function removes from the active players list*/
{
	if (toDelete == lst->head)
	{
		PlayersListNode* temp;

		temp = lst->head->next;
		lst->head = temp;

		freePlayer(toDelete);
		free(toDelete);

		if (!lst->head == NULL)
			lst->head->prev = NULL;
		else
			lst->tail = NULL;
	}

	else if (toDelete == lst->tail)
	{
		PlayersListNode* temp;

		temp = toDelete->prev;
		lst->tail = temp;

		freePlayer(toDelete);
		free(toDelete);

		temp->next = NULL;
	}

	else /*remove from mid*/
	{
		PlayersListNode* temp1, * temp2;

		temp1 = toDelete->prev;
		temp2 = toDelete->next;
		temp1->next = temp2;
		temp2->prev = temp1;

		freePlayer(toDelete);
		free(toDelete);
	}
}

void freePlayer(PlayersListNode* playerNode)
/*this function frees a player*/
{
	int i, j;

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
		{
			if (playerNode->player->possibilities[i][j] != NULL)
			{
				free(playerNode->player->possibilities[i][j]->arr);
				free(playerNode->player->possibilities[i][j]);
			}
		}

	free(playerNode->player->possibilities);
}

void freeWinnersList(PlayersList* lst)
/*this function frees the players list*/
{
	PlayersListNode* curr, * tmp;
	curr = lst->head;

	if (!isEmptyPlayerList(lst))
	{
		while (curr != NULL)
		{
			tmp = curr;
			curr = curr->next;

			free(tmp);
		}
	}
}