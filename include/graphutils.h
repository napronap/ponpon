#pragma once

#include "raylib.h"
#include "../include/globals.h"
#include "game.h"

extern Texture2D GR_TileTextures[TOTAL_TILE_TEXTURES];

// doing this manually as I add textures
#define TMP_OTHER_TEXTURES_NUM (1)
extern Texture2D GR_OtherTextures[TMP_OTHER_TEXTURES_NUM];

Texture2D GenerateTileTexture(Image tileBg, char* tileName);

void LoadAllTileTextures();

void LoadOtherTextures();

void RenderHand(int* handTiles);

void RenderRivers(const Player players[]);

void RenderBoard();

Rectangle GetRectForTileInHand(int tileidx);
