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
	int TileSet[TOTAL_TILES_NUM];
	Player Players[PLAYERS];

	LoadAllTiles(TileTextures);

	GenerateTileSet(TileSet);

	SetupPlayers(Players);

	DealTiles(TileSet, Players);



	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(DARKGREEN);

		DrawHand(TileTextures, Players[0].hand);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
