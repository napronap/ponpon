#include "raylib.h"

#include "../include/graphutils.h"
#include "../include/globals.h"
#include "../include/tileutils.h"

int main(void) {
	//----------------------------------------
	// Initialization
	//----------------------------------------
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PonPon!");

	SetTargetFPS(60);
	//---------------------------------------

	Texture2D TileTextures[TOTAL_TILES_TEXTURES];
	int TileSet[TOTAL_TILES_NUM];

	LoadAllTiles(TileTextures);

	GenerateTileSet(TileSet, sizeof(TileSet) / sizeof(TileSet[0]));

	int hand[TILES_IN_HAND];

	for (int i = 0; i < TILES_IN_HAND; i++) {
		hand[i] = TileSet[i];
	}


	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(DARKGREEN);

		DrawHand(TileTextures, hand);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}