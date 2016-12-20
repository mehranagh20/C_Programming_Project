#include<stdio.h>
#include<Windows.h>

int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 650, 250, TRUE); // 900 width, 500 height


	char *filename;
	FILE *flnm;

	flnm = fopen("filename.txt", "w+");
	filename = (char*)malloc(50);

	printf("\n");
	printf("   Enter the file name (with its prefix Ex: file.txt):\n");
	printf("   ");
	fgets(filename, 50, stdin);
	fputs(filename, flnm);

	fclose(flnm);
}