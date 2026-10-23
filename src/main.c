#include "raylib.h"

#include "../include/graphutils.h"
#include "../include/globals.h"
#include "../include/tileutils.h"
#include "../include/game.h"

int main(void) {
	//----------------------------------------
	// Initialization
	//----------------------------------------
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PonPon!");

	SetTargetFPS(60);
	//---------------------------------------

	Texture2D TileTextures[TOTAL_TILES_TEXTURES];

	LoadAllTiles(TileTextures);

	InitGame();

	OrderHand(&G_GameState.players[0]);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(DARKGREEN);

		DrawHand(TileTextures, G_GameState.players[0].hand);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
