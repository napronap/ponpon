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

		// open debug, this needs to be outside conditionals to intercept the input
		ActivateDebug(&M_GameState);

		if (DEBUG_MODE) {
			CheckPlayerInputDebug();
			CheckDebug();
		} else {
			// Handle the displayed state before advancing the turn.
			CheckPlayerInput(&M_GameState);
			CheckTurn(&M_GameState);
		}

		BeginDrawing();

			// always draw these
			ClearBackground(GRAY);
			RenderBoard();
			RenderRivers(M_GameState.players);
			RenderMelds(&M_GameState.players[0]);

			if (DEBUG_MODE) {
				DrawDebug();
			} else {
				RenderHand(M_GameState.players[0].hand);
				RenderMeldOptions(&M_GameState);
			}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
