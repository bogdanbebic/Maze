#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HEIGHT 80
#define MAX_WIDTH 50

#define MAX_MAZE_DIMENSIONS 100

#define DEFAULT_HEIGHT 5
#define DEFAULT_WIDTH 5

#define MAX_EXITS 3

#define OUT_FILE_NAME ("outfile.txt")


#define hashCoordinates(x) ((x) * 2 + 1)
#define unhashCoordinates(x) (((x) - 1) / 2)


enum MazeObjects { WALL, IN, OUT, PATH };

enum MenuOptions { ExitGame, GenerateMaze, InputMaze, SolveMaze, SetNumberOfExits, SetCoordinatesForExitsAndIn, SetMazeDimensions, NumberOfMenuOptions };

enum MazeDirections { UP, RIGHT, DOWN, LEFT , NUMBER_OF_DIRECTIONS };


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

#pragma region RANDOM_FUNCTIONS
int randomInt(int low, int high) {
	double x;
	x = (double)rand() / (RAND_MAX + 1);
	x = low + (high - low) * x;
	return (int)x;
}

Coordinates getRandomNode(MazeDimensions dimensions) {
	Coordinates randomNode;
	randomNode.i = randomInt(0, unhashCoordinates(dimensions.i));
	randomNode.j = randomInt(0, unhashCoordinates(dimensions.j));
	return randomNode;
}

Coordinates getRandomWall(MazeCell **maze, Coordinates node) {
	enum MazeDirections direction;
	Coordinates randomWall, temp;
	enum MazeDirections choice[NUMBER_OF_DIRECTIONS];
	int walls = 0;
	randomWall.i = hashCoordinates(node.i);
	randomWall.j = hashCoordinates(node.j);
	temp = randomWall;
	temp.i--;
	if (maze[temp.i][temp.j] == WALL) {
		choice[walls] = UP;
		walls++;
	}
	temp = randomWall;
	temp.i++;
	if (maze[temp.i][temp.j] == WALL) {
		choice[walls] = DOWN;
		walls++;
	}
	temp = randomWall;
	temp.j--;
	if (maze[temp.i][temp.j] == WALL) {
		choice[walls] = LEFT;
		walls++;
	}
	temp = randomWall;
	temp.j++;
	if (maze[temp.i][temp.j] == WALL) {
		choice[walls] = RIGHT;
		walls++;
	}
	direction = choice[randomInt(0, walls)];
	
	switch (direction) {
	case UP:
		randomWall.i--;
		break;
	case RIGHT:
		randomWall.j++;
		break;
	case DOWN:
		randomWall.i++;
		break;
	case LEFT:
		randomWall.j--;
		break;
	default:
		break;
	}
	

	return randomWall;
}
#pragma endregion

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
	return temp > 0 ? min(temp, MAX_EXITS) : oldNumberOfExits;
}

#pragma region MAZE_COORDINATES_SETTING_FUNCTIONS
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
#pragma endregion

#pragma region MAZE_OUTPUT
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
#pragma endregion

#pragma region MAZE_INIT
void deallocateMaze(MazeCell **maze, int height) {
	int i;
	if (maze != NULL) {
		for (i = 0; i < height; i++) {
			free(maze[i]);
		}
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
#pragma endregion

#pragma region COORDINATES_CHECK
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
	return flag;
}
#pragma endregion

#pragma region MAZE_CELL_FUNCTIONS
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
#pragma endregion

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

#define hashCoordinatesToNumber(xi, xj) ((xi) * MAX_MAZE_DIMENSIONS + (xj))
#define unhashICoordinateFromNumber(x) ((x) / MAX_MAZE_DIMENSIONS)
#define unhashJCoordinateFromNumber(x) ((x) % MAX_MAZE_DIMENSIONS)

#pragma region SET_AND_NODE_FUNCTIONS_FOR_PRIM
int isNodeInSet(MazeCell *set, int numberOfNodesInSet, Coordinates node) {
	int i;
	MazeCell temp;
	temp = hashCoordinatesToNumber(node.i, node.j);
	for (i = 0; i < numberOfNodesInSet; i++) {
		if (temp == set[i])
			return 1;
	}
	return 0;
}

void insertNodeToSet(MazeCell *set, int *numberOfNodesInSet, Coordinates node) {
	if (!isNodeInSet(set, *numberOfNodesInSet, node)) {
		set[(*numberOfNodesInSet)++] = hashCoordinatesToNumber(node.i, node.j);
	}
	return;
}

void deleteNodeFromSet(MazeCell *set, int *numberOfNodesInSet, Coordinates node) {
	int i;
	MazeCell temp, thirdLocation;
	temp = hashCoordinatesToNumber(node.i, node.j);
	for (i = 0; i < *numberOfNodesInSet; i++) {
		if (temp == set[i]) {
			thirdLocation = set[*numberOfNodesInSet - 1];
			set[*numberOfNodesInSet - 1] = temp;
			set[i] = thirdLocation;
			(*numberOfNodesInSet)--;
			break;
		}
	}
	return;
}

Coordinates getRandomNodeFromSet(MazeCell *set, int numberOfNodesInSet) {
	int i;
	Coordinates randomNodeFromSet;
	i = randomInt(0, numberOfNodesInSet);
	randomNodeFromSet.i = unhashICoordinateFromNumber(set[i]);
	randomNodeFromSet.j = unhashJCoordinateFromNumber(set[i]);
	return randomNodeFromSet;
}

Coordinates getAdjecentNode(Coordinates node, Coordinates wall, MazeDimensions dimensions) {
	Coordinates adjecentNode = node;
	if (hashCoordinates(node.i) < wall.i) {
		adjecentNode.i++;
	}
	else if (hashCoordinates(node.i) > wall.i) {
		adjecentNode.i--;
	}
	if (hashCoordinates(node.j) < wall.j) {
		adjecentNode.j++;
	}
	else if (hashCoordinates(node.j) > wall.j) {
		adjecentNode.j--;
	}
	return adjecentNode;
}

int isAdjecentNodeInSet(MazeCell *set, int numberOfNodesInSet, Coordinates node, Coordinates wall, MazeDimensions dimensions) {
	Coordinates adjecentNode = node;
	if (wall.i == 0 || wall.j == 0 || wall.i == dimensions.i - 1 || wall.j == dimensions.j - 1) {
		return 1;
	}
	adjecentNode = getAdjecentNode(node, wall, dimensions);
	return isNodeInSet(set, numberOfNodesInSet, adjecentNode);
}

int isNodeSurrounded(MazeCell *set, int numberOfNodesInSet, Coordinates node, MazeDimensions dimensions) {
	int i = 0;
	Coordinates wall;
	wall.i = hashCoordinates(node.i) + 1;
	wall.j = hashCoordinates(node.j);
	i += isAdjecentNodeInSet(set, numberOfNodesInSet, node, wall, dimensions);
	wall.i = hashCoordinates(node.i) - 1;
	wall.j = hashCoordinates(node.j);
	i += isAdjecentNodeInSet(set, numberOfNodesInSet, node, wall, dimensions);
	wall.i = hashCoordinates(node.i);
	wall.j = hashCoordinates(node.j) + 1;
	i += isAdjecentNodeInSet(set, numberOfNodesInSet, node, wall, dimensions);
	wall.i = hashCoordinates(node.i);
	wall.j = hashCoordinates(node.j) - 1;
	i += isAdjecentNodeInSet(set, numberOfNodesInSet, node, wall, dimensions);
	return (i == 4);
}
#pragma endregion

void mazeGenerationPrim(MazeCell **maze, MazeDimensions dimensions) {
	Coordinates tempNode, adjecentNode, tempWall;
	int numberOfNodesInSet = 0, numberOfNodesInSet1 = 0;
	int flag;
	int i;
	MazeCell set[MAX_MAZE_DIMENSIONS * MAX_MAZE_DIMENSIONS], set1[MAX_MAZE_DIMENSIONS * MAX_MAZE_DIMENSIONS];

	tempNode = getRandomNode(dimensions);
	insertNodeToSet(set, &numberOfNodesInSet, tempNode);
	insertNodeToSet(set1, &numberOfNodesInSet1, tempNode);

	while (numberOfNodesInSet > 0) {
		tempNode = getRandomNodeFromSet(set, numberOfNodesInSet);
		tempWall = getRandomWall(maze, tempNode);
		flag = isAdjecentNodeInSet(set1, numberOfNodesInSet1, tempNode, tempWall, dimensions);
		if (flag == 0) {
			adjecentNode = getAdjecentNode(tempNode, tempWall, dimensions);
			makePath(maze, dimensions, tempWall);
			insertNodeToSet(set, &numberOfNodesInSet, adjecentNode);
			insertNodeToSet(set1, &numberOfNodesInSet1, adjecentNode);
		}
		for (i = 0; i < numberOfNodesInSet; i++) {
			tempNode.i = unhashICoordinateFromNumber(set[i]);
			tempNode.j = unhashJCoordinateFromNumber(set[i]);
			if (isNodeSurrounded(set1, numberOfNodesInSet1, tempNode, dimensions)) {
				deleteNodeFromSet(set, &numberOfNodesInSet, tempNode);
			}
		}
	}
	return;
}

void setMazeInAndExits(MazeCell **maze, MazeDimensions dimensions, Coordinates in, Coordinates exits[], int numberOfExits) {
	int i;
	if (!checkCoordinates(dimensions, in)) {
		in.i = hashCoordinates(unhashCoordinates(dimensions.i) - 1);
		in.j = hashCoordinates(unhashCoordinates(dimensions.j) - 1);
	}
	maze[in.i][in.j] = IN;

	for (i = 0; i < numberOfExits; i++) {
		if (!checkCoordinates(dimensions, exits[i])) {
			switch (i) {
			case 0:
				exits[i].i = hashCoordinates(0);
				exits[i].j = hashCoordinates(0);
				break;
			case 1:
				exits[i].i = hashCoordinates(unhashCoordinates(dimensions.i) - 1);
				exits[i].j = hashCoordinates(0);
				break;
			case 2:
				exits[i].i = hashCoordinates(0);
				exits[i].j = hashCoordinates(unhashCoordinates(dimensions.j) - 1);
				break;
			}
		}
		maze[exits[i].i][exits[i].j] = OUT;
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
	int temp;

	/*
	*	!!!!!
	*	Koordinate cvorova su na neparnim indeksima
	*	Koordinate grana su na parnim i neparnim indeksima (zbir indeksa im je neparan)
	*	!!!!!
	*/

	exits[0].i = hashCoordinates(0);
	exits[0].j = hashCoordinates(0);
	for (i = 1; i < MAX_EXITS; i++) {
		exits[i].i = -1;
		exits[i].j = -1;
	}
	in.i = hashCoordinates(DEFAULT_HEIGHT - 1);
	in.j = hashCoordinates(DEFAULT_WIDTH - 1);

	dimensions.i = hashCoordinates(DEFAULT_HEIGHT);
	dimensions.j = hashCoordinates(DEFAULT_WIDTH);

	maze = allocateMaze(maze, dimensions);
	initMaze(maze, dimensions);

	srand((unsigned int)time(NULL));

	while (isGameRunning) {
		printMenu();

		scanf_s("%d", &menuOption);
		
		switch (menuOption) {

		case ExitGame:
			deallocateMaze(maze, dimensions.i);
			isGameRunning = 0;
			break;

		case GenerateMaze:
			if (maze != NULL) {
				deallocateMaze(maze, dimensions.i);
				maze = allocateMaze(maze, dimensions);
				initMaze(maze, dimensions);

				mazeGenerationPrim(maze, dimensions);	// TODO: implement

				setMazeInAndExits(maze, dimensions, in, exits, numberOfExits);

				if (dimensions.i < MAX_HEIGHT && dimensions.j < MAX_WIDTH) {
					printMazeToStdout(maze, dimensions);
				}
				printMazeToFile(maze, dimensions);

			}
			else {
				printf("There is no maze\n");
			}
			break;

		case InputMaze:
			if (maze != NULL) {
				deallocateMaze(maze, dimensions.i);
				maze = allocateMaze(maze, dimensions);
				initMaze(maze, dimensions);

				inputMaze(maze, dimensions);

				setMazeInAndExits(maze, dimensions, in, exits, numberOfExits);

				if (dimensions.i < MAX_HEIGHT && dimensions.j < MAX_WIDTH) {
					printMazeToStdout(maze, dimensions);
				}
				printMazeToFile(maze, dimensions);

			}
			else {
				printf("There is no maze\n");
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
			temp = setNumberOfExits(numberOfExits);
			if (temp < numberOfExits) {
				for (i = temp; i < MAX_EXITS; i++) {
					if (checkCoordinates(dimensions, exits[i]))
						maze[exits[i].i][exits[i].j] = PATH;
					exits[i].i = -1;
					exits[i].j = -1;
				}
			}
			numberOfExits = temp;
			break;

		case SetCoordinatesForExitsAndIn:
			maze[in.i][in.j] = PATH;
			printf("Coordinates for in:\n");
			in = setCoordinates(in, dimensions);
			for (i = 0; i < numberOfExits; i++) {
				if (checkCoordinates(dimensions, exits[i]))
					maze[exits[i].i][exits[i].j] = PATH;
				printf("Coordinates for exit[%d]:\n", i);
				exits[i] = setCoordinates(exits[i], dimensions);
			}
			break;

		case SetMazeDimensions:
			deallocateMaze(maze, dimensions.i);
			dimensions = setMazeDimensions(dimensions);
			maze = allocateMaze(maze, dimensions);
			if (maze != NULL) {
				initMaze(maze, dimensions);
			}
			else {
				printf("Unsuccessful allocation\n");
			}
			break;

		default:
			break;
		}

		putchar('\n');
	}

	return 0;
}
