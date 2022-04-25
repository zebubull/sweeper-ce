#include "game/loop.h"
#include "game/graphics.h"
#include "gfx/gfx.h"
#include "io/input.h"

#include <fileioc.h>

void DisplayMenu(int w, int h);

void Save(int w, int h)
{
    ti_var_t f = ti_OpenVar("SWEPCE", "w", TI_APPVAR_TYPE);
    ti_Write(&w, 3, 1, f);
    ti_Write(&h, 3, 1, f);
    ti_Close(f);
}

int main(void)
{
    SetupGraphics();
    int s = 0; 
    int w = 12;
    int h = 20;

    ti_var_t f = ti_OpenVar("SWEPCE", "r", TI_APPVAR_TYPE);
    if (f)
    {
        ti_Read(&w, 3, 1, f);
        ti_Read(&h, 3, 1, f);
        ti_Close(f);
    }

    do
    {
        ReadKeypad();
        DisplayMenu(w, h);

        gfx_PrintStringXY(">", 0, s * 12 + 36);
        if (kb_FallingEdge(okb_Up, kb_Up) && s > 0) s--;
        if (kb_FallingEdge(okb_Down, kb_Down) && s < 4) s++;

        if (kb_FallingEdge(okb_Right, kb_Right))
        {
            switch (s)
            {
                case 1:
                if (w < 20) w++;
                break;
                case 2:
                if (h < 20) h++;
                break;
            }
        }

        if (kb_FallingEdge(okb_Left, kb_Left))
        {
            switch (s)
            {
                case 1:
                if (w > 5) w--;
                break;
                case 2:
                if (h > 10) h--;
                break;
            }
        }

        if (kb_FallingEdge(okb_Del, kb_Del))
        {
            gfx_End();
            Save(w, h);
            exit(0);
        }

        if (kb_FallingEdge(okb_Enter, kb_Enter))
        {
            switch (s)
            {
                case 0:
                loop(w, h);
                break;
                case 3:
                gfx_End();
                Save(w, h);
                exit(0);
                break;
            }
        }

        gfx_SwapDraw();
    }
    while (1);

    gfx_End();
}

void DisplayMenu(int w, int h)
{
    gfx_FillScreen(0x02);
    gfx_SetTextFGColor(0x01);
    gfx_SetTextBGColor(0x02);
    gfx_Sprite_NoClip(titlecard, 0, 0);
    gfx_PrintStringXY("Start", 8, 36);
    gfx_PrintStringXY("Width: ", 8, 48);
    gfx_PrintInt(w, 2);
    gfx_PrintStringXY("Height: ", 8, 60);
    gfx_PrintInt(h, 2);
    gfx_PrintStringXY("Quit", 8, 72);
}