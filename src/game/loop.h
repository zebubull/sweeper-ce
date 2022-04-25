#ifndef _LOOP_H
#define _LOOP_H
#include "board.h"
#include <graphx.h>


#define HIDDEN_TILE 0
#define MINE_TILE 10
#define FLAG_TILE 11

#define TIMER_FREQ 32768

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t, int* h);
void Lose(Board* b, gfx_tilemap_t* t, int flags, int seconds);
void Cleanup(Board* b, gfx_tilemap_t* t, bool first);
void DrawUI(int flags, int seconds);
void PrintCenter(const char* msg);
int loop(int width, int height);

#endif