#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "game/board.h"
#include "game/graphics.h"
#include "gfx/gfx.h"
#include "io/input.h"

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 20
#define NUM_MINES 25


int main(void)
{
    Board* board = GenerateBoard(BOARD_WIDTH, BOARD_HEIGHT, NUM_MINES);

    gfx_tilemap_t* tilemap = CreateTilemap(BOARD_WIDTH, BOARD_HEIGHT);

    unsigned int px = BOARD_WIDTH / 2;
    unsigned int py = BOARD_HEIGHT / 2;

    SetupGraphics();

    do
    {
        ReadKeypad();

        if (kb_RisingEdge(okb_Left, kb_Left) && px > 0) px--;
        if (kb_RisingEdge(okb_Right, kb_Right) && px < BOARD_WIDTH - 1) px++;
        if (kb_RisingEdge(okb_Up, kb_Up) && py > 0) py--;
        if (kb_RisingEdge(okb_Down, kb_Down) && py < BOARD_HEIGHT - 1) py++;


        gfx_FillScreen(0x02);
        gfx_Tilemap(tilemap, 0, 0);
        gfx_SetColor(0x01);
        gfx_Rectangle(px * 12 + 100, py * 12, 12, 12);
        gfx_SwapDraw();
    }
    while (!kb_FallingEdge(okb_Del, kb_Del));

    gfx_End();
    FreeBoard(board);
    FreeTilemap(tilemap);
    return 0;
}