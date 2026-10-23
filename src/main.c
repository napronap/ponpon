#include "raylib.h"

#include "../include/graphutils.h"
#include "../include/globals.h"
#include "../include/game.h"

int main(void) {
	//----------------------------------------
	// Initialization
	//----------------------------------------
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PonPon!");

	SetTargetFPS(60);
	//---------------------------------------

	GameState M_GameState;

	InitGame();

	InitMatch(&M_GameState);

	while (!WindowShouldClose()) {

		CheckTurn(&M_GameState);

		BeginDrawing();

		ClearBackground(DARKGREEN);

		RenderHand(GR_TileTextures, M_GameState.players[0].hand);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
