#include <tice.h>
#include <graphx.h>
#include "board.h"

int main(void)
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    Board* board = GenerateBoard(10, 10, 5);
    gfx_FillScreen(0xFF);
    gfx_SetColor(0x00);
    DisplayBoard(board);
    gfx_BlitBuffer();
    FreeBoard(board);

    while (!os_GetKey()) {}
    gfx_End();
    return 0;
}