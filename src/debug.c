#include <stdbool.h>
#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../include/agari.h"
#include "../include/debug.h"
#include "../include/game.h"
#include "../include/globals.h"
#include "../include/graphutils.h"
#include "../include/tileutils.h"

typedef struct DEBUG_STATE {
	int 	debug_hand[TILES_IN_HAND];
	int 	selected_tile;
	char 	tile_input[16];
	int 	input_length;
	bool 	show_tile_help;
	bool 	is_agari;
} DebugState;

bool DEBUG_MODE = false;

static DebugState debug_state = {.selected_tile = -1};

static void CheckDebugHand(void) {
	Player player = {0};
	memcpy(player.hand, debug_state.debug_hand, sizeof(player.hand));
	debug_state.is_agari = IsAgari(&player);
}

static void ClearTileInput(void) {
	debug_state.tile_input[0] = '\0';
	debug_state.input_length = 0;
}

static void ChangeTileInHand(void) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		debug_state.selected_tile = -1;
		ClearTileInput();
		Vector2 mouse = GetMousePosition();

		// empty slots are selectable too, so a 13-tile hand can be completed.
		for (int i = 0; i < TILES_IN_HAND; i++) {
			if (CheckCollisionPointRec(mouse, GetRectForTileInHand(i))) {
				debug_state.selected_tile = i;
				break;
			}
		}
	}

	for (int character = GetCharPressed(); character != 0; character = GetCharPressed()) {
		if (debug_state.selected_tile < 0 || character < '0' || character > '9') continue;
		if (debug_state.input_length >= (int)sizeof(debug_state.tile_input) - 1) continue;

		debug_state.tile_input[debug_state.input_length++] = (char)character;
		debug_state.tile_input[debug_state.input_length] = '\0';
	}

	if (debug_state.selected_tile < 0) return;

	if (IsKeyPressed(KEY_BACKSPACE) && debug_state.input_length > 0) {
		debug_state.tile_input[--debug_state.input_length] = '\0';
	}

	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
		long tile = strtol(debug_state.tile_input, NULL, 10);
		if (debug_state.input_length == 0 || tile < 0 || tile >= INDIVIDUAL_TILES) {
			ClearTileInput();
			return;
		}

		debug_state.debug_hand[debug_state.selected_tile] = (int)tile;
		debug_state.selected_tile = -1;
		ClearTileInput();
		CheckDebugHand();
	}
}

static void ShowTileHelp() {
	if (IsKeyPressed(KEY_S)) {
		debug_state.show_tile_help = !debug_state.show_tile_help;
	}
}

static void DebugOrderTiles() {
	if (IsKeyPressed(KEY_O)) {
		OrderHand(debug_state.debug_hand, 13);
		debug_state.selected_tile = -1;
	}
}

void DrawDebug(void) {
	if (!DEBUG_MODE) return;

	RenderHand(debug_state.debug_hand);
	DrawText("debug mode is on!!", 10, 10, 20, DARKGRAY);
	DrawText(debug_state.is_agari ? "IsAgari: TRUE" : "IsAgari: FALSE",
		10, 36, 20, debug_state.is_agari ? DARKGREEN : MAROON);
	DrawText("D: exit | RMB: choose tile | O: order tiles | S: show tile help reference", 10, 66, 16, DARKGRAY);
	DrawText("ID 0-36 + Enter", 10, 88, 16, DARKGRAY);
	int debug_text_last_pos = 88;

	for (int i = 0; i < TILES_IN_HAND; i++) {
		Rectangle bounds = GetRectForTileInHand(i);
		DrawText(TextFormat("%d", debug_state.debug_hand[i]), (int)bounds.x + 4,
			(int)bounds.y - 20, 16, BLACK);
	}

	if (debug_state.selected_tile >= 0) {
		Rectangle bounds = GetRectForTileInHand(debug_state.selected_tile);
		DrawRectangleLinesEx(bounds, 3, GOLD);
		DrawText(TextFormat("Position %d: ID %s_", debug_state.selected_tile,
			debug_state.tile_input), 10, 120, 18, BLACK);
		debug_text_last_pos = 120;
	}

	if (debug_state.show_tile_help) {
		char* tile_help_text[] = {
			"Pin1-9		0-8",
			"Sou1-9		9-17",
			"Man1-9		18-26",
			"Ton		27",
			"Nan		28",
			"Sha 		29",
			"Pei 		30",
			"Haku		31",
			"Hatsu		32",
			"Chun		33",
			"Dora5		34-36",
		};

		for (size_t i = 0; i < sizeof(tile_help_text) / sizeof(tile_help_text[0]); i++) {
			DrawText(tile_help_text[i], 10, debug_text_last_pos + 22 * (i+1), 16, DARKGRAY);
		}
	}
}

void ActivateDebug(GameState* game_state) {
	if (IsKeyPressed(KEY_D)) {
		DEBUG_MODE = !DEBUG_MODE;

		if (DEBUG_MODE) {
			memcpy(debug_state.debug_hand, game_state->players[0].hand, sizeof(debug_state.debug_hand));
			debug_state.selected_tile = -1;
			ClearTileInput();
			CheckDebugHand();
		}
	}

	if (DEBUG_MODE) {
		ChangeTileInHand();
		ShowTileHelp();
		DebugOrderTiles();
	}
}
