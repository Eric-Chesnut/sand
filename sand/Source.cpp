// use map library to map board values (i.e. 0 empty, 1 sand, 2 water, -1 border, etc.) to SDL colors to make pixels easier to manage


//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stack>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//copies the tempBoard to the main board
void copyBoard();

//sets the position on the main board
void setBoard(int x, int y, int val);

//gets the position on the main board
int getBoard(int x, int y);

//same but for tempBoard
void setTempBoard(int x, int y, int val);

//same but for tempBoard
int getTempBoard(int x, int y);

//creates a ring of -1's around the array 
void setBoarder();

//same but for tempBoard
void setTempBoarder();

//empties and resets the tempBoard
void emptyTempBoard();

//draws the screen
void drawScreen();

//run sand simulation
void sandSim(int x, int y);

//runs the simulation
void runSim();


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;



//the board, initialized to array full of 0's
int board[SCREEN_HEIGHT*SCREEN_WIDTH + SCREEN_WIDTH + 1] = { {0} };
int tempBoard[SCREEN_HEIGHT * SCREEN_WIDTH + SCREEN_WIDTH + 1] = { {0} }; 

//holds the values to be rerun
//stack<int> redo;

//run sand simulation
void sandSim(int x, int y)
{
	int direction;
	int swaps = rand() % 2;
	if (swaps == 0)
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	
	if (getBoard(x, y + 1) == 0 || getBoard(x, y + 1) == 2) //water or empty spot below
	{
		if (getTempBoard(x, y + 1) == 0 or getTempBoard(x, y + 1) == 2)//check that spot is still empty
		{
			setTempBoard(x, y, getTempBoard(x, y + 1));//swap spots
			setTempBoard(x, y + 1, 1);
			return;
		}
	}
	if (getBoard(x - direction, y + 1) == 0 || getBoard(x - direction, y + 1) == 2) //first down to the side spot
	{
		if (getTempBoard(x - direction, y + 1) == 0 or getTempBoard(x - direction, y + 1) == 2)//check that spot is still empty
		{
			setTempBoard(x, y, getTempBoard(x - direction, y + 1));//swap spots
			setTempBoard(x - direction, y + 1, 1);
			return;
		}
	}
	if (getBoard(x + direction, y + 1) == 0 || getBoard(x + direction, y + 1) == 2) //second down to the side spot
	{
		if (getTempBoard(x + direction, y + 1) == 0 or getTempBoard(x + direction, y + 1) == 2)//check that spot is still empty
		{
			setTempBoard(x, y, getTempBoard(x + direction, y + 1));//swap spots
			setTempBoard(x + direction, y + 1, 1);
			return;
		}
	}
	//sand not moving
	setTempBoard(x, y, 1);
	return;
}

//runs the simulation
void runSim()
{
	for (int y = SCREEN_HEIGHT - 2; y > 1; y--)
	{
		for (int x = 2; x < SCREEN_WIDTH-1; x++)
		{
			if (getBoard(x, y) == 1)
			{
				sandSim(x, y);
			}
		}
	}
	copyBoard();
	emptyTempBoard();
}


void setBoard(int x, int y, int val)
{
	if (val > -2 && val < 3)
	{
		board[x + y * SCREEN_WIDTH] = val;
	}
	else
	{
		cout << "Invalid value" << endl;
	}
}

int getBoard(int x, int y)
{
	if (x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT)
	{
		return board[x + y * SCREEN_WIDTH];
	}
	else
	{
		cout << "Position out of range" << endl;
	}
}

void setTempBoard(int x, int y, int val)
{
	if (val > -2 && val < 3)
	{
		tempBoard[x + y * SCREEN_WIDTH] = val;
	}
	else
	{
		cout << "Invalid value" << endl;
	}
}

int getTempBoard(int x, int y)
{
	if (x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT)
	{
		return tempBoard[x + y * SCREEN_WIDTH];
	}
	else
	{
		cout << "Position out of range" << endl;
	}
}

//creates a ring of -1's around the array 
void setBoarder()
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		setBoard(0, y, -1);
		setBoard(1, y, -1);
		setBoard(SCREEN_WIDTH, y, -1);
		setBoard(SCREEN_WIDTH - 1, y, -1);
	}
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		setBoard(x, 0, -1);
		setBoard(x, 1, -1);
		setBoard(x, SCREEN_HEIGHT, -1);
		setBoard(x, SCREEN_HEIGHT - 1, -1);
	}
}

void setTempBoarder()
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		setTempBoard(0, y, -1);
		setTempBoard(1, y, -1);
		setTempBoard(SCREEN_WIDTH, y, -1);
		setTempBoard(SCREEN_WIDTH - 1, y, -1);
	}
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		setTempBoard(x, 0, -1);
		setTempBoard(x, 1, -1);
		setTempBoard(x, SCREEN_HEIGHT, -1);
		setTempBoard(x, SCREEN_HEIGHT - 1, -1);
	}
}

//empties and resets the tempBoard
void emptyTempBoard()
{
	memset(tempBoard, 0, sizeof(tempBoard));
	setTempBoarder();
}

//copies the tempBoard over to the main board
void copyBoard()
{
	for (int i = 0; i < size(board); i++)
	{
		board[i] = tempBoard[i];
	}
}

//draws the screen
void drawScreen()
{//empties board
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			switch (getBoard(x, y)) {
			case 1:
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
				SDL_RenderDrawPoint(gRenderer, x, y);
				break;
			case 2:
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 0xFF);
				SDL_RenderDrawPoint(gRenderer, x, y);
				break;
			case -1:
				SDL_SetRenderDrawColor(gRenderer, 194, 178, 128, 0xFF);
				SDL_RenderDrawPoint(gRenderer, x, y);
				break;
			}
		}
	}
	SDL_RenderPresent(gRenderer);
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


//garbage collection
void close()
{
	

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}
	setBoarder();
	emptyTempBoard();
	/*
	cout << board[0] << " and " << board[1] << endl;
	setBoard(0, 0, 1);
	cout << board[0] << " and " << board[1] << endl;
	emptyTempBoard();
	cout << tempBoard[0] << " " << getTempBoard(4, 5) << endl;
	setTempBoard(4, 5, 6);
	copyBoard();
	emptyTempBoard();
	cout << getBoard(4, 5) << " " << getTempBoard(4, 5) << endl;
	setTempBoard(4, 5, 1);
	setTempBoard(5, 5, 1);
	setTempBoard(6, 5, 1);
	setTempBoard(7, 5, 1);
	setTempBoard(8, 5, 1);
	setTempBoard(4, 7, 1);
	setTempBoard(5, 7, 1);
	setTempBoard(6, 7, 1);
	setTempBoard(4, 6, 1);
	setTempBoard(5, 6, 1);
	setTempBoard(6, 6, 1);
	setTempBoard(7, 6, 1);
	setTempBoard(8, 6, 1);
	copyBoard();
	emptyTempBoard();
	cout << getBoard(4, 5) << " " << getTempBoard(4, 5) << endl;
	drawScreen();
	*/
	bool quit = false;
	SDL_Event e;
	setTempBoard(4, 5, 1);
	setTempBoard(5, 5, 1);
	setTempBoard(6, 5, 1);
	setTempBoard(7, 5, 1);
	setTempBoard(8, 5, 1);
	setTempBoard(4, 7, 1);
	setTempBoard(5, 7, 1);
	setTempBoard(6, 7, 1);
	setTempBoard(4, 6, 1);
	setTempBoard(5, 6, 1);
	setTempBoard(6, 6, 1);
	setTempBoard(7, 6, 1);
	setTempBoard(8, 6, 1);
	setBoard(8, 6, 0);
	cout << getBoard(8, 6) << " " << getTempBoard(8, 6);
	copyBoard();
	emptyTempBoard();
	drawScreen();


	//int donee = 100;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		runSim();
		drawScreen();
		//if(donee > 0)
		//{ 
			setTempBoard(SCREEN_WIDTH / 2, 2, 1);
			//donee--;
			
		//}
		
	}
	close();
	return 0;
}
