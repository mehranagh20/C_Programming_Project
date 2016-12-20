#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#define HORSIZE 50
#define VERSIZE 15
#define BUFFER_SIZE 200
typedef struct nodes
{
	short int iOfIndex;
	short int jOfindex;
	nodes * nextNode;
}Node;
short int numOfNodesToPass = 0;
short int wayOfPassing = 0;
short int point = 0;
short int lastMove = 0;
short int arr[HORSIZE][VERSIZE] = { 0 };
void printArray(short int ifLost)
{
	CONSOLE_SCREEN_BUFFER_INFO scInfo;
	WORD currColor;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scInfo);
	currColor = scInfo.wAttributes;
	if (ifLost == 0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("you lost. YOUR POINT IS: %d	\t\t\t\n", point);
	}
	else
	{
		printf("YOUR POINT IS: %d. \n", point);
	}
	static char ifChangeColor = 0;
	for (int i = 0; i < VERSIZE; i++)//for vertical \n s
	{
		for (int j = 0; j < HORSIZE; j++)
		{
			if (arr[j][i] == 0)//for spaces
			{
				printf(" ");
			}
			else if (arr[j][i] == 1)//for walls
			{
				printf("%c", 178);
			}
			else if (arr[j][i] == 2)//for snake
			{
				printf("%c", 206);//148
			}
			else if (arr[j][i] == 3)
			{
				if (ifChangeColor == 1)//change the color
				{
					CONSOLE_SCREEN_BUFFER_INFO scInfo;
					WORD currColor;
					GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scInfo);
					currColor = scInfo.wAttributes;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
					printf("%c", 147);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), currColor);
					ifChangeColor = 0;
				}
				else
				{
					printf("%c", 147);
					ifChangeColor = 1;
				}
			}
		}
		puts("");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), currColor);
}

void addToSnake(Node *headOfSnake, int movement)// we add new snake body to the end of snake(nodes)
{
	point++;
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->nextNode = NULL;

	if (headOfSnake->nextNode == NULL && (movement == 'd' || movement == 'D'))
	{
		if (arr[headOfSnake->jOfindex - 1][headOfSnake->iOfIndex] == 1)
		{
			newNode->jOfindex = headOfSnake->jOfindex;
			newNode->iOfIndex = headOfSnake->iOfIndex + 1;
			arr[newNode->jOfindex][newNode->iOfIndex] = 2;
			headOfSnake->nextNode = newNode;
			return;
		}
		newNode->iOfIndex = headOfSnake->iOfIndex;
		newNode->jOfindex = headOfSnake->jOfindex - 1;
		arr[headOfSnake->jOfindex - 1][headOfSnake->iOfIndex] = 2;
		headOfSnake->nextNode = newNode;
		return;
	}
	else if (headOfSnake->nextNode == NULL && (movement == 'a' || movement == 'A'))
	{
		if (arr[headOfSnake->jOfindex + 1][headOfSnake->iOfIndex] == 1)
		{
			newNode->jOfindex = headOfSnake->jOfindex;
			newNode->iOfIndex = headOfSnake->iOfIndex + 1;
			arr[newNode->jOfindex][newNode->iOfIndex] = 2;
			headOfSnake->nextNode = newNode;
			return;
		}
		newNode->iOfIndex = headOfSnake->iOfIndex;
		newNode->jOfindex = headOfSnake->jOfindex + 1;
		arr[headOfSnake->jOfindex + 1][headOfSnake->iOfIndex] = 2;
		headOfSnake->nextNode = newNode;
		return;
	}
	else if (headOfSnake->nextNode == NULL && (movement == 'W' || movement == 'w'))
	{
		if (arr[headOfSnake->jOfindex][headOfSnake->iOfIndex + 1] == 1)
		{
			newNode->jOfindex = headOfSnake->jOfindex + 1;
			newNode->iOfIndex = headOfSnake->iOfIndex;
			arr[newNode->jOfindex][newNode->iOfIndex] = 2;
			headOfSnake->nextNode = newNode;
			return;
		}
		newNode->jOfindex = headOfSnake->jOfindex;
		newNode->iOfIndex = headOfSnake->iOfIndex + 1;
		arr[headOfSnake->jOfindex][headOfSnake->iOfIndex + 1] = 2;
		headOfSnake->nextNode = newNode;
		return;
	}
	else if (headOfSnake->nextNode == NULL && (movement == 'S' || movement == 's'))
	{
		if (arr[headOfSnake->jOfindex][headOfSnake->iOfIndex - 1] == 1)
		{
			newNode->jOfindex = headOfSnake->jOfindex + 1;
			newNode->iOfIndex = headOfSnake->iOfIndex;
			arr[newNode->jOfindex][newNode->iOfIndex] = 2;
			headOfSnake->nextNode = newNode;
			return;
		}
		newNode->jOfindex = headOfSnake->jOfindex;
		newNode->iOfIndex = headOfSnake->iOfIndex - 1;
		arr[headOfSnake->jOfindex][headOfSnake->iOfIndex - 1] = 2;
		headOfSnake->nextNode = newNode;
		return;
	}
	while (headOfSnake->nextNode->nextNode != NULL)
	{
		headOfSnake = headOfSnake->nextNode;
	}
	if (headOfSnake->iOfIndex == headOfSnake->nextNode->iOfIndex)//we add to left or right.
	{
		if (headOfSnake->jOfindex > headOfSnake->nextNode->jOfindex)
		{
			newNode->jOfindex = headOfSnake->nextNode->jOfindex - 1;
			arr[headOfSnake->nextNode->jOfindex - 1][headOfSnake->nextNode->iOfIndex] = 2;
		}
		else
		{
			newNode->jOfindex = headOfSnake->nextNode->jOfindex + 1;
			arr[headOfSnake->nextNode->jOfindex + 1][headOfSnake->nextNode->iOfIndex] = 2;
		}
		newNode->iOfIndex = headOfSnake->nextNode->iOfIndex;
	}
	else if (headOfSnake->jOfindex == headOfSnake->nextNode->jOfindex)//we add to up or down
	{
		if (headOfSnake->iOfIndex > headOfSnake->nextNode->iOfIndex)
		{
			newNode->iOfIndex = headOfSnake->nextNode->iOfIndex - 1;
			arr[headOfSnake->nextNode->jOfindex][headOfSnake->nextNode->iOfIndex - 1] = 2;
		}
		else if (headOfSnake->iOfIndex < headOfSnake->nextNode->iOfIndex)
		{
			newNode->iOfIndex = headOfSnake->nextNode->iOfIndex + 1;
			arr[headOfSnake->nextNode->jOfindex][headOfSnake->nextNode->iOfIndex + 1] = 2;
		}
		newNode->jOfindex = headOfSnake->nextNode->jOfindex;
	}
	headOfSnake->nextNode->nextNode = newNode;

}

int moveSnake(int movement, Node ** head)//0 for start, 1 for goint right, 2 for going left , 3 for up, 4 for down
{
	//for the known problem a possible solution is: 'think of other solutions .'
	//determie the number of nodes that is going to pass the sides.or any other good possible solutions.
	//determine the last move ...the real last move.
	//until the all the nodes are not passed we do the efficient arithmitics...for example if the head was going to up we add to i...
	Node * tempNode = *head;
	int tempIOfIndex = (*head)->iOfIndex;
	int tempJOfIndex = (*head)->jOfindex;
	if ((movement == 'd' || movement == 'D') && (lastMove == 'a' || lastMove == 'A'))
	{
		movement = lastMove;
	}
	if ((movement == 'w' || movement == 'W') && (lastMove == 's' || lastMove == 'S'))
	{
		movement = lastMove;
	}
	if ((movement == 's' || movement == 'S') && (lastMove == 'w' || lastMove == 'W'))
	{
		movement = lastMove;
	}
	if ((movement == 'A' || movement == 'a') && (lastMove == 'd' || lastMove == 'D'))
	{
		movement = lastMove;
	}
	if (movement == 100 || movement == 68)//going right
	{
		if (tempNode->jOfindex == 49)
		{
			if (arr[0][tempNode->iOfIndex] == 1)
			{
				return 0;
			}
		}
		else
		{
			if (arr[tempNode->jOfindex + 1][tempNode->iOfIndex] == 1)
			{
				return 0;
			}
		}
		if (arr[tempNode->jOfindex + 1][tempNode->iOfIndex] == 3)
		{
			arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 0;
			addToSnake(tempNode, movement);
			short int randJ = rand() % 47 + 1;
			short int randI = rand() % 13 + 1;
			while (arr[randJ][randI] != 0)
			{
				randJ = rand() % 47 + 1;
				randI = rand() % 13 + 1;
			}
			arr[randJ][randI] = 3;

		}
		else if (arr[tempNode->jOfindex + 1][tempNode->iOfIndex] == 1)
		{
			return 0;// 0 means we lost the game...
		}
		arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
		if (tempNode->jOfindex == 49)
		{
			nodes *nodeForCheckin = (*head)->nextNode; wayOfPassing = 1;
			while (nodeForCheckin != NULL)
			{
				nodeForCheckin = nodeForCheckin->nextNode;
				numOfNodesToPass++;
			}
			arr[0][tempNode->iOfIndex] = 2;
			(*head)->jOfindex = 0;
		}
		else
		{
			arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 2;
			(*head)->jOfindex += 1;
		}
		while (tempNode->nextNode != NULL)
		{
			tempNode = tempNode->nextNode;
			if (tempNode->jOfindex == 49&&wayOfPassing==1)//this is for coming to other side when there is no walls.
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[0][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 0;
				tempJOfIndex = 49;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (wayOfPassing == 3 && numOfNodesToPass > 0 && tempNode->iOfIndex == 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][14] = 2;
				tempNode->iOfIndex = 14;
				tempIOfIndex = 0;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (wayOfPassing == 4 && numOfNodesToPass > 0 && tempNode->iOfIndex == 14)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][0] = 2;
				tempNode->iOfIndex = 0;
				tempIOfIndex = 14;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (tempNode->jOfindex == 0 && wayOfPassing == 2 && numOfNodesToPass > 0)//this is for coming to other side when there is no walls.
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[49][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 49;
				tempJOfIndex = 0;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			int x = tempNode->iOfIndex, y = tempNode->jOfindex;
			if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex < tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex += 1;
			}
			else if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex > tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex - 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex -= 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex > tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 2;
				tempNode->iOfIndex -= 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex < tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 2;
				tempNode->iOfIndex += 1;
			}
			tempJOfIndex = y;
			tempIOfIndex = x;
		}
	}
	else if (movement == 97 || movement == 65)//going left(ASCI code)
	{
		if (tempNode->jOfindex == 0)
		{
			if (arr[49][tempNode->iOfIndex] == 1)
			{
				return 0;
			}
		}
		else
		{
			if (arr[tempNode->jOfindex - 1][tempNode->iOfIndex] == 1)
			{
				return 0;
			}
		}
		if (arr[tempNode->jOfindex - 1][tempNode->iOfIndex] == 3)
		{
			arr[tempNode->jOfindex - 1][tempNode->iOfIndex] = 0;
			addToSnake(tempNode, movement);
			short int randJ = rand() % 47 + 1;
			short int randI = rand() % 13 + 1;
			while (arr[randJ][randI] != 0)
			{
				randJ = rand() % 47 + 1;
				randI = rand() % 13 + 1;
			}
			arr[randJ][randI] = 3;
		}
		else if (arr[tempNode->jOfindex - 1][tempNode->iOfIndex] == 1)
		{
			return 0;// 0 means we lost the game...
		}
		arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
		if (tempNode->jOfindex == 0)
		{
			nodes *nodeForCheckin = (*head)->nextNode; wayOfPassing = 2;
			while (nodeForCheckin != NULL)
			{
				nodeForCheckin = nodeForCheckin->nextNode;
				numOfNodesToPass++;
			}
			arr[49][tempNode->iOfIndex] = 2;
			(*head)->jOfindex = 49;
		}
		else
		{
			arr[tempNode->jOfindex - 1][(*head)->iOfIndex] = 2;
			(*head)->jOfindex -= 1;
		}
		while (tempNode->nextNode != NULL)
		{
			tempNode = tempNode->nextNode;
			if (tempNode->jOfindex == 0&&wayOfPassing==2)//this is for coming to other side when there is no walls.
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[49][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 49;
				tempJOfIndex = 0;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (wayOfPassing == 3 && numOfNodesToPass > 0 && tempNode->iOfIndex == 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][14] = 2;
				tempNode->iOfIndex = 14;
				tempIOfIndex = 0;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (wayOfPassing == 4 && numOfNodesToPass > 0 && tempNode->iOfIndex == 14)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][0] = 2;
				tempNode->iOfIndex = 0;
				tempIOfIndex = 14;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (tempNode->jOfindex == 49 && numOfNodesToPass > 0 && wayOfPassing == 1)//this is for coming to other side when there is no walls.
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[0][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 0;
				tempJOfIndex = 49;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			int x = tempNode->iOfIndex, y = tempNode->jOfindex;
			if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex > tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex - 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex -= 1;
			}
			else if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex < tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex += 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex > tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 2;
				tempNode->iOfIndex -= 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex < tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 2;
				tempNode->iOfIndex += 1;
			}
			tempJOfIndex = y;
			tempIOfIndex = x;
		}
	}
	else if (movement == 119 || movement == 87)// going up
	{
		if (arr[tempNode->jOfindex][tempNode->iOfIndex - 1] == 2)
		{
			return 0;
		}
		if (tempNode->iOfIndex == 0)
		{
			if (arr[tempNode->jOfindex][14] == 1)
			{
				return 0;
			}
		}
		else
		{
			if (arr[tempNode->jOfindex][tempNode->iOfIndex - 1] == 1)
			{
				return 0;
			}
		}

		if (arr[tempNode->jOfindex][tempNode->iOfIndex - 1] == 3)
		{
			short int randJ;
			short int randI;
			arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 0;
			addToSnake(tempNode, movement);
			randJ = rand() % 47 + 1;
			randI = rand() % 13 + 1;
			while (arr[randJ][randI] != 0)
			{
				randJ = rand() % 47 + 1;
				randI = rand() % 13 + 1;
			}
			arr[randJ][randI] = 3;
		}
		arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
		if (tempNode->iOfIndex == 0)
		{
			nodes *nodeForCheckin = (*head)->nextNode; wayOfPassing = 3;//up
			while (nodeForCheckin != NULL)
			{
				nodeForCheckin = nodeForCheckin->nextNode;
				numOfNodesToPass++;
			}
			arr[tempNode->jOfindex][14] = 2;
			(*head)->iOfIndex = 14;
		}
		else {
			arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 2;
			(*head)->iOfIndex -= 1;
		}
		while (tempNode->nextNode != NULL)
		{
			tempNode = tempNode->nextNode;
			if (tempNode->iOfIndex == 0&&wayOfPassing==3)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][14] = 2;
				tempNode->iOfIndex = 14;
				tempIOfIndex = 0;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (wayOfPassing == 2 && numOfNodesToPass > 0 && tempNode->jOfindex == 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[49][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 49;
				tempJOfIndex = 0;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (wayOfPassing == 1 && numOfNodesToPass > 0 && tempNode->jOfindex == 49)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[0][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 0;
				tempJOfIndex = 49;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (tempNode->iOfIndex == 14 && wayOfPassing == 4 && numOfNodesToPass > 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][0] = 2;
				tempNode->iOfIndex = 0;
				tempIOfIndex = 14;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			int x = tempNode->iOfIndex, y = tempNode->jOfindex;
			if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex > tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex - 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex -= 1;
			}
			else if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex < tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex += 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex > tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 2;
				tempNode->iOfIndex -= 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex < tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 2;
				tempNode->iOfIndex += 1;
			}
			tempJOfIndex = y;
			tempIOfIndex = x;
		}
	}
	else if (movement == 115 || movement == 83)//goint down
	{
		if (arr[tempNode->jOfindex][tempNode->iOfIndex + 1] == 2)
		{
			return 0;
		}
		if (tempNode->iOfIndex == 14)
		{
			if (arr[tempNode->jOfindex][0] == 1)
			{
				return 0;
			}
		}
		else
		{
			if (arr[tempNode->jOfindex][tempNode->iOfIndex + 1] == 1)
			{
				return 0;
			}
		}
		if (arr[tempNode->jOfindex][tempNode->iOfIndex + 1] == 3)
		{
			arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 0;
			addToSnake(tempNode, movement);
			short int randJ = rand() % 47 + 1;
			short int randI = rand() % 13 + 1;
			while (arr[randJ][randI] != 0)
			{
				randJ = rand() % 47 + 1;
				randI = rand() % 13 + 1;
			}
			arr[randJ][randI] = 3;
		}
		arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
		if (tempNode->iOfIndex == 14)
		{
			nodes *nodeForCheckin = (*head)->nextNode; wayOfPassing = 4;
			while (nodeForCheckin != NULL)
			{
				nodeForCheckin = nodeForCheckin->nextNode;
				numOfNodesToPass++;
			}
			arr[tempNode->jOfindex][0] = 2;
			(*head)->iOfIndex = 0;
		}
		else
		{
			arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 2;
			(*head)->iOfIndex += 1;
		}
		while (tempNode->nextNode != NULL)
		{
			tempNode = tempNode->nextNode;
			if (tempNode->iOfIndex == 14&&wayOfPassing==4)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][0] = 2;
				tempNode->iOfIndex = 0;
				tempIOfIndex = 14;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			if (wayOfPassing == 2 && numOfNodesToPass > 0 && tempNode->jOfindex == 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[49][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 49;
				tempJOfIndex = 0;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (wayOfPassing == 1 && numOfNodesToPass > 0 && tempNode->jOfindex == 49)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[0][tempNode->iOfIndex] = 2;
				tempNode->jOfindex = 0;
				tempJOfIndex = 49;
				tempIOfIndex = tempNode->iOfIndex;
				continue;
			}
			if (tempNode->iOfIndex == 0 && wayOfPassing == 3 && numOfNodesToPass > 0)
			{
				numOfNodesToPass--;
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][14] = 2;
				tempNode->iOfIndex = 14;
				tempIOfIndex = 0;
				tempJOfIndex = tempNode->jOfindex;
				continue;
			}
			int x = tempNode->iOfIndex, y = tempNode->jOfindex;
			if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex > tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex - 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex -= 1;
			}
			else if (tempNode->iOfIndex == tempIOfIndex&&tempNode->jOfindex < tempJOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex + 1][tempNode->iOfIndex] = 2;
				tempNode->jOfindex += 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex < tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex + 1] = 2;
				tempNode->iOfIndex += 1;
			}
			else if (tempNode->jOfindex == tempJOfIndex&&tempNode->iOfIndex > tempIOfIndex)
			{
				arr[tempNode->jOfindex][tempNode->iOfIndex] = 0;
				arr[tempNode->jOfindex][tempNode->iOfIndex - 1] = 2;
				tempNode->iOfIndex -= 1;
			}
			tempJOfIndex = y;
			tempIOfIndex = x;
		}
	}
	lastMove = movement;
	printArray(1);
}

void main()//arr[][1] to arr[][13] is available..arr[1][] to arr[48] is available.
{
	srand(time(NULL));
	int move = 0;
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	Node headNode;
	headNode.iOfIndex = 1;
	headNode.jOfindex = 1;
	headNode.nextNode = NULL;
	Node *head = &headNode;

	FILE *ptrToFileOfWalls = fopen("wall.txt", "r");
	if (ptrToFileOfWalls == NULL)
	{
		puts("map not found!.\npress any key to exit...");
		getchar();
		return;
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 51; j++)
		{
			char temp = fgetc(ptrToFileOfWalls);
			if (temp == 48)
				arr[j][i] = 0;
			else if (temp == 49)
				arr[j][i] = 1;
			else if (temp == 50)
				arr[j][i] = 2;
			else if (temp == 51)
				arr[j][i] = 3;
		}
	}
	char temp;
	while (moveSnake(move, &head))
	{
		if (kbhit())
		{
			temp = getch();
			if (temp == 65 || temp == 97 || temp == 83 || temp == 115 || temp == 87 || temp == 119 || temp == 100 || temp == 68)
			{
				move = temp;
			}
		}
		if (move == 65 || move == 97 || move == 68 || move == 100)
		{
			Sleep(90);
		}
		else if (!0)
		{
			Sleep(160);
		}

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	printArray(0);
	getchar();
}