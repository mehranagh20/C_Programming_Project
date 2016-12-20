#include<stdio.h>
#include<Windows.h>

int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 650, 250, TRUE); // 900 width, 500 height


	char *statement;
	FILE *flnm;

	flnm = fopen("exe\\search_statment.txt", "w+");
	statement = (char*)malloc(51);

	printf("\n");
	printf("   Enter search statement (atmost 50 characters):\n");
	printf("   (statement will be colored if found)\n");
	printf("   ");
	fgets(statement, 51, stdin);
	fputs(statement, flnm);

	fclose(flnm);
}