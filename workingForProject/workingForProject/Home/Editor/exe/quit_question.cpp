#include<stdio.h>
#include<Windows.h>
#include <conio.h>

int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 650, 250, TRUE); // 650 width, 250 height

	char *filename;
	FILE *flnm;

	flnm = fopen("exe\\quit_choice.txt", "w+");
	filename = (char*)malloc(50);
	char c;

	printf("\n");
	printf("   Are you sure to exit from editor?\n");
	printf("   (Press Enter for Yes or any other key for No)\n");
	printf("   ");
	c = _getch();

	switch (c) {
		case 13:
			c = 1;
			fputc(c, flnm);
			break;

		default:
			c = 0;
			fputc(c, flnm);
	}
	
	fclose(flnm);
}