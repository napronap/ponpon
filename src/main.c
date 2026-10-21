#include "raylib.h"

#include "../include/graphutils.h"
#include "../include/globals.h"

int main(void) {
	//----------------------------------------
	// Initialization
	//----------------------------------------
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PonPon!");

	SetTargetFPS(60);
	//---------------------------------------

	Texture2D TileTextures[TOTAL_TILES_TEXTURES];

	LoadAllTiles(TileTextures);

	// Rectangle tileBounds = {screenWidth/2.0f - 75, screenHeight/2.0f - 250/2.0f, 150.f, 250.f};


	// int leftMargin = 75;


	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(DARKGREEN);

		int hand[TILES_IN_HAND] = {23,24,1,3,4,4,2,2,2,12,12,12,13};

		DrawHand(TileTextures, hand);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}