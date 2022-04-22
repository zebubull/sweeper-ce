#ifndef _BOARD_H
#define _BOARD_H

typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned int mines;
    unsigned char** data;
} Board;

Board* GenerateBoard(unsigned int w, unsigned int h, unsigned int mines); 
void FreeBoard(Board* b);

#endif