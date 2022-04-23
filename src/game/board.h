#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>

typedef struct
{
    int width;
    int height;
    int mines;
    char** data;
} Board;

Board* GenerateBoard(int w, int h, int mines, int x, int y); 
void FreeBoard(Board* b);
bool Cleared(Board* b, int x, int y);
void Clear(Board* b, int x, int y);
int NearMines(Board* b, int x, int y);
bool IsMine(Board* b, int x, int y);
bool IsFlagged(Board* b , int x, int y);
void ToggleFlag(Board* b, int x, int y);

#endif