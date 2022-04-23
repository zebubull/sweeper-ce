#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "game/board.h"
#include "game/graphics.h"
#include "gfx/gfx.h"
#include "io/input.h"

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 20
#define NUM_MINES 35

#define HIDDEN_TILE 0
#define MINE_TILE 10
#define FLAG_TILE 11

#define TIMER_FREQ 32768

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t, int* h);
void Lose(Board* b, gfx_tilemap_t* t, int flags, int seconds);
void Cleanup(Board* b, gfx_tilemap_t* t);
void DrawUI(int flags, int seconds);

int main(void)
{
    bool first = true;
    Board* board;
    gfx_tilemap_t* tilemap = CreateTilemap(BOARD_WIDTH, BOARD_HEIGHT);

    unsigned int px = BOARD_WIDTH / 2;
    unsigned int py = BOARD_HEIGHT / 2;
    int hidden = BOARD_HEIGHT * BOARD_WIDTH - NUM_MINES;
    int flags = NUM_MINES;

    SetupGraphics();

    // Set up timer
    int seconds = 0;
    timer_Disable(1);
    timer_Set(1, TIMER_FREQ);
    timer_SetReload(1, TIMER_FREQ);

    do
    {
        ReadKeypad();

        // Read User Input
        if (kb_RisingEdge(okb_Left, kb_Left) && px > 0) px--;
        if (kb_RisingEdge(okb_Right, kb_Right) && px < BOARD_WIDTH - 1) px++;
        if (kb_RisingEdge(okb_Up, kb_Up) && py > 0) py--;
        if (kb_RisingEdge(okb_Down, kb_Down) && py < BOARD_HEIGHT - 1) py++;

        // Reveal Square
        if (kb_RisingEdge(okb_Enter, kb_Enter) && ((!Cleared(board, px, py) && !IsFlagged(board, px, py)) || first))
        {
            if (first)
            {
                board = GenerateBoard(BOARD_WIDTH, BOARD_HEIGHT, NUM_MINES, px, py);
                Reveal(board, px, py, tilemap, &hidden);
                timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
                first = false;
            }

            if (IsMine(board, px, py))
            {
                Lose(board, tilemap, flags, seconds);
            }

            Reveal(board, px, py, tilemap, &hidden);
        }

        // Toggle Flag
        if (kb_RisingEdge(okb_Add, kb_Add) && !Cleared(board, px, py) && !first)
        {
            ToggleFlag(board, px, py);

            if (IsFlagged(board, px, py))
            {
                flags--;
                gfx_SetTileMapped(tilemap, px, py, FLAG_TILE);
            }
            else
            {
                flags++;
                gfx_SetTileMapped(tilemap, px, py, HIDDEN_TILE);
            }
        }

        if (hidden == 0)
        {
            timer_Disable(1);
            gfx_SetTextFGColor(0x01);
            gfx_PrintStringXY("you win!", 2, 114);
            DrawUI(flags, seconds);
            gfx_SwapDraw();
            do
            {
                ReadKeypad();
            }
            while (!kb_RisingEdge(okb_Enter, kb_Enter));
            break;
        }

        // Check timer
        if (timer_ChkInterrupt(1, TIMER_RELOADED))
        {
            seconds++;
            timer_AckInterrupt(1, TIMER_RELOADED);
        }

        // Draw Graphics
        gfx_FillScreen(0x02);
        gfx_Tilemap(tilemap, 0, 0);

        // Selector
        gfx_SetColor(0x01);
        gfx_Rectangle(px * 12 + gfx_lcdWidth / 2 - TILE_WIDTH * BOARD_WIDTH / 2, py * 12, 12, 12);

        DrawUI(flags, seconds);
        
        gfx_SwapDraw();
    }
    while (!kb_FallingEdge(okb_Del, kb_Del));

    Cleanup(board, tilemap);
    return 0;
}

#define MIN(A, B) (A < B) * A + (B <= A) * B
#define MAX(A, B) (A > B) * A + (B >= A) * B

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t, int* h)
{
    // Don't do dumb stuff
    if (x < 0 || y < 0 || x >= b->width || y >= b->height) return;
    if (Cleared(b, x, y)) return;
    // Clear and update values
    Clear(b, x, y);
    *h--;
    int nearby = NearMines(b, x, y);
    gfx_SetTileMapped(t, x, y, (unsigned char)nearby + 1);

    // If it is a zero, clear surrounding squares
    if (nearby == 0)
    {
        for (int i = MAX(y - 1, 0); i <= MIN(y + 1, b->height); i++)
        {
            for (int j = MAX(x - 1, 0); j <= MIN(x + 1, b->width); j++)
            {
                if (IsMine(b, j, i)) continue;
                Reveal(b, j, i, t, h);
            }
        } 
    }
}

void Lose(Board* b, gfx_tilemap_t* t, int flags, int seconds)
{
    // Reveal all the mines
    for (int y = 0; y < b->height; y++)
    {
        for (int x = 0; x < b->width; x++)
        {
            if (IsMine(b, x, y)) gfx_SetTileMapped(t, x, y, 10);
        }
    }
    gfx_FillScreen(0x02);
    gfx_Tilemap(t, 0, 0);

    timer_Disable(1);
    gfx_SetTextFGColor(0x01);
    gfx_PrintStringXY("you lose", 2, 114);
    DrawUI(flags, seconds);
    gfx_SwapDraw();

    do
    {
        ReadKeypad();
    }
    while (!kb_RisingEdge(okb_Enter, kb_Enter));
    Cleanup(b, t);
    exit(0);
    
}

void Cleanup(Board* b, gfx_tilemap_t* t)
{
    gfx_End();
    if (b)
        FreeBoard(b);
    FreeTilemap(t);
}

void DrawUI(int flags, int seconds)
{
    // UI Text
    gfx_SetTextFGColor(0x01);
    gfx_SetTextBGColor(0x02);
    // Total Flags Indicator
    gfx_PrintStringXY("Mines Left:", 2, LCD_HEIGHT - 24);
    gfx_SetTextXY(2, LCD_HEIGHT - 12);
    gfx_PrintInt(flags, 1);
    // Timer
    int mins = seconds / 60;
    int secs = seconds - mins * 60;

    gfx_SetTextXY(2, 2);
    gfx_SetTextFGColor(0x03);
    gfx_PrintInt(mins, 2);
    gfx_PrintChar(':');
    gfx_PrintInt(secs, 2);
}
