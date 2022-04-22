#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <graphx.h>
#include "../gfx/gfx.h"

#define TILE_WIDTH 12
#define TILE_HEIGHT 12

gfx_tilemap_t* CreateTilemap(unsigned int width, unsigned int height);
void FreeTilemap(gfx_tilemap_t* t);

void SetupGraphics(void);

#endif