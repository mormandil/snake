// Mor Mandil 207134107
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/////////////CONST////////// 

#define MAXROWS 25 						
#define MAXCOLS 75
#define SNAKE '@'
#define FOOD '$'
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key

////////////THE SNAKE///////////
typedef struct Snake      
{
	int x;
	int y;
	char dir;
	struct Snake* next;
} snake;
////////////////////////////////


/* Turn escape codes ON. 
 clear the screen.*/
void init();


/* Moves the cursor to position (x,y) on screen.
   Parameters:
		x: the row of the position.
		y: the column of the position.*/
void gotoxy(int x, int y);


/* This function gets keyCodes for special keys such as arrows , escape and others
It returns the Key code such as 72 for arrow up, 27 for Escape... */
int getKey();
 
/* this function going recursivly over the snake parts and return the last one.
Parameters
		snakePart: the current snake part to check if it is the last.*/
snake * getLastTail(snake * snakePart);

/*this function creates new snake part with the given paramters.
Paramters:
		x: the x corrdinates of the new snakePart.
		y: the y corrdinates of the new snakePart.
		dir: the diraction of the new snakePart.*/
snake * createNewSnakePart(int x,int y,char dir);

/*this function increases the snake by connecting new snake part to the last snake part
Paramters:
		head: the head of the snake.*/
void increaseSnakeSize(snake* head);

/*this function prints all the snake's parts one by each other
Paramters:
		head: the head of the snake.*/
void printSnake(snake* head);

/* this function reorders the snake's parts.
 each part gets the locatoin and diraction of it's previous part.
Paramters:
		head: the head of the snake.*/
void reorderSnakeParts(snake * head);

// this function print the board borders
void printBorders();

/* this function checks recursivly if the head of the snake hits one of it's parts.
 by checking if the locatoin of the head is equivlent to the location of each part in the tail.
Paramters:
		snakeTail: the tail part that being checked.
		x: the x coordinate of the head.
		y: the y coordinate of the head.*/
bool isSnakeHitItself(snake * snakeTail, int x, int y);

/* this function moves the head of the snake according to the user keyboard input.
Paramters:
		the head of the snake.
		dircation: the direction given by the user.*/
void movesSnakeHead(snake * head, char direction);

/* this function checks if the snake was collapsing into the walls or to itself.
Paramters:
		head: the head of the snake.*/
bool isGameOver(snake * head);

/* this function prints the message GAME OVER if the player lose the game.*/
void printGameOver();

/* generates randomly a new location for the food.
Paramters:
	foodX: the X coordinate to store the new location.
	foodY: the Y coordinate to store the new location.
	head: the head of the snake.*/
void createNewFood(int * foodX, int * foodY, snake * head);

// print the food char at the current location
void printFood(int x, int y);

// check if the snake head impact the food location at the current position.
bool isTheFoodEaten(int foodX, int foodY, snake * head);

// this function prints the message YOU WIN if the player won the game.
void printWinner();

// print the menu option to the user and get his choice.
int menu();

// the main functionof the game.
void game();

int main(){

	bool exit = false;
	while (!exit)
	{
		int choice = menu();
		switch (choice)
		{
		case 0:
			exit = true;
			break;
		case 1:
			game();
			break;
		default:
			break;
		} 
		
	}
	
}

void game()
{
	char direction = (char) 0;
    int snakeSize = 0;
    bool initiliazed = false;
    int foodX, foodY;
    int eatenFood = 0;
    bool win = false;
	snake* head;

	init();
	printBorders();
    createNewFood(&foodX,&foodY, NULL);
	printFood(foodX,foodY);		
	
	while (direction != ESC)
	{
		Sleep(1000);
		// get new direction according to the user input.
		if (_kbhit()) {
			direction = getKey();
		}

        if(!initiliazed){
			if (direction == (char) 0) continue;
            gotoxy(MAXROWS / 2, MAXCOLS / 2);
            head = createNewSnakePart(MAXROWS / 2, MAXCOLS / 2, direction);
            movesSnakeHead(head, direction);
            initiliazed = true;
        } 

        if (snakeSize < 10){
            increaseSnakeSize(head);
            snakeSize++;
        }

		movesSnakeHead(head, direction);
		
		// need to check if we eat an apple
		if (isTheFoodEaten(foodX,foodY,head)) {
			increaseSnakeSize(head);
            createNewFood(&foodX,&foodY,head);
            eatenFood++;
            if (eatenFood == 10)
            {
                win = true;
                break;
            }            
		}

		init();
		printBorders();
		printFood(foodX,foodY);		
		printSnake(head);
		gotoxy(MAXROWS + 1, 0);

		if (isGameOver(head)) break;
	
		reorderSnakeParts(head);
	}

	if (win) printWinner();
    else printGameOver();
}

int menu()
{
	int userInput;

	printf("*******************************************************\n* Pick Your Level, to exit press 0\n* 0 -Exit\n* 1 - Basic Level\n* 2 - Intermidiate LEvel\n* 3 - Advanced Level\n*******************************************************\n");
	scanf("%d", &userInput);
	return userInput;
}

bool isTheFoodEaten(int foodX, int foodY, snake * head){
    return (foodY == (head->x) && foodX == (head->y));
}

void createNewFood(int * foodX, int * foodY, snake * head){
    srand(time(NULL));

	int x = MAXCOLS, y = 5;
	bool valid = false;

	while(!valid){
		y = (rand() % (MAXROWS - 1)) + 1;
		x = (rand() % (MAXCOLS - 1)) + 1;	

		if(y == 1 || y == MAXROWS) continue;

		if(x == 1 || x == MAXCOLS) continue;

		if(head != NULL && isSnakeHitItself(head,x,y)) continue;

		valid = true;
	} 

	* foodY = y;
	* foodX = x;
}

void printFood(int x, int y){
	gotoxy(y,x);
	printf("%c",(char)FOOD);
}

void printGameOver() {
	init();
	gotoxy(0, 0);
	printf("##################################\n           GAME OVER\n##################################\n");
}

void printWinner(){
    init();
	gotoxy(0, 0);
	printf("##################################\n             YOU WIN\n##################################\n");
}

bool isGameOver(snake * head) {
	int headX = head->x;
	int headY = head->y;

	if (headX == 0 || headX == MAXROWS) return true;
	else if (headY == 0 || headY == MAXCOLS) return true;
	else return isSnakeHitItself(head->next, head->x, head->y);
}

void movesSnakeHead(snake * head, char direction) {
	switch (direction)
	{
	case UP:
		if (head->dir != DOWN) {
			head->dir = UP;
			head->x -= 1;
		}
		else { direction = head->dir; }
		//x--;
		break;
	case LEFT:
		if (head->dir != RIGHT) {
			head->dir = LEFT;
			head->y -= 1;
		}
		else { direction = head->dir; }
		//y--;
		break;
	case DOWN:
		if (head->dir != UP) {
			head->dir = DOWN;
			head->x += 1;
		}
		else { direction = head->dir; }
		//x++;
		break;
	case RIGHT:
		if (head->dir != LEFT) {
			head->dir = RIGHT;
			head->y += 1;
		}
		else { direction = head->dir; }
		//y++;
		break;
	}
}

bool isSnakeHitItself(snake * snakeTail, int x, int y) {
	if (snakeTail == NULL) return false;
	else if (snakeTail->x == x && snakeTail->y == y) return true;
	else return isSnakeHitItself(snakeTail->next, x, y);
}

void printBorders() {
	int i;

	// print top border
	gotoxy(0, 0);
	for (i = 0; i < MAXCOLS; i++) {
		printf("#");
	}

	// print bottom border
	gotoxy(MAXROWS, 0);
	for (i = 0; i < MAXCOLS; i++) {
		printf("#");
	}

	//print left border
	gotoxy(0, 0);
	for (i = 0; i < MAXROWS; i++) {
		printf("#\n");
	}
	
	//print right border
	for (i = 0; i < MAXROWS; i++) {
		gotoxy(i, MAXCOLS);
		printf("#");
	}
}

void reorderSnakeParts(snake * head) {
	snake* temp, pre, nxt;

	temp = head; // try to delete the temp object
	pre = *temp;

	while (temp->next != NULL)
	{
		nxt.x = temp->next->x;
		nxt.y = temp->next->y;
		nxt.dir = temp->next->dir;
		temp->next->x = pre.x;
		temp->next->y = pre.y;
		temp->next->dir = pre.dir;
		temp = temp->next;
		pre = nxt;
	}
}

void increaseSnakeSize(snake* head) {
	snake * lastSnakePart = getLastTail(head);
	snake * newSnakePart = createNewSnakePart(lastSnakePart->x, lastSnakePart->y, lastSnakePart->dir);
	lastSnakePart->next = newSnakePart;
}

snake * getLastTail(snake * snakePart) {
	if (snakePart->next == NULL) return snakePart;
	else return getLastTail(snakePart->next);
}

snake * createNewSnakePart(int x, int y, char dir)
{
	snake* newSnakePart = (struct Snake*)malloc(sizeof(struct Snake));
	int newX = 0;
	int newY = 0;
	switch (dir) {
		case RIGHT:
			newY = y - 1;
			break;
		case LEFT:
			newY = y + 1;
			break;
		case UP:
			newX = x + 1;
			break;
		case DOWN:
			newX = x - 1;
			break;
	}

	if (newX!=0) newSnakePart->x = newX;
	else newSnakePart->x = x;

	if (newY!=0) newSnakePart->y = newY;
	else newSnakePart->y = y;

	newSnakePart->dir = dir;
	newSnakePart->next = NULL;
	
	return newSnakePart;
}

void printSnake(snake* head)
{
	while (head != NULL) {
		gotoxy(head->x, head->y);
		printf("%c", SNAKE);
		head = head->next;
	}
}

void init() {
	system("cls");
}

void gotoxy(int x, int y)
{
	printf("\x1b[%d;%df", x, y);                           
}

int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}


