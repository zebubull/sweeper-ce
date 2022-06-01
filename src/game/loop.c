#include "loop.h"
#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "board.h"
#include "graphics.h"
#include "../gfx/gfx.h"
#include "../io/input.h"

int loop(int width, int height)
{
    bool first = true;
    Board* board;
    gfx_tilemap_t* tilemap = CreateTilemap(width, height);

    int px = width / 2;
    int py = height / 2;
    float fmines = (float)(width * height) * 0.15;
    int mines = (int)fmines;
    int minesLeft = mines;


    // Set up timer
    int seconds = 0;
    timer_Disable(1);
    timer_Set(1, TIMER_FREQ);
    timer_SetReload(1, TIMER_FREQ);

    do
    {
        ReadKeypad();

        // Read User Input
        if (kb_FallingEdge(okb_Left, kb_Left) && px > 0) px--;
        if (kb_FallingEdge(okb_Right, kb_Right) && px < width - 1) px++;
        if (kb_FallingEdge(okb_Up, kb_Up) && py > 0) py--;
        if (kb_FallingEdge(okb_Down, kb_Down) && py < height - 1) py++;

        // Check victory before clearing board to make sure all graphics are drawn before the win screen shows
        if (board && IsCleared(board))
        {
            timer_Disable(1);
            gfx_SetTextFGColor(0x01);
            gfx_Tilemap_NoClip(tilemap, 0, 0);
            PrintCenter("you win!");
            DrawUI(minesLeft, seconds);
            gfx_SwapDraw();
            do
            {
                ReadKeypad();
            }
            while (!kb_FallingEdge(okb_Del, kb_Del));
            break;
        }

        // Reveal Square
        if (kb_FallingEdge(okb_Enter, kb_Enter) && ((!Cleared(board, px, py) && !IsFlagged(board, px, py)) || first))
        {
            if (first)
            {
                board = GenerateBoard(width, height, mines, px, py);
                Reveal(board, px, py, tilemap);
                timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
                first = false;
            }

            if (IsMine(board, px, py))
            {
                Lose(board, tilemap, minesLeft, seconds);
                break;
            }

            Reveal(board, px, py, tilemap);
        }

        // Toggle Flag
        if (kb_FallingEdge(okb_Add, kb_Add) && !Cleared(board, px, py) && !first)
        {
            ToggleFlag(board, px, py);

            if (IsFlagged(board, px, py))
            {
                minesLeft--;
                gfx_SetTileMapped(tilemap, px, py, FLAG_TILE);
            }
            else
            {
                minesLeft++;
                gfx_SetTileMapped(tilemap, px, py, HIDDEN_TILE);
            }
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
        gfx_Rectangle(px * 12 + 80, py * 12, 12, 12);

        DrawUI(minesLeft, seconds);
        
        gfx_SwapDraw();
    }
    while (!kb_FallingEdge(okb_Del, kb_Del));

    Cleanup(board, tilemap, first);
    return 0;
}

#define MIN(A, B) (A < B) * A + (B <= A) * B
#define MAX(A, B) (A > B) * A + (B >= A) * B

void Reveal(Board* b, int x, int y, gfx_tilemap_t* t)
{
    // Don't check an area outside of the board or a square that is already cleared
    if (x < 0 || y < 0 || x >= b->width || y >= b->height) return;
    if (Cleared(b, x, y)) return;
    // Clear the square
    Clear(b, x, y);
    int nearby = NearMines(b, x, y);
    gfx_SetTileMapped(t, x, y, nearby + 1);

    // If there are no nearby mines, clear surrounding squares
    if (nearby == 0)
    {
        for (int i = MAX(y - 1, 0); i <= MIN(y + 1, b->height); i++)
        {
            for (int j = MAX(x - 1, 0); j <= MIN(x + 1, b->width); j++)
            {
                if (IsMine(b, j, i)) continue;
                Reveal(b, j, i, t);
            }
        } 
    }
}

void Lose(Board* b, gfx_tilemap_t* t, int minesLeft, int seconds)
{
    // Reveal all the mine tiles
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
    PrintCenter("you lose");
    DrawUI(minesLeft, seconds);
    gfx_SwapDraw();

    do
    {
        ReadKeypad();
    }
    while (!kb_FallingEdge(okb_Del, kb_Del));
}

/*Free resources used by the game*/
void Cleanup(Board* b, gfx_tilemap_t* t, bool first)
{
    if (!first)
       FreeBoard(b);
    if (t)
        FreeTilemap(t);
}

void DrawUI(int minesLeft, int seconds)
{
    // UI Text Colors
    gfx_SetTextFGColor(0x01);
    gfx_SetTextBGColor(0x02);
    // Mines Left
    gfx_PrintStringXY("Mines Left:", 2, LCD_HEIGHT - 24);
    gfx_SetTextXY(2, LCD_HEIGHT - 12);
    gfx_PrintInt(minesLeft, 1);
    // Controls
    gfx_PrintStringXY("Enter:", 2, 16);
    gfx_PrintStringXY("Reveal", 2, 28);
    gfx_PrintStringXY("+:", 2, 42);
    gfx_PrintStringXY("Flag", 2, 54);
    gfx_PrintStringXY("Arrows:", 2, 70);
    gfx_PrintStringXY("Move", 2, 82);
    gfx_PrintStringXY("Del:", 2, 98);
    gfx_PrintStringXY("Quit", 2, 110);
    // Timer
    int mins = seconds / 60;
    int secs = seconds - mins * 60;
    gfx_SetTextXY(2, 2);
    gfx_SetTextFGColor(0x03);
    gfx_PrintInt(mins, 2);
    gfx_PrintChar(':');
    gfx_PrintInt(secs, 2);
}

/*Print a message in a box in the center of the screen*/
void PrintCenter(const char* msg)
{
    gfx_SetColor(0x02);
    gfx_FillRectangle_NoClip(LCD_WIDTH / 2 - 35, LCD_HEIGHT / 2 - 8, 68, 16);
    gfx_SetColor(0x01);
    gfx_Rectangle_NoClip(LCD_WIDTH / 2 - 35, LCD_HEIGHT / 2 - 8, 68, 16);
    gfx_Rectangle_NoClip(LCD_WIDTH / 2 - 32, LCD_HEIGHT / 2 - 6, 62, 12);
    gfx_PrintStringXY(msg, LCD_WIDTH / 2 - 28, LCD_HEIGHT / 2 - 4);
}