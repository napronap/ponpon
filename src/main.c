#include "raylib.h"

#include "../include/graphutils.h"
#include "../include/globals.h"
#include "../include/game.h"
#include "../include/debug.h"

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

		// DEBUG
		ActivateDebug(&M_GameState);

		CheckTurn(&M_GameState);

		BeginDrawing();

		ClearBackground(GRAY);

		RenderBoard();

		RenderRivers(M_GameState.players);

		if (!DEBUG_MODE) RenderHand(M_GameState.players[0].hand);

		// DEBUG
		DrawDebug();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
