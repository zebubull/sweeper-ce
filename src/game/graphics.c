#include "graphics.h"

gfx_tilemap_t* CreateTilemap(unsigned int width, unsigned int height)
{

    gfx_tilemap_t* tilemap = malloc(sizeof(gfx_tilemap_t));
    tilemap->map = calloc(width * height, 1);
    tilemap->tiles = tilemap_tiles;
    tilemap->type_width = gfx_tile_no_pow2;
    tilemap->type_height = gfx_tile_no_pow2;
    tilemap->tile_width = TILE_WIDTH;
    tilemap->tile_height = TILE_HEIGHT;
    tilemap->draw_width = 12;
    tilemap->draw_height = 20;
    tilemap->width = width;
    tilemap->height = height;
    tilemap->x_loc = gfx_lcdWidth / 2 - TILE_WIDTH * width / 2;
    tilemap->y_loc = 0;

    return tilemap;
}

void FreeTilemap(gfx_tilemap_t* t)
{
    free(t->map);
}

void SetupGraphics(void)
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetColor(0);
    gfx_SetTextFGColor(1);
    gfx_SetTextBGColor(0);
}
