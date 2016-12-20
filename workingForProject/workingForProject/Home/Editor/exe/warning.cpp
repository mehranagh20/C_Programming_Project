#include<stdio.h>
#include<Windows.h>
#include<time.h>

int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 650, 250, TRUE); // 650 width, 250 height

	printf("\n\n");
	printf("      You haven't saved the text in any file!\n");
	printf("      A new window, asking file name will be opened by\n");
	printf("      Pressing an key to save current text in a file. After\n");
	printf("      Save success message you will be asked to open a new file\n");
	printf("      ");
	getchar();
}