#pragma once

#include "raylib.h"

Texture2D GenerateTileTexture(Image tileBg, char* tileName);

// Will fill up the array with Texture2D as follows:
//---------------------------------------------------
//				Pin1-9		0-8
//				Sou1-9		9-17
//				Man1-9		18-26
//				Ton			27
//				Nan			28
//				Sha 		29
//				Pei 		30
//				Haku		31
//				Hatsu		32
//				Chun		33
//				Dora5		34-36
//---------------------------------------------------
// This has nothing to do with the actual playing tiles
//---------------------------------------------------
void LoadAllTiles(Texture2D tilesTxtArr[]);

void DrawHand(Texture2D tilesTxtArr[], int* handTiles);

Rectangle GetRectForTileInHand(int posInHand);