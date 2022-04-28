#include "board.h"
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>

#define MIN(A, B) (A < B) * A + (B <= A) * B
#define MAX(A, B) (A > B) * A + (B >= A) * B

void SeedRandom()
{
    unsigned char sec, min, hour;
    boot_GetTime(&sec, &min, &hour);
    unsigned int seed = 0;
    unsigned char* seedp = (unsigned char*)&seed;
    *seedp++ = sec;
    *seedp++ = min;
    *seedp = hour;
    seedp += random();
    srandom(seed);
}

bool IsValid(int x, int y, int mx, int my, int w, int h)
{
    // what the hell is this
    return !(mx >= MAX(x-1, 0) && mx <= MIN(x+1, w) && my >= MAX(y-1, 0) && my <= MIN(y+1, h));
}

Board* GenerateBoard(int w, int h, int mines, int x, int y)
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

    SeedRandom();

    for (int i = 0; i < mines; i++)
    {
        int mx = randInt(0, w-1);
        int my = randInt(0, h-1);
        while (!IsValid(x, y, mx, my, w, h) || IsMine(b, mx, my))
        {
            mx = randInt(0, w-1);
            my = randInt(0, h-1);
        }
        b->data[my][mx] = 1;
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


int NearMines(Board* b, int x, int y)
{
    int mines = 0;
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (i == y && x == j) continue;
            mines += IsMine(b, j, i);
        }
    }
    return mines;
}

bool IsMine(Board* b, int x, int y)
{
    if (x < 0 || y < 0 || x >= b->width || y >= b->height) return false;
    return b->data[y][x] == 1 || b->data[y][x] == 4;
}

bool IsFlagged(Board* b, int x, int y)
{
    return b->data[y][x] > 2;
}

void ToggleFlag(Board* b, int x, int y)
{
    if (b->data[y][x] > 2) b->data[y][x] -= 3;
    else b->data[y][x] += 3;
}

bool IsCleared(Board* b)
{
    for (int y = 0; y < b->height; y++)
    {
        for (int x = 0; x < b->width; x++)
        {
            if (b->data[y][x] == 0 || b->data[y][x] == 3) return false;
        }
    }
    return true;
}
