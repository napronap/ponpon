#pragma once

#include "raylib.h"
#include "../include/globals.h"

extern Texture2D GR_TileTextures[TOTAL_TILE_TEXTURES];

Texture2D GenerateTileTexture(Image tileBg, char* tileName);

void LoadAllTileTextures();

void RenderHand(Texture2D tilesTxtArr[], int* handTiles);

void RenderRiver();

Rectangle GetRectForTileInHand(int tileidx);
