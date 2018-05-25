#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 80
#define MAX_WIDTH 50

void printMenu() {
	printf("1. Generate maze\n");
	printf("2. Input maze\n");
	printf("3. Set number of exits\n");
	printf("4. Set maze dimensions\n");
	printf("0. Exit program\n");
	printf("Choose menu option: ");
	return;
}

enum MenuOptions { ExitGame, GenerateMaze, InputMaze, SetNumberOfExits, SetMazeDimensions, NumberOfMenuOptions };

int main() {

	enum MenuOptions menuOption;

	int numberOfExits = 1;

	int temp;

	while (1) {
		printMenu();

		scanf_s("%d", &menuOption);
		
		switch (menuOption) {
		case ExitGame:
			// TODO: implement deallocation if necessary
			exit(0);
			break;
		case GenerateMaze:
			// TODO: implement
			// TODO: Primov algoritam za generisanje lavirinta
			break;
		case InputMaze:
			// TODO: implement
			break;
		case SetNumberOfExits:
			printf("Input number of exits: ");
			scanf_s("%d", &temp);
			numberOfExits = temp > 0 ? temp : numberOfExits;
			break;
		case SetMazeDimensions:
			// TODO: implement
			break;
		default:
			break;
		}
		putchar('\n');
	}

	return 0;
}
