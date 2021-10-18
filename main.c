/*
    Created by Ilya Tsykunov (2021).
    https://github.com/ilyatsykunov/2D-Terminalator
*/

#include "main.h"

int main(int argc, int **argv)
{
    instGame();

    // Game carries on until the player runs into an obstacle
    while (moveObstacles() >= 0)
    {
        // If no input is received
        if (!kbhit())
        {
            playerFall();
            draw();
            wait(30);
        }
        // If input is received
        else
        {
            // Consume the character to resume the game
            unsigned char c;
            read(0, &c, sizeof(c));

            beginPlayerJump();
        }

        // Clear console
        printf("\e[1;1H\e[2J");
    }

    endGame();
    return 0;
}

void wait(uint msec)
{
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    
    nanosleep(&ts, &ts);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void instGame()
{
    srand(time(NULL)); 

    struct position playerPos = {7, 0};
    struct character* playerTemp = malloc(sizeof(struct character));
    playerTemp->pos = playerPos;
    playerTemp->width = 12;
    playerTemp->height = 4;

    player = playerTemp;

    instObstacles();
    loadMaps();

    // Clear console
    printf("\e[1;1H\e[2J");
}

void instObstacles()
{
    obstacles = malloc(sizeof(struct character) * MAX_OBSTACLES);

    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        struct position pos = {MAP_WIDTH + (rand() % player->width * 2) + (i * player->width * 3), 0};
        struct character enemy = {pos, rand() % 4 + 1, rand() % 3 + 1};
        obstacles[i] = enemy;
    }

    lastObstacleIdx = MAX_OBSTACLES - 1;
}

int moveObstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i].pos.x--;

        if (obstacles[i].pos.x < player->pos.x + player->width && 
            obstacles[i].pos.x > player->pos.x + player->width / 3 &&
            player->pos.y < obstacles[i].pos.y + obstacles[i].height)
        {
            return -1;
        }

        if (obstacles[i].pos.x <= 0)
        {
            obstacles[i].pos.x = obstacles[lastObstacleIdx].pos.x + (rand() % player->width * 2) + (i * player->width * 3);
            obstacles[i].width = rand() % 4 + 1;
            obstacles[i].height = rand() % 3 + 1;
            lastObstacleIdx = i;
        }
    }

    return 0;
}

void beginPlayerJump()
{
    player->pos.y = 1;
    ticksSinceJump = 0;
}

void playerFall()
{
    if (ticksSinceJump < MAX_JUMP_LENGTH / 3)
    {
        player->pos.y = ticksSinceJump;
    }
    else if (ticksSinceJump > MAX_JUMP_LENGTH / 3 * 2)
    {
        player->pos.y = MAX_JUMP_LENGTH - ticksSinceJump;
    }

    if (ticksSinceJump < MAX_JUMP_LENGTH)
        ticksSinceJump++;
}

void draw()
{
    int playerCounter = 0;

    for (int y = MAP_HEIGHT - 1; y >= 0; y--)
    {
        for(int x = 0; x < MAP_WIDTH; x++)
        {
            int skipCell = 0;

            switch (x)
            {
            // Print world boundaries
            case 0:
                printf("|");
                break;
            case MAP_WIDTH - 1:
                printf("|");
                break;
            default:
                // Print enemies
                for (int i = 0; i < MAX_OBSTACLES; i++)
                {
                    if (x == obstacles[i].pos.x && y <= obstacles[i].pos.y + obstacles[i].height - 1)
                    {
                        printf("#");
                        skipCell = 1;
                        break;
                    }
                }
                if (skipCell)
                {
                    continue;
                }
                // Print player
                if (x >= player->pos.x && x < player->pos.x + player->width && y >= player->pos.y && y < player->pos.y + player->height)
                {
                    printf("%c", playerMap[playerCounter]);
                    playerCounter++;
                }
                // Print environment
                else
                {
                    switch (y)
                    {
                        case 0:
                            printf(".");
                        break;
                        default:
                            printf(" ");
                        break;
                    }
                }
            break;
            }
        }

        printf("\n");
    }
}

void loadMaps()
{
    FILE* fp;
    char* playerMapTemp = malloc(sizeof(char) * player->width * player->height);
    fp = fopen("maps/player.txt", "r");
    char ch;
    int counter = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != '\n')
        {
            playerMapTemp[counter] = ch;
            counter++;
        }
    }

    playerMap = playerMapTemp;
}

void endGame()
{
    free(playerMap);
    free(obstacles);
}