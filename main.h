/*
    Created by Ilya Tsykunov (2021).
    https://github.com/ilyatsykunov/2D-Terminalator
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 15
#define MAX_OBSTACLES 5
#define MAX_JUMP_LENGTH 30

struct position 
{
    int x;
    int y;
};

struct character 
{
    struct position pos;
    uint width;
    uint height;
};

struct character* player;
char* playerMap;
int ticksSinceJump = MAX_JUMP_LENGTH;

struct character* obstacles;
int lastObstacleIdx;

int main(int argc, int **argv);
// Freezes the game for a given number of milliseconds
void wait(uint msec);
// Returns true if a keyboard key has been pressed
int kbhit();
// Initial setup of the game
void instGame();
// Spawns MAX_OBSTACLES num of obstacles of random size at random positions
void instObstacles();
// Moves the obstacles towards the player, returns true if an obstacle collides with the player, resets the obstacles that reach end of screen 
int moveObstacles();
// Launches the player in the air
void beginPlayerJump();
// Gradually makes the player go higher then go back on the ground
void playerFall();
// Prints the game in the terminal
void draw();
// Loads bitmaps/textures from text files into array
void loadMaps();
// Frees allocated memory just because
void endGame();