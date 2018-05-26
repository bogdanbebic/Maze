#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 80
#define MAX_WIDTH 50
#define DEFAULT_HEIGHT 5
#define DEFAULT_WIDTH 5

#define MAX_EXITS 10

#define OUT_FILE_NAME ("outfile.txt")

#define hashCoordinates(x) ((x) * 2 + 1)
#define unhashCoordinates(x) (((x) - 1) / 2)


enum MazeObjects { WALL, IN, OUT, PATH };

enum MenuOptions { ExitGame, GenerateMaze, InputMaze, SolveMaze, SetNumberOfExits, SetCoordinatesForExitsAndIn, SetMazeDimensions, NumberOfMenuOptions };

typedef struct MazeDimensionsStruct {
	int i;
	int j;
} MazeDimensions;

typedef struct CoordinatesStruct {
	int i;
	int j;
} Coordinates;

typedef int MazeCell;


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


Coordinates readCoordinatesForEdges() {
	Coordinates coordinates;
	printf("Input i coordinate: ");
	scanf_s("%d", &coordinates.i);
	printf("Input j coordinate: ");
	scanf_s("%d", &coordinates.j);
	return coordinates;
}


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
	newMazeDimensions.i = hashCoordinates(newMazeDimensions.i);
	printf("Input width of maze: ");
	scanf_s("%d", &newMazeDimensions.j);
	newMazeDimensions.j = hashCoordinates(newMazeDimensions.j);
	return newMazeDimensions.i > 0 && newMazeDimensions.j > 0 ? newMazeDimensions : oldMazeDimensions;
}

Coordinates setCoordinates(Coordinates oldCoordinates, MazeDimensions dimensions) {
	Coordinates newCoordinates;
	printf("Input i coordinate: ");
	scanf_s("%d", &newCoordinates.i);
	newCoordinates.i = hashCoordinates(newCoordinates.i);
	printf("Input j coordinate: ");
	scanf_s("%d", &newCoordinates.j);
	newCoordinates.j = hashCoordinates(newCoordinates.j);
	newCoordinates = newCoordinates.i > 0 && newCoordinates.j > 0 ? newCoordinates : oldCoordinates;
	newCoordinates = newCoordinates.i < dimensions.i && newCoordinates.j < dimensions.j ? newCoordinates : oldCoordinates;
	return newCoordinates;
}


void printMazeToStdout(MazeCell **maze, MazeDimensions dimensions) {
	int i, j;
	enum MazeObjects temp;
	for (i = 0; i < dimensions.i; i++) {
		for (j = 0; j < dimensions.j; j++) {
			temp = *(*(maze + i) + j);
			switch (temp) {
			case WALL:
				putchar('#');
				break;
			case IN:
				putchar('i');
				break;
			case OUT:
				putchar('o');
				break;
			default:
				putchar(' ');
				break;
			}
			putchar(' ');
		}
		putchar('\n');
	}
	return;
}

void printMazeToFile(MazeCell **maze, MazeDimensions dimensions) {
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
			case IN:
				fputc('i', outFile);
				break;
			case OUT:
				fputc('o', outFile);
				break;
			default:
				fputc(' ', outFile);
				break;
			}
			fputc(' ', outFile);
		}
		fputc('\n', outFile);
	}
	fclose(outFile);
	return;
}


void deallocateMaze(MazeCell **maze, int height) {
	int i;
	for (i = 0; i < height; i++) {
		free(maze[i]);
	}
	free(maze);
	return;
}

MazeCell **allocateMaze(MazeCell **maze, MazeDimensions dimensions) {
	int i;
	int flag = 0;

	maze = calloc(dimensions.i, sizeof(MazeCell *));

	// Memory allocation check
	if (maze == NULL) {
		return NULL;
	}

	for (i = 0; i < dimensions.i; i++) {
		maze[i] = calloc(dimensions.j, sizeof(MazeCell));
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

void initMaze(MazeCell **maze, MazeDimensions dimensions) {
	int i, j;
	for (i = 0; i < unhashCoordinates(dimensions.i); i++) {
		for (j = 0; j < unhashCoordinates(dimensions.j); j++) {
			maze[hashCoordinates(i)][hashCoordinates(j)] = PATH;
		}
	}
	return;
}


int checkCoordinates(MazeDimensions dimensions, Coordinates coordinates) {
	if (dimensions.i <= coordinates.i || dimensions.j <= coordinates.j || coordinates.i < 0 || coordinates.j < 0)
		return 0;
	else
		return 1;
}

int checkCoordinatesForEdges(MazeDimensions dimensions, Coordinates coordinates) {
	if (checkCoordinates(dimensions, coordinates) && (coordinates.i + coordinates.j) % 2 == 1 && coordinates.i != 0 && coordinates.j != 0) {
		return 1;
	}
	else {
		return 0;
	}
}


int checkAllCoordinates(MazeDimensions dimensions, Coordinates in, Coordinates exits[], int numberOfExits) {
	int i;
	int flag = 1;
	flag = checkCoordinates(dimensions, in);
	for (i = 0; i < numberOfExits; i++) {
		flag = checkCoordinates(dimensions, exits[i]);
	}
	return 1;
}


int makePath(MazeCell **maze, MazeDimensions dimensions, Coordinates coordinates) {
	if (checkCoordinatesForEdges(dimensions, coordinates)) {
		maze[coordinates.i][coordinates.j] = PATH;
		return 1;
	}
	else {
		return 0;
	}
}

int makeWall(MazeCell **maze, MazeDimensions dimensions, Coordinates coordinates) {
	if (checkCoordinatesForEdges(dimensions, coordinates)) {
		maze[coordinates.i][coordinates.j] = WALL;
		return 1;
	}
	else {
		return 0;
	}
}

void inputMaze(MazeCell **maze, MazeDimensions dimensions) {
	int flagPath = 1, flagWall = 1;
	Coordinates temp;

	while (flagPath) {
		printf("Input coordinates for which to make path:\n");
		temp = readCoordinatesForEdges();
		flagPath = makePath(maze, dimensions, temp);
	}

	while (flagWall) {
		printf("Input coordinates for which to make wall:\n");
		temp = readCoordinatesForEdges();
		flagWall = makeWall(maze, dimensions, temp);
	}
	return;
}

int main() {
	enum MenuOptions menuOption;
	int numberOfExits = 1;
	MazeCell **maze = NULL;
	int isGameRunning = 1;
	MazeDimensions dimensions;
	Coordinates exits[MAX_EXITS], in;
	int i;


	/*	!!!!!
	*	Koordinate cvorova su na neparnim indeksima
	*	Koordinate grana su na parnim i neparnim indeksima (zbir indeksa im je neparan)
	*	!!!!!
	*/

	exits[0].i = hashCoordinates(0);
	exits[0].j = hashCoordinates(0);
	in.i = hashCoordinates(DEFAULT_HEIGHT - 1);
	in.j = hashCoordinates(DEFAULT_WIDTH - 1);

	dimensions.i = hashCoordinates(DEFAULT_HEIGHT);
	dimensions.j = hashCoordinates(DEFAULT_WIDTH);

	maze = allocateMaze(maze, dimensions);
	initMaze(maze, dimensions);

	while (isGameRunning) {
		printMenu();

		scanf_s("%d", &menuOption);
		
		switch (menuOption) {

		case ExitGame:
			deallocateMaze(maze, dimensions.i);
			isGameRunning = 0;
			break;

		case GenerateMaze:
			if (maze != NULL && checkAllCoordinates(dimensions, in, exits, numberOfExits)) {

				// TODO: Primov algoritam za generisanje lavirinta

				// TODO: stavi gde su ulazi i izlazi

				if (dimensions.i < MAX_HEIGHT && dimensions.j < MAX_WIDTH) {
					printMazeToStdout(maze, dimensions);
				}

				printMazeToFile(maze, dimensions);
			}
			else {
				printf("There is no maze or coordinates for in or exits are out of bounds\n");
			}
			break;

		case InputMaze:
			if (maze != NULL && checkAllCoordinates(dimensions, in, exits, numberOfExits)) {
				inputMaze(maze, dimensions);
			}
			else {
				printf("There is no maze or coordinates for in or exits are out of bounds\n");
			}
			break;

		case SolveMaze:
			if (maze != NULL && checkAllCoordinates(dimensions, in, exits, numberOfExits)) {
				// TODO: BFS za resavanje lavirinta
				// TODO: ispis niza koraka za resavanje
			}
			else {
				printf("There is no maze or coordinates for in or exits are out of bounds\n");
			}
			break;

		case SetNumberOfExits:
			numberOfExits = setNumberOfExits(numberOfExits);
			// TODO: random-izuj ostale exit-e
			break;

		case SetCoordinatesForExitsAndIn:
			printf("Coordinates for in:\n");
			in = setCoordinates(in, dimensions);
			for (i = 0; i < numberOfExits; i++) {
				printf("Coordinates for exit[%d]:\n", i);
				exits[i] = setCoordinates(exits[i], dimensions);
			}
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
