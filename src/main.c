#include <tice.h>
#include <graphx.h>
#include "game/board.h"

int main(void)
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    Board* board = GenerateBoard(12, 20, 25);



    gfx_End();
    FreeBoard(board);
    return 0;
}