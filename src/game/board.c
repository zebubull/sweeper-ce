#include "board.h"
#include <tice.h>
#include <stdlib.h>
#include <graphx.h>

Board* GenerateBoard(unsigned int w, unsigned int h, unsigned int mines)
{
    Board* b = calloc(1, sizeof(Board));
    b->width = w;
    b->height = h;
    b->mines = mines;

    // Create data array
    b->data = calloc(h, sizeof(unsigned char*));
    for (unsigned int i = 0; i < h; i++)
    {
        b->data[i] = calloc(w, sizeof(unsigned char));
    }

    for (unsigned int i = 0; i < mines; i++)
    {
        unsigned int x = randInt(0, w-1);
        unsigned int y = randInt(0, h-1);
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
    for (unsigned int i = 0; i < b->height; i++)
    {
        free(b->data[i]);
    }
    free(b->data);
    return;
}
