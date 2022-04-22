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

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t);


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

        if (kb_RisingEdge(okb_Enter, kb_Enter) && !Cleared(board, px, py))
        {
            Reveal(board, px, py, tilemap);
        }

        gfx_FillScreen(0x02);
        gfx_Tilemap(tilemap, 0, 0);
        gfx_SetColor(0x01);
        gfx_Rectangle(px * 12 + gfx_lcdWidth / 2 - TILE_WIDTH * BOARD_WIDTH / 2, py * 12, 12, 12);
        gfx_SwapDraw();
    }
    while (!kb_FallingEdge(okb_Del, kb_Del));

    gfx_End();
    FreeBoard(board);
    FreeTilemap(tilemap);
    return 0;
}

#define MIN(A, B) (A < B) * A + (B <= A) * B
#define MAX(A, B) (A > B) * A + (B >= A) * B

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t)
{
    if (x < 0 || y < 0 || x >= b->width || y >= b->height) return;
    if (Cleared(b, x, y)) return;
    Clear(b, x, y);
    int nearby = NearMines(b, x, y);
    gfx_SetTileMapped(t, x, y, (unsigned char)nearby + 1);
    if (nearby == 0)
    {
        for (int i = MAX(y - 1, 0); i <= MIN(y + 1, b->height); i++)
        {
            for (int j = MAX(x - 1, 0); j < MIN(x + 1, b->width); j++)
            {
                Reveal(b, j, i, t);
            }
        } 
    }
}
