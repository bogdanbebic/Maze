#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 80
#define MAX_WIDTH 50
#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 10

#define OUT_FILE_NAME ("outfile.txt")

enum MazeObjects { WALL, PATH, OUT, IN };

typedef struct MazeDimensionsStruct {
	int i;
	int j;
} MazeDimensions;

void printMenu() {
	printf("1. Generate maze\n");
	printf("2. Input maze\n");
	printf("3. Solve maze\n");
	printf("4. Set number of exits\n");
	printf("5. Set coordinates for maze in and exits\n");
	printf("6. Set maze dimensions\n");
	printf("0. Exit program\n");
	printf("Choose menu option: ");
	return;
}

enum MenuOptions { ExitGame, GenerateMaze, InputMaze, SolveMaze, SetNumberOfExits, SetCoordinatesForExitsAndIn, SetMazeDimensions, NumberOfMenuOptions };

int setNumberOfExits(int oldNumberOfExits) {
	int temp;
	printf("Input number of exits: ");
	scanf_s("%d", &temp);
	return temp > 0 ? temp : oldNumberOfExits;
}

MazeDimensions setMazeDimensions(MazeDimensions oldMazeDimensions) {
	MazeDimensions newMazeDimensions;
	printf("Input height of maze: ");
	scanf_s("%d", &newMazeDimensions.i);
	newMazeDimensions.i = newMazeDimensions.i * 2 + 1;
	printf("Input width of maze: ");
	scanf_s("%d", &newMazeDimensions.j);
	newMazeDimensions.j = newMazeDimensions.j * 2 + 1;
	return newMazeDimensions.i > 0 && newMazeDimensions.j > 0 ? newMazeDimensions : oldMazeDimensions;
}

void printMazeToStdout(char **maze, MazeDimensions dimensions) {
	int i, j;
	enum MazeObjects temp;
	for (i = 0; i < dimensions.i; i++) {
		for (j = 0; j < dimensions.j; j++) {
			temp = *(*(maze + i) + j);
			switch (temp) {
			case WALL:
				putchar('#');
				break;
			case PATH:
				putchar(' ');
				break;
			case OUT:
				putchar('o');
				break;
			case IN:
				putchar('i');
				break;
			}
		}
		putchar('\n');
	}
	return;
}

void printMazeToFile(char **maze, MazeDimensions dimensions) {
	int i, j;
	enum MazeObjects temp;
	FILE *outFile;
	fopen_s(&outFile, OUT_FILE_NAME, "w");
	for (i = 0; i < dimensions.i; i++) {
		for (j = 0; j < dimensions.j; j++) {
			temp = *(*(maze + i) + j);
			switch (temp) {
			case WALL:
				fputc('#', outFile);
				break;
			case PATH:
				fputc(' ', outFile);
				break;
			case OUT:
				fputc('o', outFile);
				break;
			case IN:
				fputc('i', outFile);
				break;
			}
		}
		fputc('\n', outFile);
	}
	fclose(outFile);
	return;
}

void deallocateMaze(char **maze, int height) {
	int i;
	for (i = 0; i < height; i++) {
		free(maze[i]);
	}
	free(maze);
	return;
}

char **allocateMaze(char **maze, MazeDimensions dimensions) {
	int i;
	int flag = 0;

	maze = calloc(dimensions.i, sizeof(char *));

	// Memory allocation check
	if (maze == NULL) {
		return NULL;
	}

	for (i = 0; i < dimensions.i; i++) {
		maze[i] = calloc(dimensions.j, sizeof(char));
	}

	// Memory allocation check
	for (i = 0; i < dimensions.i; i++) {
		if (maze[i] == NULL) {
			flag = 1;
		}
	}
	// Error handling for unnsuccessful allocation
	if (flag) {
		deallocateMaze(maze, dimensions.i);
		return NULL;
	}
	return maze;
}

int main() {
	enum MenuOptions menuOption;
	int numberOfExits = 1;
	char **maze = NULL;
	int isGameRunning = 1;
	MazeDimensions dimensions;

	dimensions.i = DEFAULT_HEIGHT * 2 + 1;
	dimensions.j = DEFAULT_WIDTH * 2 + 1;
	maze = allocateMaze(maze, dimensions);

	while (isGameRunning) {
		printMenu();

		scanf_s("%d", &menuOption);
		
		switch (menuOption) {
		case ExitGame:
			deallocateMaze(maze, dimensions.i);
			isGameRunning = 0;
			break;
		case GenerateMaze:

			// TODO: Primov algoritam za generisanje lavirinta

			// TODO: ako je definisano gde su ulazi i izlazi, onda njih stavi
			// TODO: ako nije, random smesti ulaze u i izlaz iz lavirinta

			if (dimensions.i < MAX_HEIGHT && dimensions.j < MAX_WIDTH) {
				printMazeToStdout(maze, dimensions);
			}

			printMazeToFile(maze, dimensions);

			break;
		case InputMaze:
			// TODO: implement
			break;
		case SolveMaze:
			// TODO: BFS za resavanje lavirinta
			// TODO: ispis niza koraka za resavanje
			break;
		case SetNumberOfExits:
			numberOfExits = setNumberOfExits(numberOfExits);
			break;
		case SetCoordinatesForExitsAndIn:
			// TODO: implement
			break;
		case SetMazeDimensions:
			deallocateMaze(maze, dimensions.i);
			dimensions = setMazeDimensions(dimensions);
			maze = allocateMaze(maze, dimensions);
			if (maze == NULL) {
				printf("Unsuccessful allocation");
			}
			break;
		default:
			break;
		}
		putchar('\n');
	}

	return 0;
}
