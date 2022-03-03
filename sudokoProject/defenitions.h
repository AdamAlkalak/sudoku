#define _CRT_SECURE_NO_WARNINGS	
#define SIZE 9
#define NOT_FINISH 0
#define FINISH_SUCCESS 1
#define FINISH_FAILURE -1
#define MAX_NAME_LEN 100

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct _Array
{
	short* arr;
	unsigned short size;
} Array;

typedef Array CellPossibilities;

typedef struct _CellLocation
{
	unsigned short row;
	unsigned short col;
} CellLocation;

typedef struct _CellListNode
{
	CellLocation location;
	struct CellListNode* next;
	struct CellListNode* prev;
} CellListNode;

typedef struct _CellList
{
	CellListNode* head;
	CellListNode* tail;
} CellList;

typedef struct _Player
{
	char name[MAX_NAME_LEN];
	short board[SIZE][SIZE];
	CellPossibilities*** possibilities;
	int numOfStartingCells;
} Player;

typedef struct _PlayersListNode
{
	Player* player;
	struct PlayersListNode* next;
	struct PlayersListNode* prev;
} PlayersListNode;

typedef struct _PlayersList
{
	PlayersListNode* head;
	PlayersListNode* tail;
} PlayersList;

typedef struct _TreeNode
{
	PlayersListNode** data;
	struct _TreeNode* left;
	struct _TreeNode* right;
} TreeNode;

typedef struct _Tree
{
	TreeNode* root;
} Tree;

/*Seif 1*/
Array*** PossibleDigits(short sudokuBoard[][9]);
Array* possibleDigitsPerCell(short sudokuBoard[][SIZE], short i, short j);

/*Seif 2*/
int OneStage(short board[][9], Array*** possibilities, int* x, int* y);
void newPossibilities(Array*** possibilities, short i, short j, short val);
void newPossibilitiesSquare(Array*** possibilities, short i, short j, short val);
void newPossibilitiesRow(Array*** possibilities, short i, short val);
void newPossibilitiesCol(Array*** possibilities, short j, short val);
void newPossibilitiesArray(Array* posArr, short val);
short isDuplicate(short sudokuBoard[][SIZE]);
short checkDupSquare(short sudokuBoard[][SIZE], short i, short j);
short checkDupRow(short sudokuBoard[][SIZE], short i);
short checkDupCol(short sudokuBoard[][SIZE], short i);


/*Seif 3*/
int FillBoard(short board[][9], Array*** possibilities);

/*general*/
void checkMemoryAllocation(void* ptr);
void sudokoPrintBoard(short sudokuBoard[][SIZE]);
void printTopPart();
void printSeparators();
void freeWinnersList(PlayersList* lst);

/*randomized board*/
void startingBoard(short sudokuBoard[][SIZE], int* numOfStartingCells);
void createLocationList(CellList* lst);
short randNumFromPos(CellPossibilities*** possibilities, CellLocation location);

/*list functions*/
void makeEmptyList(CellList* lst);
int isEmptyList(CellList lst);
void freeList(CellList lst);
void insertNodeToEndList(CellList* lst, CellListNode* newTail);
CellListNode* createNewCellListNode(CellLocation location, CellListNode* next, CellListNode* prev);
void insertLocationToEndList(CellList* lst, CellLocation location);
CellLocation deleteCellFromList(CellList* lst, int K, int listSize);

/*players list functions*/
void createActivePlayersList();
void makeEmptyPlayersList(PlayersList* lst);
bool isEmptyPlayerList(PlayersList* lst);
void insertPlayerNodeToEndList(PlayersList* lst, PlayersListNode* toAdd);
PlayersListNode* createPlayerNode(Player* player, PlayersListNode* prev, PlayersListNode* next);
void printPlayersList(PlayersList* lst);
void printPlayer(Player* player);

/*array and merge sort functions*/
int calcSizeOfArray(int playerNum);
PlayersListNode** createPointerArrOfPlayersList(PlayersList lst, int listSize);
void mergeSort(PlayersListNode** arr, int size);
void merge(PlayersListNode** arr1, int size1, PlayersListNode** arr2, int size2, PlayersListNode** res);

/*tree functions*/
Tree buildTreeFromArrayRec(PlayersListNode** pArr, int size);

/*game functions*/
void game(Tree tr, PlayersList* activePlayersList, PlayersList* winnersList);
void gameRec(TreeNode* root, PlayersList* activePlayersList, PlayersList* winnersList);
void handleGameStatus(int gameStatus, TreeNode* root, PlayersListNode* node, PlayersList* activePlayersList, PlayersList* winnersList, int x, int y);
void movePlayerFromActiveToWinner(PlayersListNode* node, PlayersList* activePlayersList, PlayersList* winnersList);
void removeFromActivePlayersList(PlayersList* lst, PlayersListNode* toDelete);
void freePlayer(PlayersListNode* playerNode);