#include "board.h"
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>

Board* GenerateBoard(int w, int h, int mines)
{
    Board* b = calloc(1, sizeof(Board));
    b->width = w;
    b->height = h;
    b->mines = mines;

    // Create data array
    b->data = calloc(h, sizeof(unsigned char*));
    for (int i = 0; i < h; i++)
    {
        b->data[i] = calloc(w, sizeof(unsigned char));
    }

    for (int i = 0; i < mines; i++)
    {
        int x = randInt(0, w-1);
        int y = randInt(0, h-1);
        while (b->data[y][x])
        {
            x = randInt(0, w-1);
            y = randInt(0, h-1);
        }
        b->data[y][x] = 1;
    }

    return b;
}

void FreeBoard(Board* b)
{
    for (int i = 0; i < b->height; i++)
    {
        free(b->data[i]);
    }
    free(b->data);
    return;
}

bool Cleared(Board* b, int x, int y)
{
    return b->data[y][x] == 2;
}

void Clear(Board* b, int x, int y)
{
    b->data[y][x] = 2;
}

#define MIN(A, B) (A < B) * A + (B <= A) * B
#define MAX(A, B) (A > B) * A + (B >= A) * B

int NearMines(Board* b, int x, int y)
{
    int mines = 0;
    for (int i = MAX(y - 1, 0); i <= MIN(y + 1, b->height); i++)
    {
        for (int j = MAX(x - 1, 0); j <= MIN(x + 1, b->width); j++)
        {
            if (b->data[i][j] == 1) mines++;
        }
    } 
   return mines;
}
