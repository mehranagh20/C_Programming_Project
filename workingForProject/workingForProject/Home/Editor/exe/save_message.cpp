#include<stdio.h>
#include<Windows.h>
#include<time.h>

int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 650, 250, TRUE); // 650 width, 250 height

	printf("\n\n");
	printf("      File saved successfully!\n");
	Sleep(1500);
}