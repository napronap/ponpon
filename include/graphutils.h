#pragma once

#include "raylib.h"

Texture2D GenerateTileTexture(Image tileBg, char* tileName);

void LoadAllTiles(Texture2D tilesTxtArr[]);

void DrawHand(Texture2D tilesTxtArr[], int* handTiles);

Rectangle GetRectForTileInHand(int posInHand);