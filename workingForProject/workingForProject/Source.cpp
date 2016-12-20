#include<string.h>
#include<stdlib.h>
#include<direct.h>
#include<time.h>
#include <stddef.h>
#include<stdio.h>
#include<ctype.h>
#include<windows.h>
#include<string>
#include<conio.h>
#include <atlbase.h>//this is for conversions for recursively removing directory
char pathOfCommandFile[MAX_PATH];//this is the path for the file containing commands.since the directory change we store it at the start.

char realPathOfUsersFile[MAX_PATH];
struct userInfo {
	char name[51];
	char userName[21];
	char password[21];
	char usageTerm[12];
	int accessLevel;
};
struct userInfo user = { "", "", "", "", 1 };// user info
FILE * userPtr; // pointer to users file
void gotoxy(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.Y;
}
void userLogin() {
	int isUserInfoFound(char * username, char * password);//func prototype

	int i, x, y;
	x = 10;
	char username[21] = "";
	char password[21] = "";

	printf("Username: ");
	fgets(username, 21, stdin);

	y = wherey();
	printf("Password: ");
	for (i = 0; i < 20; i++) {
		y = wherey();
		password[i] = _getch();

		if (password[i] == '\r') {
			password[i] = '\n';
			i++;
			break;
		}
		else if (password[i] == 8) {
			if (i == 0) {
				i--;
			}
			else {
				int count = 0;
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("  ");
				}
				i -= 2;
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("* ");
					count++;
				}
				x += count * 2;
				gotoxy(x, y);
			}
		}
		else {
			x = 10;
			gotoxy(x, y);
			for (int j = -1; j < i; j++) {
				printf("* ");
			}
		}
	}
	password[i] = '\0';
	puts("");

	while (!isUserInfoFound(username, password)) {
		printf("Username: ");
		fgets(username, 21, stdin);

		y = wherey();
		printf("Password: ");
		for (i = 0; i < 20; i++) {
			y = wherey();
			password[i] = _getch();

			if (password[i] == '\r') {
				password[i] = '\n';
				i++;
				break;
			}
			else if (password[i] == 8) {
				if (i == 0) {
					i--;
				}
				else {
					int count = 0;
					x = 10;
					gotoxy(x, y);
					for (int j = -1; j < i; j++) {
						printf("  ");
					}
					i -= 2;
					x = 10;
					gotoxy(x, y);
					for (int j = -1; j < i; j++) {
						printf("* ");
						count++;
					}
					x += count * 2;
					gotoxy(x, y);
				}
			}
			else {
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("* ");
				}
			}
		}
		password[i] = '\0';
		puts("");

	}//checks if entered date are found in users file
}//func for user login at the beginning of the program
int isUserInfoFound(char * username, char * password) {
	int userTime(char * string); //func prototype
	fseek(userPtr, 0, SEEK_SET); // set stream at file beginning
	struct userInfo fuser = { "", "", "", "", 1 };

	while (!feof(userPtr)) {
		fread(&fuser, sizeof(fuser), 1, userPtr);
		if ((strcmp(username, fuser.userName) == 0) &&
			(strcmp(password, fuser.password) == 0)) {

			if (!userTime(fuser.usageTerm)) {
				puts("You cannot log into this account because its usage term is finished.\n");
				puts("----------------------------------------");
				return 0;
			}

			user = fuser;
			printf("\nWelcome %s\n", fuser.name);
			puts("----------------------------------------");
			return 1;
		}
	}

	puts("\nusername or password is not correct.\n");
	puts("----------------------------------------");
	return 0;
}//func which checks if entered username and password are found in users file

int isUsernameFound(char * username) {
	rewind(userPtr); // set stream at file beginning
	struct userInfo fuser = { "", "", "", "", 1 };

	while (!feof(userPtr)) {
		fread(&fuser, sizeof(fuser), 1, userPtr);
		if (strcmp(username, fuser.userName) == 0) {
			return 1;
		}
	}

	return 0;
}// this func checks if a specific username is fuoun in users file
void changeAccessTerm(char * term, char *usern) {//
	int isUsernameFound(char * username);
	int isTimeValid(char * term);
	if (term == NULL || usern == NULL)
	{
		printf("wrong usage of command-> passwd -l time userName\n");
		puts("----------------------------------------");
		return;
	}

	if (user.accessLevel) {
		puts("You don't have access to this part :(\n");
		puts("----------------------------------------");
	}
	else {
		struct userInfo fuser = { "", "", "", "", 1 };
		char username[21] = "";
		char newTerm[12] = "";//for entering in the file

		int i;
		for (i = 0; *usern != '\0'; i++) {
			username[i] = *usern;
			usern++;
		}
		username[i] = '\n';
		//assigning \n at the end of username. cause all usernames in file have \n at the end

		strcpy(newTerm, term);
		//term is as the same as new term but needed for validation. bacease time validation function deletes slashes(/) of entered time

		while (!isUsernameFound(username)) {
			printf("Incorrect username. Enter again: ");
			fgets(username, 21, stdin);
		}

		while (!isTimeValid(term)) {
			printf("Incorrect time data. Enter again(Ex: 21/8/2017): ");
			fgets(term, 21, stdin);
			strcpy(newTerm, term);
		}

		rewind(userPtr);
		while (!feof(userPtr)) {
			fread(&fuser, sizeof(fuser), 1, userPtr);

			if (strcmp(username, fuser.userName) == 0) {
				strcpy(user.usageTerm, newTerm);
				strcpy(fuser.usageTerm, newTerm);

				puts("Time changed successfully!\n");
				puts("----------------------------------------");

				fseek(userPtr, -(sizeof(struct userInfo)), SEEK_CUR);
				fwrite(&fuser, sizeof(struct userInfo), 1, userPtr);

				fclose(userPtr); // saving new term
				userPtr = fopen(realPathOfUsersFile, "rb+");//opening users file
				break;
			}
		}
	}
}//func for admin to change users usage term(time)
int isTimeValid(char * term) {
	char termcpy1[21];
	strcpy(termcpy1, term);

	char * tokenPtr;
	tokenPtr = strtok(termcpy1, "/");
	int count;
	for (count = 0; tokenPtr != NULL; count++) {
		tokenPtr = strtok(NULL, "/");
	}

	if (count != 3) {
		return 0;
	}// this checks if time is consisted of 3 parts.


	char * termTokenPtr;
	termTokenPtr = strtok(term, "/");
	int temp;

	int i;
	for (i = 0; *termTokenPtr != '\0'; i++) {
		if (!isdigit(*termTokenPtr)) {
			return 0;
		}
		termTokenPtr++;
	}//checks day to be an integer

	termTokenPtr -= i;
	sscanf(termTokenPtr, "%d", &temp);
	if (temp > 31 || temp < 1) {
		return 0;
	}//checks day to be a number between 1 & 31

	termTokenPtr = strtok(NULL, "/");
	for (i = 0; *termTokenPtr != '\0'; i++) {
		if (!isdigit(*termTokenPtr)) {
			return 0;
		}
		termTokenPtr++;
	}//checks month to be an integer

	termTokenPtr -= i;
	sscanf(termTokenPtr, "%d", &temp);
	if (temp > 12 || temp < 1) {
		return 0;
	}//checks month to be a number between 1 & 12

	termTokenPtr = strtok(NULL, "/");
	for (i = 0; *termTokenPtr != '\0'; i++) {
		if (!isdigit(*termTokenPtr)) {
			return 0;
		}
		termTokenPtr++;
		if (*termTokenPtr == '\n') {
			i++;
			break;
		}
	}//checks year to be an integer

	termTokenPtr -= i;
	sscanf(termTokenPtr, "%d", &temp);
	if (temp < 1000) {
		return 0;
	}//checks year to be a number greater than 1000

	return 1;
}// this func ckecks that time format is valid or not. A valid time format is day/month/year

int userTime(char * string) {
	char *token;
	int day, month, year, y, m;
	struct tm *ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	token = strtok(string, "/, ,\n");
	sscanf(token, "%d", &day);
	token = strtok(NULL, "/, ,\n");
	sscanf(token, "%d", &month);
	token = strtok(NULL, "/, ,\n");
	sscanf(token, "%d", &year);
	y = (ptr->tm_year) - 100;
	y += 2000;
	if ((y) > (year))
		return 0;
	if ((y) < (year))
		return 1;
	if ((y) == (year)) {
		m = (ptr->tm_mon) + 1;
		if (m > month)
			return 0;
		if (m<month)
			return 1;
		if (m == month) {
			if ((ptr->tm_mday)>day)
				return 0;
			if ((ptr->tm_mday) <= day)
				return 1;
		}
	}
}//fuc compare a time with system time. return 1 if time hasn't come & 0 otherwise.
void switchUser(char *tokenPtr) {
	int isUserInfoFound(char * username, char * password);//func prototype

	char username[21] = "";
	char password[21] = "";

	int i, x, y;

	for (i = 0; *tokenPtr != '\0'; i++) {
		username[i] = *tokenPtr;
		tokenPtr++;
	}
	username[i] = '\n';
	//assigning \n at the end of username. cause all usernames in file have \n at the end

	printf("Password: ");
	for (i = 0; i < 20; i++) {
		y = wherey();
		password[i] = _getch();

		if (password[i] == '\r') {
			password[i] = '\n';
			i++;
			break;
		}
		else if (password[i] == 8) {
			if (i == 0) {
				i--;
			}
			else {
				int count = 0;
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("  ");
				}
				i -= 2;
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("* ");
					count++;
				}
				x += count * 2;
				gotoxy(x, y);
			}
		}
		else {
			x = 10;
			gotoxy(x, y);
			for (int j = -1; j < i; j++) {
				printf("* ");
			}
		}
	}
	password[i] = '\0';
	puts("");

	while (!isUserInfoFound(username, password)) {
		printf("Username: ");
		fgets(username, 21, stdin);

		printf("Password: ");
		for (i = 0; i < 20; i++) {
			y = wherey();
			password[i] = _getch();

			if (password[i] == '\r') {
				password[i] = '\n';
				i++;
				break;
			}
			else if (password[i] == 8) {
				if (i == 0) {
					i--;
				}
				else {
					int count = 0;
					x = 10;
					gotoxy(x, y);
					for (int j = -1; j < i; j++) {
						printf("  ");
					}
					i -= 2;
					x = 10;
					gotoxy(x, y);
					for (int j = -1; j < i; j++) {
						printf("* ");
						count++;
					}
					x += count * 2;
					gotoxy(x, y);
				}
			}
			else {
				x = 10;
				gotoxy(x, y);
				for (int j = -1; j < i; j++) {
					printf("* ");
				}
			}
		}
		password[i] = '\0';
		puts("");
	}
}//func for switching user

void adChangePass(char *usern) {
	int isUsernameFound(char * username);
	char username[21] = "";
	char newpass[21] = "";

	int i;
	for (i = 0; *usern != '\0'; i++) {
		username[i] = *usern;
		usern++;
	}
	username[i] = '\n';
	//assigning \n at the end of username. cause all usernames in file have \n at the end

	if (user.accessLevel) {
		puts("You don't have access to this part :(\n");
		puts("----------------------------------------");
	}
	else {
		struct userInfo fuser = { "", "", "", "", 1 };

		while (!isUsernameFound(username)) {
			printf("Incorrect username. Enter again: ");
			fgets(username, 21, stdin);
		}//checking if username exits ot not

		printf("\nNew Password: ");
		fgets(newpass, 21, stdin);

		rewind(userPtr);
		while (!feof(userPtr)) {
			fread(&fuser, sizeof(fuser), 1, userPtr);

			if (strcmp(username, fuser.userName) == 0) {
				strcpy(user.password, newpass);
				strcpy(fuser.password, newpass);

				puts("password changed successfully!\n");
				puts("----------------------------------------");

				fseek(userPtr, -(sizeof(struct userInfo)), SEEK_CUR);
				fwrite(&fuser, sizeof(struct userInfo), 1, userPtr);

				fclose(userPtr); // saving new passowrd
				userPtr = fopen(realPathOfUsersFile, "rb+");//opening users file

				break;
			}
		}
	}
}//func for admin user to change anyone's password
void addUser(FILE *fptr) {
	int isUsernameFound(char * username);
	int isTimeValid(char * term);
	if (user.accessLevel) {
		puts("You don't have access to this part :(\n");
		puts("----------------------------------------");
	}
	else {
		int isTimeValid(char * term);
		struct userInfo fuser = { "", "", "", "", 1 };
		char timecpy[12];//for validating time format

		fseek(fptr, 0, SEEK_END);// putting stream position at the end

		printf("Enter name (50 letters): ");
		fgets(fuser.name, 51, stdin);

		printf("Enter user name (20 letters): ");
		fgets(fuser.userName, 21, stdin);

		while (isUsernameFound(fuser.userName)) {
			printf("This username is used before. Enter another one: ");
			fgets(fuser.userName, 21, stdin);
		}//chacking if username entered is not used already

		printf("Enter password (20 letters): ");
		fgets(fuser.password, 21, stdin);

		printf("Enter usage term (Ex: 21/8/2017): ");
		fgets(fuser.usageTerm, 12, stdin);
		strcpy(timecpy, fuser.usageTerm);

		while (!isTimeValid(timecpy)) {
			printf("Wrong usage term syntax. Enter again (Ex: 21/8/2017): ");
			fgets(fuser.usageTerm, 12, stdin);
			strcpy(timecpy, fuser.usageTerm);
		}//checking for time to be valid

		fwrite(&fuser, sizeof(struct userInfo), 1, fptr);//add record to file
		fclose(fptr);//saving new user
		puts("New user added succesfully!\n");
		puts("----------------------------------------");
		fopen(realPathOfUsersFile, "rb+");//opening users file
	}
}//this function adds user if it's called by admin user

void changePass() {
	rewind(userPtr); // set stream at file beginning
	struct userInfo fuser = { "", "", "", "", 1 };

	char oldpass[21];
	char newpass[21];

	printf("current Password: ");
	fgets(oldpass, 21, stdin);

	if (strcmp(oldpass, user.password) != 0) {
		printf("wrong password.\n\n");
		return;
	}//checks if old pass is entered correctly
	puts("");

	printf("New Password: ");
	fgets(newpass, 21, stdin);

	while (!feof(userPtr)) {
		fread(&fuser, sizeof(fuser), 1, userPtr);
		if (strcmp(user.userName, fuser.userName) == 0) {
			strcpy(user.password, newpass);
			strcpy(fuser.password, newpass);

			puts("Your password changed successfully!\n");
			puts("----------------------------------------");

			fseek(userPtr, -(sizeof(struct userInfo)), SEEK_CUR);
			fwrite(&fuser, sizeof(struct userInfo), 1, userPtr);

			fclose(userPtr); // saving new passowrd
			userPtr = fopen(realPathOfUsersFile, "rb+");//opening users file

			break;
		}
	}
}//func for chaging the user's own password




void time() {
	struct tm *ptr;// tm yek sturct tarif shode dar time.h ast ke shamel chandin no int ast mesle: tm_mon , tm_year ...
	time_t lt;
	lt = time(NULL);// time_t time(time_t *time)
	ptr = localtime(&lt);//struct tm *localtime(time_t *time)

	printf(asctime(ptr));//char *asctime(struct tm *ptr) / asctime yek vorodi b soorat pointer az struct tm migirad va b soorat moghabel namayesh midahad:day month date hours:minutes:second:year\n\0
	puts("----------------------------------------");
}


void clear(char *his) {
	if (his==NULL)
	{
		int i;
		for (i = 0; i < 500; ++i)
			printf("\n");
		gotoxy(0, 0);
	}
	else
	{
		if (strcmp(his, "history") == 0)
		{
			char tempPath[MAX_PATH];
			strcpy(tempPath, pathOfCommandFile);
			char *tmp = strrchr(tempPath, '\\');
			*tmp = '\0';
			strcat(tempPath, "\\Users\\history.txt");
			FILE*ptr = fopen(tempPath, "w");
			fclose(ptr);
		}
		else
		{
			int i;
			for (i = 0; i < 500; ++i)
				printf("\n");
			gotoxy(0, 0);
		}
	}

}

void cal() {
	char *month[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
	char *s;
	int month_day[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	int year, mon, y, i, first_day_jan, j, ii;// fisrt_they_jan neshan dahande 
	struct tm *ptr;// tm yek sturct tarif shode dar time.h ast ke shamel chandin no int ast mesle: tm_mon , tm_year ...
	time_t lt;
	lt = time(NULL);// time_t time(time_t *time)
	ptr = localtime(&lt);//struct tm *localtime(time_t *time)
	year = ptr->tm_year;
	y = year - 100;
	mon = ptr->tm_mon;
	for (first_day_jan = 0, i = 12; i < y; ++i, ++first_day_jan) {
		if ((i % 4) == 0)
			++first_day_jan;
	}
	if ((y % 4) != 0) {//baraye salhaye gheyre kabise
		if (mon == 1)
			first_day_jan += 3;
		if (mon == 2)
			first_day_jan += 3;
		if (mon == 3)
			first_day_jan += 6;
		if (mon == 4)
			first_day_jan += 1;
		if (mon == 5)
			first_day_jan += 4;
		if (mon == 6)
			first_day_jan += 6;
		if (mon == 7)
			first_day_jan += 2;
		if (mon == 8)
			first_day_jan += 5;
		if (mon == 10)
			first_day_jan += 3;
		if (mon == 11)
			first_day_jan += 5;
	}

	if ((y % 4) == 0) {//baraye salhaye kabise
		if (mon == 1)
			first_day_jan += 3;
		if (mon == 2)
			first_day_jan += 4;
		if (mon == 3)
			first_day_jan += 0;
		if (mon == 4)
			first_day_jan += 2;
		if (mon == 5)
			first_day_jan += 5;
		if (mon == 6)
			first_day_jan += 0;
		if (mon == 7)
			first_day_jan += 3;
		if (mon == 8)
			first_day_jan += 6;
		if (mon == 9)
			first_day_jan += 1;
		if (mon == 10)
			first_day_jan += 4;
		if (mon == 11)
			first_day_jan += 6;
	}
	printf("%s 20%d\n\tsu\tmo\tTu\tWe\tTh\tFr\tSa\n\t", month[mon], y);
	if ((y % 4) == 0)//baraye sal kabise
		month_day[1] = 29;
	switch (first_day_jan % 7) {
	case 0:
		for (j = 0, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {//tashkhise emrooz va rangi kardan an
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				printf("\n\t");
				j = 0;
			}
		}
		puts("");
		puts("----------------------------------------");
		break;

	case 1:
		for (j = 1, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	case 2:
		for (j = 9999, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else if (j == 9999) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t\t%d", i);

			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	case 3:
		for (j = 9999, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else if (j == 9999) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t\t\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t\t\t%d", i);

			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	case 4:
		for (j = 9999, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else if (j == 9999) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t\t\t\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t\t\t\t%d", i);

			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	case 5:
		for (j = 9999, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else if (j == 9999) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t\t\t\t\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t\t\t\t\t%d", i);

			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	case 6:
		for (j = 9999, i = 1; i <= (month_day[mon]); ++i, ++first_day_jan) {
			if (j == 0) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("%d", i);
			}
			else if (j == 9999) {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t\t\t\t\t\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t\t\t\t\t\t%d", i);

			}
			else {
				if (i == ptr->tm_mday) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("\t%d", i);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else
					printf("\t%d", i);
			}
			++j;
			if (first_day_jan % 7 == 6) {
				j = 0;
				printf("\n\t");
			}
		}
		puts("");
		puts("----------------------------------------");
		break;
	}

}




char currentPath[MAX_PATH] = "Home";//pwd will print this.
int ifUserHaveAccessToFile(char * nameOfFile)//checks to see if user has access to file.0 if no acc,1 for acc,2 if file does not contain access lvl , 3 if file does not exist.
{
	FILE *filePtr = fopen(nameOfFile, "r");
	if (filePtr != NULL)
	{
		int accLvl;
		char temp[20];
		fscanf(filePtr, "%s %d", temp, &accLvl);
		if (strcmp(temp, "AccessLvl") == 0)
		{
			if (user.accessLevel == accLvl)
			{
				fclose(filePtr);
				return 1;
			}
			fclose(filePtr);
			return 0;
		}
		else
		{
			fclose(filePtr);
			return 2;
		}
	}
	else
	{
		return 3;
	}
}
int ifUserHaveAccessToDir()//this function will check if user have access to a dir
{
	FILE *accessLvlPtr;
	if ((accessLvlPtr = fopen("lvlOfAccess.txt", "r")) != NULL)
	{
		int lvlOfAccessForFolder;
		fscanf(accessLvlPtr, "%d", &lvlOfAccessForFolder);
		if (lvlOfAccessForFolder != user.accessLevel)
		{
			fclose(accessLvlPtr);
			return 0;
		}
		fclose(accessLvlPtr);
	}
	return 1;
}

void mv(char *fileToMove, char *nameOfMovedFile, char *pathToCopy)//if file2 exist we ask if user want to overwrite it (if he has access) if not we create file2 and ask for access lvl.
{
	if (fileToMove == NULL || nameOfMovedFile == NULL)
	{
		printf("wrong usage of command.->'mv file1 file2' or mv file1 file2 :home\\yourDesiredPath...\n");
		puts("----------------------------------------");
	}
	else
	{
		char realPathToCopy[MAX_PATH] = "";//first we check the path that user entered if its valid.
		char currentPathForRestoring[MAX_PATH];
		if (pathToCopy != NULL)//if user has entered sth for path.
		{
			strcpy(currentPathForRestoring, _getcwd(NULL, 0));
			strcpy(realPathToCopy, pathOfCommandFile);
			char *temp = strrchr(realPathToCopy, '\\');
			while (*temp != '\0')
			{
				*temp = '\0';
				temp++;
			}
			temp = strrchr(realPathToCopy, '\\');
			temp++;
			strcpy(temp, pathToCopy);
			if (_chdir(realPathToCopy) != 0)
			{
				printf("you entered wrong path to copy file.\nusage of command.->'mv file1 file2' or mv file1 file2 :home\\yourDesiredPath...\n");
				puts("----------------------------------------");
				return;
			}
			_chdir(currentPathForRestoring);
		}
		else
			strcpy(realPathToCopy, _getcwd(NULL, 0));//if user hasn't enterd any thing for path we copy file in current dir.
		_chdir(realPathToCopy);
		if (ifUserHaveAccessToDir() == 0)//check to see if user has access to destination folder
		{
			printf("your do not have access to destinatoin folder.\n");
			puts("----------------------------------------");
			_chdir(currentPathForRestoring);
			return;
		}
		_chdir(currentPathForRestoring);
		strcat(realPathToCopy, "\\");
		strcat(realPathToCopy, nameOfMovedFile);
		strcat(realPathToCopy, ".txt\0");
		strcat(fileToMove, ".txt");
		int ifUserAccessF1 = ifUserHaveAccessToFile(fileToMove);
		int ifUserAccessF2 = ifUserHaveAccessToFile(realPathToCopy);
		if (ifUserAccessF1 == 0)
		{
			printf("you do not have access to '%s'\n", fileToMove);
			puts("----------------------------------------");
			return;
		}
		if (ifUserAccessF2 == 0)
		{
			printf("you do not have access to '%s'\n", nameOfMovedFile);
			puts("----------------------------------------");
			return;
		}
		if (ifUserAccessF1 == 3)
		{
			printf("the file '%s' does not exist\n", fileToMove);//existence
			puts("----------------------------------------");
			return;
		}
		FILE *fileToMovePtr = fopen(fileToMove, "r");//we dont check for null because we already have checked it in ifUserHasAccessToFile function...
		if (ifUserAccessF1 == 1)// avoid moving accessLvl.
		{
			int temp;
			char tempStr[20];
			fscanf(fileToMovePtr, "%s %d\n", tempStr, &temp);
		}
		FILE *movedFilePtr;
		if (ifUserAccessF2 == 1)
		{
			printf("the file '%s' exist already. do you want to overWrite it? 1 for yes?->", nameOfMovedFile);
			int ifOverWrite;
			scanf("%d", &ifOverWrite);
			getchar();
			if (ifOverWrite == 1)
			{
				movedFilePtr = fopen(realPathToCopy, "w");
				fprintf(movedFilePtr, "%s %d\n", "AccessLvl", 0);
			}
			else
				puts("----------------------------------------");
			return;//if user do not want the existing file to change...
		}
		else if (ifUserAccessF2 == 3)
		{
			movedFilePtr = fopen(realPathToCopy, "w");
			printf("do you want '%s' to have admin AccessLvl? 1 for yes?->", nameOfMovedFile);
			int ifAdminOnly;
			scanf("%d", &ifAdminOnly);
			getchar();
			if (ifAdminOnly == 1)
			{
				fprintf(movedFilePtr, "%s %d\n", "AccessLvl", 0);
			}
		}
		else if (ifUserAccessF2 == 2)//file exist without any acc lvl.
		{
			printf("the file '%s' exist already. do you want to overWrite it? 1 for yes?->", nameOfMovedFile);
			int ifOverWrite;
			scanf("%d", &ifOverWrite);
			getchar();
			if (ifOverWrite == 1)
			{
				movedFilePtr = fopen(realPathToCopy, "w");
			}
			else
				puts("----------------------------------------");
			return;
		}
		while (!feof(fileToMovePtr))
		{
			fputc(fgetc(fileToMovePtr), movedFilePtr);
		}
		fclose(movedFilePtr);
		fclose(fileToMovePtr);
		remove(fileToMove);
		puts("----------------------------------------");
	}

}

void cp(char *fileToCpyFrom, char *nameOfCopiedFile, char *pathToCopy)//if file2 doesn't exist we create it.if it exists we ask user if he wants to copy file1 at the end of file 2 if user has access to it.
//all of functions including this func will consider user accessiblity to directions and files...
{
	if (fileToCpyFrom == NULL || nameOfCopiedFile == NULL)
	{
		printf("wrong usage of command.->'cp file1 file2' or cp file1 file2 :home\\yourDesiredPath...\n");
		puts("----------------------------------------");
	}
	else
	{

		char realPathToCopy[MAX_PATH] = "";//first we check the path that user entered if its valid.
		char currentPathForRestoring[MAX_PATH];
		if (pathToCopy != NULL)//if user has entered sth for path.
		{
			strcpy(currentPathForRestoring, _getcwd(NULL, 0));
			strcpy(realPathToCopy, pathOfCommandFile);
			char *temp = strrchr(realPathToCopy, '\\');
			while (*temp != '\0')
			{
				*temp = '\0';
				temp++;
			}
			temp = strrchr(realPathToCopy, '\\');
			temp++;
			strcpy(temp, pathToCopy);
			if (_chdir(realPathToCopy) != 0)
			{
				printf("you entered wrong path to copy file.\nusage of command.->'cp file1 file2' or cp file1 file2 :home\\yourDesiredPath...\n");
				puts("----------------------------------------");
				return;
			}
			_chdir(currentPathForRestoring);
		}
		else
			strcpy(realPathToCopy, _getcwd(NULL, 0));//if user hasn't enterd any thing for path we copy file in current dir.
		_chdir(realPathToCopy);
		if (ifUserHaveAccessToDir() == 0)//check to see if user has access to destination folder
		{
			printf("your do not have access to destinatoin folder.\n");
			puts("----------------------------------------");
			return;
		}
		_chdir(currentPathForRestoring);
		char fileToCpytoWithExt[20];//stores second argument with txt extension
		strcpy(fileToCpytoWithExt, nameOfCopiedFile);
		strcat(fileToCpytoWithExt, ".txt");
		strcat(fileToCpyFrom, ".txt");
		strcat(realPathToCopy, "\\");
		strcat(realPathToCopy, fileToCpytoWithExt);
		int ifUserAccessFile1 = ifUserHaveAccessToFile(fileToCpyFrom);
		int ifUserAccessFile2 = ifUserHaveAccessToFile(realPathToCopy);
		if (ifUserAccessFile2 == 0)
		{
			printf("you do not have access to '%s'\n", fileToCpytoWithExt);
			puts("----------------------------------------");
			return;
		}
		if (ifUserAccessFile1 == 0)
		{
			printf("you do not have access to '%s'\n", fileToCpyFrom);
			puts("----------------------------------------");
			return;
		}
		else if (ifUserAccessFile1 == 3)
		{
			printf("file '%s' does not exist.\n", fileToCpyFrom);
			puts("----------------------------------------");
			return;
		}
		FILE *ptrToFile1 = fopen(fileToCpyFrom, "r");
		if (ifUserAccessFile1 == 1)//avoid copying accessLvl.
		{
			int temp;
			char tempStr[20];
			fscanf(ptrToFile1, "%s %d\n", tempStr, &temp);
		}
		FILE *ptrToFile2;
		if (ifUserAccessFile2 == 3)//we create non existece file and ask the access lvl of it.
		{
			printf("you are creating '%s'.is this file for admin only? 1 for yes?> ", nameOfCopiedFile);
			int isAccessLvlNeeded;
			scanf("%d", &isAccessLvlNeeded);
			getchar();
			ptrToFile2 = fopen(realPathToCopy, "w");
			if (isAccessLvlNeeded == 1)
				fprintf(ptrToFile2, "%s %d\n", "AccessLvl", 0);
			while (!feof(ptrToFile1))
			{
				fputc(fgetc(ptrToFile1), ptrToFile2);
			}
			puts("----------------------------------------");
			fclose(ptrToFile1);
			fclose(ptrToFile2);
		}
		else
		{
			ptrToFile2 = fopen(realPathToCopy, "a");
			while (!feof(ptrToFile1))
			{
				fputc(fgetc(ptrToFile1), ptrToFile2);
			}
			fclose(ptrToFile1);
			fclose(ptrToFile2);
		}
	}

}

void rm(char *fileName)
{
	if (fileName == NULL)
	{
		printf("wrong usage of command-> rm filename\n");
		puts("----------------------------------------");
		return;
	}
	char fileNameWithExt[20];//we add extension to file.
	strcpy(fileNameWithExt, fileName);
	strcat(fileNameWithExt, ".txt");
	int tempForCheckingAccess = ifUserHaveAccessToFile(fileNameWithExt);
	if (tempForCheckingAccess == 3)
	{
		printf("file '%s' doesn't exist.\n", fileName);
		puts("----------------------------------------");
	}
	else if (tempForCheckingAccess == 0)
	{
		printf("you do not have access to '%s'\n", fileName);
		puts("----------------------------------------");
	}
	else
	{
		int t = remove(fileNameWithExt);
		if (t != 0)
		{
			printf("Could not remove '%s'\n", fileName);
			puts("----------------------------------------");
		}
	}
}

void cat(char *fileName)
{
	if (fileName == NULL)
	{
		printf("wrong usage of command-> cat filename.\n", fileName);
		puts("----------------------------------------");
		return;
	}
	char fileNameWithExt[30];
	strcpy(fileNameWithExt, fileName);
	strcat(fileNameWithExt, ".txt");
	int tempForCheckingAccess = ifUserHaveAccessToFile(fileNameWithExt);
	if (tempForCheckingAccess == 3)//file does not exitst...
	{
		printf("the file %s does not exist.\n", fileName);
	}
	if (tempForCheckingAccess == 1 || tempForCheckingAccess == 2)//if the file does not have accessLvl or user has access.
	{
		FILE *ptrToFile = fopen(fileNameWithExt, "r");
		if (ptrToFile == NULL)
		{
			printf("could not open the file.\n");
			puts("----------------------------------------");
		}
		else
		{
			if (tempForCheckingAccess == 1)//avoid showing accessLvl to user.
			{
				int temp;
				char tempStr[20];
				fscanf(ptrToFile, "%s %d\n", tempStr, &temp);
			}
			puts("");
			while (!feof(ptrToFile))
				putchar(fgetc(ptrToFile));
		}
		puts("");
		fclose(ptrToFile);
	}
	puts("----------------------------------------");
}

void wc(char* fileName)//shows sum of chars and lines...
{
	if (fileName == NULL)
	{
		printf("wrong usage of commadn.-> wc filename\n");
		puts("----------------------------------------");
	}
	else
	{
		char fileNameWithExt[20];
		strcpy(fileNameWithExt, fileName);
		strcat(fileNameWithExt, ".txt");
		int tempForCheckingAccess = ifUserHaveAccessToFile(fileNameWithExt);
		if (tempForCheckingAccess == 3)
		{
			printf("file '%s' does not exist\n", fileName);
			puts("----------------------------------------");
			return;
		}
		if (tempForCheckingAccess == 0)
		{
			printf("you do not have access to this file.\n");
			puts("----------------------------------------");
			return;
		}
		FILE *ptrToFile = fopen(fileNameWithExt, "r");
		if (ptrToFile == NULL)
		{
			printf("could not open '%s' file\n", fileName);
			puts("----------------------------------------");
			return;
		}
		if (tempForCheckingAccess == 1)//we ignore counting accessLvl word...
		{
			int temp;
			char tempStr[20];
			fscanf(ptrToFile, "%s %d\n", tempStr, &temp);
		}
		int iForChars = 0, iForLines = 0;
		while (!feof(ptrToFile))
		{
			if (fgetc(ptrToFile) == '\n')
			{
				iForLines++;
				continue;
			}
			iForChars++;
		}
		printf("number of chars is: %d\nnumber of lines is: %d\n", iForChars, iForLines + 1);
		puts("----------------------------------------");
		fclose(ptrToFile);
	}
}

void writeToEndOfFile(char*fA, char*sA)//fist argu,sec argu.this function is for >>
{
	if (fA != NULL&&sA != NULL)
	{
		sA = strcat(sA, ".txt");//second argument is surly a file.
		char fArgumentTemp[70];//max lengh for text
		strcpy(fArgumentTemp, fA);
		strcat(fArgumentTemp, ".txt");//this is a temp variable to see if there is a txt file with this name
		FILE *fileToWriteInPtr;//sec file
		FILE *fileToReadFromPtr;//first file
		int accessToFile2 = ifUserHaveAccessToFile(sA);//these two variables are used to store returened value of acctofile func.
		int accessToFile1 = ifUserHaveAccessToFile(fArgumentTemp);
		if (accessToFile2 == 0)//check to see if we have access to file 2
		{
			printf("you do not have access to %s .\n", sA);
			puts("----------------------------------------");
			return;
		}
		if (accessToFile1 == 0)// -------to file 1
		{
			printf("you do not have access to %s .\n", fArgumentTemp);
			puts("----------------------------------------");
			return;
		}
		if ((fileToWriteInPtr = fopen(sA, "a")) == NULL)//if file to write in opens unsuccessfully.
		{
			printf("could not open %s ,", sA);
			puts("----------------------------------------");
			return;
		}
		if (accessToFile2 == 3)//if the file to write in does not exist then we have to add it.
		{
			printf("you are creating '%s'.is this file for admin only? 1 for yes?> ", sA);
			int isAccessLvlNeeded;
			scanf("%d", &isAccessLvlNeeded);
			getchar();
			if (isAccessLvlNeeded == 1)
			{
				fprintf(fileToWriteInPtr, "%s %d\n", "AccessLvl", 0);
			}
			puts("----------------------------------------");
		}
		if ((fileToReadFromPtr = fopen(fArgumentTemp, "r")) != NULL)//if first argument is a file not text
		{
			if (accessToFile1 == 1)//this prevent the accessLvl text to be copied to new file.
			{
				int temp;
				char tempStr[20];
				fscanf(fileToReadFromPtr, "%s %d\n", tempStr, &temp);
			}
			while (!feof(fileToReadFromPtr))
			{
				fputc(fgetc(fileToReadFromPtr), fileToWriteInPtr);//we copy every char in file1 to file2.
			}
			fclose(fileToWriteInPtr);
			fclose(fileToReadFromPtr);
		}
		else//the first argument is simple text.
		{
			fprintf(fileToWriteInPtr, "%s", fA);
			fclose(fileToWriteInPtr);
		}
	}
	else
	{
		puts("wrong user of command-> file1>>file2 or text>>file");
		puts("----------------------------------------");
	}

}

void writeToFile(char *fA, char*sA)//if this is new file we create and ask for access lvl.if it exists we check permission.
{
	if (fA != NULL&&sA != NULL)
	{
		sA = strcat(sA, ".txt");//second argument is surly a file.
		char fArgumentTemp[70];//max lengh for text
		strcpy(fArgumentTemp, fA);
		strcat(fArgumentTemp, ".txt");//this is a temp variable to see if there is a txt file with this name
		FILE *fileToWriteInPtr;//sec file
		FILE *fileToReadFromPtr;//first file
		int accessToFile2 = ifUserHaveAccessToFile(sA);//these two variables are used to store returened value of acctofile func.
		int accessToFile1 = ifUserHaveAccessToFile(fArgumentTemp);
		if (accessToFile2 == 0)//check to see if we have access to file 2
		{
			printf("you do not have access to %s .\n", sA);
			puts("----------------------------------------");
			return;
		}
		if (accessToFile1 == 0)// -------to file 1
		{
			printf("you do not have access to %s .\n", fArgumentTemp);
			puts("----------------------------------------");
			return;
		}
		if ((fileToWriteInPtr = fopen(sA, "w")) == NULL)//if file to write in opens unsuccessfully.
		{
			printf("could not open %s ,", sA);
			puts("----------------------------------------");
			return;
		}
		if (accessToFile2 == 1)//if the file already exist and had access lvl. we have to add it again.
		{
			fprintf(fileToWriteInPtr, "%s %d\n", "AccessLvl", 0);
		}
		if (accessToFile2 == 3)//if the file to write in does not exist then we have to add it.
		{
			printf("you are creating '%s'.is this file for admin only? 1 for yes?> ", sA);
			int isAccessLvlNeeded;
			scanf("%d", &isAccessLvlNeeded);
			getchar();
			if (isAccessLvlNeeded == 1)
			{
				fprintf(fileToWriteInPtr, "%s %d\n", "AccessLvl", 0);
			}
			puts("----------------------------------------");
		}
		if ((fileToReadFromPtr = fopen(fArgumentTemp, "r")) != NULL)//if first argument is a file not text
		{
			if (accessToFile1 == 1)//this prevent the accessLvl text to be copied to new file.
			{
				int temp;
				char tempStr[20];
				fscanf(fileToReadFromPtr, "%s %d\n", tempStr, &temp);
			}
			while (!feof(fileToReadFromPtr))
			{
				fputc(fgetc(fileToReadFromPtr), fileToWriteInPtr);//we copy every char in file1 to file2.
			}
			fclose(fileToWriteInPtr);
			fclose(fileToReadFromPtr);
		}
		else//the first argument is simple text.
		{
			fprintf(fileToWriteInPtr, "%s", fA);
			fclose(fileToWriteInPtr);
		}
	}
	else
	{
		puts("wrong user of command-> file1>file2 or text>file");
		puts("----------------------------------------");
	}
}
void cDirectory(char *destination)//will change directory base on access lvl of user.
{
	if (destination == NULL)
	{
		puts("Wrong use of command-> cd dirName.");
		puts("----------------------------------------");
		return;
	}
	char *temp = strrchr(_getcwd(NULL, 0), '\\');//we can not get out of home folder.(the main folder of operating system).
	if (strcmp(temp, "\\home") == 0 && strcmp(destination, "..") == 0)
	{
		return;
	}
	if (strcmp(destination, "..") == 0)
	{
		_chdir("..");
		temp = strrchr(currentPath, '\\');
		if (temp != NULL)
			while (*temp != '\0')//we change the currentdirectory for pwd
			{
				*temp = '\0';
				temp++;
			}
		return;
	}
	if (!_chdir(destination) == 0)
	{
		printf("directory '%s' not found.\n", destination);
		puts("----------------------------------------");
		return;
	}
	if (ifUserHaveAccessToDir() == 0)
	{
		printf("you do not have permission for change to '%s' directory.\n", destination);
		cDirectory("..");
		puts("----------------------------------------");
		return;
	}
	//if we got here means the directory has changed successfully!
	strcat(currentPath, "\\");//this is for pwd...
	strcat(currentPath, destination);
}
void ls()
{
	puts("");
	CONSOLE_SCREEN_BUFFER_INFO scInfo;
	WORD currColor;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scInfo);
	currColor = scInfo.wAttributes;//saving current color of console.
	using namespace std;
	USES_CONVERSION;
	string tempStr = _getcwd(NULL, 0); tempStr += "\\*";
	LPCSTR tempFN = tempStr.c_str();
	LPCWSTR folderName = A2W(tempFN);
	WIN32_FIND_DATA FindFileData;
	HANDLE handle;
	int i = 1;
	handle = FindFirstFile(folderName, &FindFileData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(handle, &FindFileData))
		{
			if (_tcscmp(FindFileData.cFileName, TEXT("..")) != 0)
			{
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				}
				_tprintf(TEXT("%-35s"), FindFileData.cFileName);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), currColor);
				if (i % 2 == 0)
					puts("");
				i++;
			}
		}
	}
	puts("");
	puts("----------------------------------------");
	FindClose(handle);
}

int deleteNonEmpDir(LPCWSTR path, LPCWSTR folderName)//we should have checked all directories for acclvl before deleting but we had to check all directories first and them start to delete files.so i bikhialed.
{
	using namespace std;
	WIN32_FIND_DATA FindFileData;
	HANDLE handle;
	handle = FindFirstFile(path, &FindFileData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(handle, &FindFileData) != 0)
		{
			if (_tcscmp(FindFileData.cFileName, TEXT("..")) != 0)
			{
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					SetCurrentDirectory(folderName);
					wstring temp = wstring(FindFileData.cFileName) + wstring(L"\\*");
					LPCWSTR fpath = temp.c_str();
					deleteNonEmpDir(fpath, FindFileData.cFileName);
					_chdir("..");
				}
				else
				{
					SetCurrentDirectory(folderName);
					DeleteFile(FindFileData.cFileName);
					_chdir("..");
				}
			}
		}
		FindClose(handle);
		RemoveDirectory(folderName);
		return 1;
	}
}

void rmDirection(char *dirName)//this function will remove even non-empty directories with a recurisive func.
{
	using namespace std;
	if (_chdir(dirName) == 0)
	{
		if (ifUserHaveAccessToDir() == 0)//check to see if user have permission to remove the folder.
		{
			puts("you do not have access to this folder.");
			puts("----------------------------------------");
			_chdir("..");
			return;
		}
		_chdir("..");
		USES_CONVERSION;//next 7lines prepare char* so it can be used as LPCWSTR(idk why!!!)
		string tempStr = dirName;
		LPCSTR tempFN = tempStr.c_str();
		LPCWSTR folderName = A2W(tempFN);
		tempStr += "\\*";
		LPCSTR tempLp = tempStr.c_str();
		LPCWSTR realPath = A2W(tempLp);
		deleteNonEmpDir(realPath, folderName);
		_rmdir(dirName);
		//}
	}
	else
	{
		printf("there is no directory called '%s' to remove.\n", dirName);
		puts("----------------------------------------");
	}
}
void mvdir(char *dirToMove, char *movedDirName, char *path)
{
	if (dirToMove == NULL || movedDirName == NULL)
	{
		printf("wrong usage of command.-> mvdir DirToMove NameOfMovedDir or mvdir DirToMove NameOfMovedDir:home//yourDesiredDestination\n");
		puts("----------------------------------------");
		return;
	}
	if (_chdir(dirToMove) != 0)
	{
		printf("there is no directory called %s to move\n", dirToMove);
		puts("----------------------------------------");
		return;
	}
	if ((ifUserHaveAccessToDir()) == 0)
	{
		printf("you do not have access to %s\n", dirToMove);
		puts("----------------------------------------");
		_chdir("..");
		return;
	}
	_chdir("..");
	char realPathToMove[MAX_PATH], pathOfRestoring[MAX_PATH];
	strcpy(pathOfRestoring, _getcwd(NULL, 0));
	strcpy(realPathToMove, pathOfCommandFile);
	char *tmpPtr = strrchr(realPathToMove, '\\');
	*tmpPtr = '\0';
	tmpPtr = strrchr(realPathToMove, '\\');
	*(tmpPtr++) = '\0';
	USES_CONVERSION;
	using namespace std;
	string iDir = dirToMove, fDir = movedDirName, des;
	if (path == NULL)
		des = fDir;
	else
	{
		des = realPathToMove;
		des += "\\";
		des += path;
		const char*p = des.c_str();
		if (_chdir(p) != 0)
		{
			printf("the entered path %s is wrong.\n", path);
			puts("----------------------------------------");
			return;
		}
		if (ifUserHaveAccessToDir() == 0)
		{
			printf("you do not have access to destination folder.\n");
			puts("----------------------------------------");
			_chdir(pathOfRestoring);
			return;
		}
		if (_chdir(movedDirName) == 0)
		{
			printf("a folder with this name already exist.\n");
			puts("----------------------------------------");
			_chdir(pathOfRestoring);
			return;
		}
		_chdir(pathOfCommandFile);
		des += "\\";
		des += fDir;
	}
	LPCSTR temp;
	temp = iDir.c_str();
	LPCWSTR a = A2W(temp); temp = des.c_str();
	LPCWSTR b = A2W(temp);
	if (!MoveFileEx(a, b, MOVEFILE_COPY_ALLOWED))
	{
		printf("could not move the dir.wrong path maybe!\n");
		puts("----------------------------------------");
	}

}
void mkDirection(char *dirName)
{
	if (dirName != NULL)
	{
		if (_chdir(dirName) != 0)//we check to see if there is a folder with same name...
		{
			char tempForPath[MAX_PATH];
			strcpy(tempForPath, _getcwd(NULL, 0));
			strcat(tempForPath, "\\");
			if (_mkdir(strcat(tempForPath, dirName)) != 0)
			{
				puts("could not create folder.");
				puts("----------------------------------------");
			}
			else//we ask user to see who can access this new dir...
			{
				int tempAccLvlOfDir;
				printf("do you want just the root user to access this directory? press 1 for yes?-> ");
				scanf("%d", &tempAccLvlOfDir);
				getchar();
				if (tempAccLvlOfDir == 1)//we create a file with access lvl of 0 in this new dir.
				{
					_chdir(dirName);
					FILE *tempFilePtr;
					tempFilePtr = fopen("lvlOfAccess.txt", "w");
					fprintf(tempFilePtr, "%d", 0);
					_chdir("..");
					fclose(tempFilePtr);
				}
				puts("----------------------------------------");
			}
		}
		else
		{
			cDirectory("..");
			puts("there is already a folder with this name.");
			puts("----------------------------------------");
		}
	}
	else
		puts("wrong use of commadn-> mkdir fileName");

}
void pwd()
{
	printf("%s\n", currentPath);
	puts("----------------------------------------");
}
void printCurrentDirectory()//will print current path ...like Home:\>
{
	char *temp;
	char tempUs[20];
	strcpy(tempUs, user.userName);
	if ((temp = strchr(tempUs, '\n')) != NULL)
		*temp = '\0';
	printf("%s:~", tempUs);
	char currPa[MAX_PATH];
	temp = _getcwd(currPa, MAX_PATH);
	strcpy(currPa, temp);
	temp = strrchr(currPa, '\\');
	printf("%s:\> ", temp);
}
void help(char *command)
{
	char tempPath[MAX_PATH];
	char currPath[MAX_PATH];
	strcpy(currPath, _getcwd(NULL, 0));
	strcpy(tempPath, pathOfCommandFile);
	char *tmp=strrchr(tempPath, '\\');
	*tmp = '\0';
	strcat(tempPath, "\\help");
	if (command == NULL)
	{
		_chdir(tempPath);
		cat("summaryOfCommands");
		_chdir(currPath);
		
	}
	else
	{
		char cmp[30],c;
		strcat(tempPath, "\\help.txt");
		FILE *ptr = fopen(tempPath, "r");
		if (ptr != NULL)
		{
			while (!feof(ptr))
			{
				fscanf(ptr, "|%s\n", cmp);
				if (strcmp(cmp, command) == 0)
				{
					puts("");
					while ((c = fgetc(ptr)) != '$')
						putchar(c);
					puts("");
					puts("----------------------------------------\n");
					fclose(ptr);
					return;

				}
				while (fgetc(ptr) != '$'&&!feof(ptr)); fgetc(ptr);
			}
		}
		else
			printf("could not open help.txt\n");
	}
}

void history()
{
	char tempPath[MAX_PATH];
	strcpy(tempPath, pathOfCommandFile);
	char *tmp = strrchr(tempPath, '\\');
	*tmp = '\0';
	strcat(tempPath, "\\Users\\history.txt");
	FILE*ptr = fopen(tempPath, "r");
	puts("");
	while (!feof(ptr))
		putchar(fgetc(ptr));
	puts("\n----------------------------------------\n");
	fclose(ptr);
}
using namespace std;
int search(char *fileName,char *pathOfFoundFile,string p)
{
	if (fileName == NULL)
	{
		printf("wrong usage of command-> search filename\n");
		puts("\n----------------------------------------\n");
		return 0;
	}
	USES_CONVERSION;//next 7lines prepare char* so it can be used as LPCWSTR(idk why!!!)
	string tempStr = fileName;
	LPCSTR tempFN = tempStr.c_str();
	LPCWSTR folderName = A2W(tempFN);
	tempStr = p;
	tempStr += "\\*";
	LPCSTR tempLp = tempStr.c_str();
	LPCWSTR realPath = A2W(tempLp);
	WIN32_FIND_DATA FindFileData;
	HANDLE handle;
	handle = FindFirstFile(realPath, &FindFileData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(handle, &FindFileData) != 0)
		{
			if (_tcscmp(FindFileData.cFileName, TEXT("..")) != 0)
			{
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					wchar_t t[40];
					wcscpy(t, FindFileData.cFileName);
					int i = wcslen(t);
					while (i >= 0)
					{
						t[i] = towlower(t[i]);
						i--;
					}

					if (wcscmp(t, folderName) == 0)
					{
						strcpy(pathOfFoundFile, p.c_str());
						return 1;
					}
					wstring temp = wstring(FindFileData.cFileName);
					string x = p;
					string y(temp.begin(), temp.end());
					x += "\\" ;
					x += y;
					if (search(fileName, pathOfFoundFile, x) == 1)
					{
						return 1;
					}
				}
				else
				{
					wchar_t x[40];
					wcscpy(x, FindFileData.cFileName);
					int i = wcslen(x);
					while (i >= 0)
					{
						x[i] = towlower(x[i]);
						i--;
					}
						
					if (wcscmp(x,folderName)==0)
					{
						strcpy(pathOfFoundFile, p.c_str());
						return 1;
					}
				}
			}
		}
		FindClose(handle);
	}
	return 0;
}
void timer()
{
	USES_CONVERSION;
	char path[MAX_PATH];
	strcpy(path, pathOfCommandFile);
	char *temp=strrchr(path, '\\'); *(++temp) = '\0';
	string s = path;
	s += "games\\timer.exe";
	LPCSTR t= s.c_str();
	LPCWSTR tmp = A2W(t);
	ShellExecute(HWND_DESKTOP, L"open",tmp, NULL, NULL, SW_SHOW);
}
void runEditor(char *fileName)
{
	char editorFolder[MAX_PATH];
	char currentPath[MAX_PATH];
	strcpy(currentPath, _getcwd(NULL, 0));
	USES_CONVERSION;
	char tempFileName[40];
	if (fileName != NULL)
	{
		strcpy(tempFileName, fileName);
		strcat(tempFileName, ".txt");
	}
	char pathOfEditor[MAX_PATH];
	strcpy(pathOfEditor, pathOfCommandFile);
	char *tmp = strrchr(pathOfEditor, '\\'); *(++tmp) = '\0';
	strcpy(editorFolder, pathOfEditor);
	strcat(editorFolder, "editor");
	string pathForEditor = pathOfEditor;
	pathForEditor += "editor\\ExampPracticre.exe";
	LPCSTR tmpD = pathForEditor.c_str();
	LPCWSTR realPathOfEditor = A2W(tmpD);
	strcat(pathOfEditor, "editor\\pathFile.txt");
	if (fileName == NULL)
	{
		FILE *ptr = fopen(pathOfEditor, "w");
		fclose(ptr);
		_chdir(editorFolder);
		ShellExecute(HWND_DESKTOP, L"open", realPathOfEditor, NULL, NULL, SW_SHOW);
		_chdir(currentPath);
	}
	else//checks to see if user has access to file.0 if no acc,1 for acc,2 if file does not contain access lvl , 3 if file does not exist.
	{
		FILE *ptr = fopen(pathOfEditor, "w");
		int t = ifUserHaveAccessToFile(tempFileName);
		if (t == 0)
		{
			printf("you do not have access to %s\n", fileName);
			puts("\n----------------------------------------\n");
			fclose(ptr);
			return;
		}
		if (t == 1 || t == 2)
		{
			fprintf(ptr, "%s\\%s", _getcwd(NULL, 0), tempFileName);
			fclose(ptr);
			_chdir(editorFolder);
			ShellExecute(HWND_DESKTOP, L"open", realPathOfEditor,NULL, NULL, SW_SHOW);
			//ptr=fopen(pathOfEditor, "w");
			//fclose(ptr);
			_chdir(currentPath);
		}
		else if (t == 3)
		{
			fprintf(ptr, "%s\\%s", _getcwd(NULL, 0), tempFileName);
			fclose(ptr);
			ptr = fopen(tempFileName, "w");
			fclose(ptr);
			_chdir(editorFolder);
			ShellExecute(HWND_DESKTOP, L"open", realPathOfEditor, NULL, NULL, SW_SHOW);
			//ptr = fopen(pathOfEditor, "w");
			//fclose(ptr);
			_chdir(currentPath);
		}
	}
}

int getUserCommand(char *userEnteredCommand)//will recognize the user command
{
	char *tempForToLower;
	char temp[100];//we dont want the userEnteredCommand to change so we work with temp.(tokenizing will change the string??).
	char command[20];//for storing command from file
	int commandId;
	FILE *ptrToCommandFile;
	if ((ptrToCommandFile = fopen(pathOfCommandFile, "r")) == NULL)
	{
		printf("Could not open Command file.\n");
		return 0;
	}
	fgets(userEnteredCommand, 49, stdin);
	if (strcmp(userEnteredCommand, "\n") == 0)
	{
		return -1;//if user enters without any command...
	}
	userEnteredCommand[strlen(userEnteredCommand) - 1] = '\0';
	short int i = 0;
	while (i < strlen(userEnteredCommand))
	{
		userEnteredCommand[i] = tolower(userEnteredCommand[i]);
		i++;
	}
	strcpy(temp, userEnteredCommand);
	char * tokenPtr = strtok(temp, " ");
	while (!feof(ptrToCommandFile))
	{

		fscanf(ptrToCommandFile, "%s %d\n", command, &commandId);
		if (strcmp(tokenPtr, command) == 0)
		{
			char tempPath[MAX_PATH];
			strcpy(tempPath, pathOfCommandFile);
			char *tmp=strrchr(tempPath, '\\');
			*tmp = '\0';
			strcat(tempPath, "\\Users\\history.txt");
			FILE*ptr = fopen(tempPath, "a+");
			if (ptr == NULL)
				printf("could not open history\n");
			else
			{
				fprintf(ptr, "%s\n", userEnteredCommand);
			}
			fclose(ptr);
			return commandId;
		}
	}
	fclose(ptrToCommandFile);
	if ((tokenPtr = strstr(userEnteredCommand, ">>")) != NULL)
		return 20;//comman >> may be executed...
	if ((tokenPtr = strstr(userEnteredCommand, ">")) != NULL)
		return 19;
	return 0;
}

int executeUserCommand()
{
	string x;
	char *temp;
	char *firstTokenOfComm;
	char *secondTokenOfComm;//commands will have at most to tokens.(useful data)
	char userEnteredCommand[100];
	int commandId = getUserCommand(userEnteredCommand);
	while (commandId == 0 || commandId == -1)//while command is wrong or just an enter
	{
		if (commandId == 0)
		{
			puts("wrong command.");
			puts("----------------------------------------");//40 dashes
			puts("");
		}
		printCurrentDirectory();
		commandId = getUserCommand(userEnteredCommand);
	}
	char userEnteredCommandForTok[50];//these 5 lines tokens the recognized command for different functions...
	strcpy(userEnteredCommandForTok, userEnteredCommand);
	firstTokenOfComm = strtok(userEnteredCommandForTok, " ");
	firstTokenOfComm = strtok(NULL, " ");
	if (strchr(userEnteredCommandForTok, ':') != 0)
	{
		secondTokenOfComm = strtok(NULL, " ");
	}
	else
	{
		secondTokenOfComm = strtok(NULL, ":");
	}
	switch (commandId)
	{
	case 1://su
		switchUser(firstTokenOfComm);
		clear(NULL);
		printf("\nWelcome %s\n", user.name);
		puts("----------------------------------------");
		break;
	case 2://passwd
		if (firstTokenOfComm == NULL)
		{
			changePass();
			break;
		}
		if (strcmp(firstTokenOfComm, "-l") == 0)
		{
			firstTokenOfComm = strtok(userEnteredCommand, " ");
			firstTokenOfComm = strtok(NULL, " ");
			firstTokenOfComm = strtok(NULL, " ");
			secondTokenOfComm = strtok(secondTokenOfComm, " ");
			secondTokenOfComm = strtok(NULL, " ");
			changeAccessTerm(firstTokenOfComm, secondTokenOfComm);
			break;
		}
		else
			adChangePass(firstTokenOfComm);
		break;
	case 3://call
		cal();
		break;
	case 4://clear
		clear(firstTokenOfComm);
		break;
	case 5://exit
		return 0;
		break;
	case 6://history
		history();
		break;
	case 7://cd
		cDirectory(firstTokenOfComm);
		break;
	case 8://pwd
		pwd();
		break;
	case 9://mkdir
		mkDirection(firstTokenOfComm);
		break;
	case 10://cat
		cat(firstTokenOfComm);
		break;
	case 11://rm
		rm(firstTokenOfComm);
		break;
	case 12://rmdir
		rmDirection(firstTokenOfComm);
		break;
	case 13://cp
		temp = strtok(userEnteredCommand, ":");
		temp = strtok(NULL, ":");
		cp(firstTokenOfComm, secondTokenOfComm, temp);
		break;
	case 14://mv
		temp = strtok(userEnteredCommand, ":");
		temp = strtok(NULL, ":");
		mv(firstTokenOfComm, secondTokenOfComm, temp);
		break;
	case 15://wc
		wc(firstTokenOfComm);
		break;
	case 16://myeditor
		runEditor(firstTokenOfComm);
		break;
	case 17://snake
		char tempForSnakePath[MAX_PATH];
		char pathForRestoring[MAX_PATH];
		strcpy(pathForRestoring, _getcwd(NULL, 0));
		strcpy(tempForSnakePath, pathOfCommandFile);
		temp = strrchr(tempForSnakePath, '\\');
		strcpy(temp, "\0");
		strcat(tempForSnakePath, "\\games\\snakegame\\release");
		_chdir(tempForSnakePath);
		ShellExecute(HWND_DESKTOP, L"open", L"snakegame.exe", NULL, NULL, SW_SHOW);
		_chdir(pathForRestoring);
		break;
	case 18://help
		help(firstTokenOfComm);
		break;
	case 19://>
		firstTokenOfComm = strtok(userEnteredCommand, ">");
		secondTokenOfComm = strtok(NULL, ">");
		writeToFile(firstTokenOfComm, secondTokenOfComm);
		break;
	case 20://>>
		firstTokenOfComm = strtok(userEnteredCommand, ">>");
		secondTokenOfComm = strtok(NULL, ">>");
		while (*firstTokenOfComm == ' ')// we ignore spaces because it is going to be a problem for file names...
		{
			firstTokenOfComm++;
		}
		while (*secondTokenOfComm == ' ')
		{
			secondTokenOfComm++;
		}
		writeToEndOfFile(firstTokenOfComm, secondTokenOfComm);
		break;
	case 21://time
		time();
		break;
	case 22://adduser
		addUser(userPtr);
		break;
	case 23://ls
		ls();
		break;//mvdir
	case 24:
		temp = strtok(userEnteredCommand, ":");
		temp = strtok(NULL, ":");
		mvdir(firstTokenOfComm, secondTokenOfComm, temp);
		break;
	case 25://search
		char pathOfFoundFile[MAX_PATH];
		char homePath[MAX_PATH];
		strcpy(homePath, pathOfCommandFile);
		temp = strrchr(homePath, '\\'); *temp = '\0';
		x = homePath;
		if (search(firstTokenOfComm, pathOfFoundFile, x) == 1)
		{
			temp = strstr(pathOfFoundFile, "home");
			printf("found in : \n%s\n",temp);
			puts("----------------------------------------\n");
		}
		else
		{
			printf("%s not found\n",firstTokenOfComm);
			puts("----------------------------------------\n");
		}
		break;
	case 26:
		timer();
		break;
	default:
		break;
	}
	return 1;
}

void main()
{
	strcpy(realPathOfUsersFile, _getcwd(NULL, 0));
	strcat(realPathOfUsersFile, "\\home\\users\\users.txt");
	userPtr = fopen(realPathOfUsersFile, "rb+");//opening users file
	userLogin();
	_chdir("home");//at the first we are in the main folder called home...
	strcpy(pathOfCommandFile, _getcwd(NULL, 0));
	strcat(pathOfCommandFile, "\\commands.txt");
	int ex = 1;
	char tempPath[MAX_PATH];//cleanin history when user exit the os
	strcpy(tempPath, pathOfCommandFile);
	char *tmp = strrchr(tempPath, '\\');
	*tmp = '\0';
	strcat(tempPath, "\\Users\\history.txt");
	FILE*ptr = fopen(tempPath, "w");
	fclose(ptr);
	while (ex)//ex=0 means we exit the program...
	{
		printCurrentDirectory();
		ex = executeUserCommand();
	}
}