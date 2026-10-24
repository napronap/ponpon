#include <stddef.h>
#include "raylib.h"

#include "../include/globals.h"
#include "../include/tileutils.h"
#include "../include/graphutils.h"
#include "../include/debug.h"
#include "../include/game.h"

void InitGame() {
	LoadAllTileTextures();
	LoadOtherTextures();
}

void InitPlayers(Player players[], size_t player_size, Player** current_dealer) {
	int firstWind = GetRandomValue(TON, PEI);

    for (size_t i = 0; i < player_size; i++) {
        Player* player = &players[i];

        player->current_wind = TON + (firstWind - TON + i) % PLAYERS;
        player->next_kawa_ptr = player->kawa;

        for (int discard = 0; discard < MAX_DISCARDS; discard++) {
            player->kawa[discard] = (Kawa){.id = -1, .is_riichi = false};
        }

        for (int tile = 0; tile < TILES_IN_HAND; tile++) {
            player->hand[tile] = -1;
        }

        if (player->current_wind == TON) {
            *current_dealer = player;
        }
    }
}

void InitMatch(GameState* game_state) {
	InitPlayers(game_state->players, sizeof(game_state->players) / sizeof(game_state->players[0]), &game_state->current_dealer);

	GenerateTileSet(game_state->tileset, sizeof(game_state->tileset) / sizeof(game_state->tileset[0]));

	DealTiles(game_state->tileset, game_state->players, game_state->current_dealer, &game_state->dead_wall_start_ptr, &game_state->next_draw_ptr);

	game_state->round_wind = TON;
	game_state->current_player_turn = game_state->current_dealer;
	game_state->turn_status = WAITING_FOR_INPUT;
}

static void AdvanceTurn(GameState* game_state) {
	ptrdiff_t nextpos_idx = (game_state->current_player_turn - game_state->players + 1) % PLAYERS;
	game_state->current_player_turn = &game_state->players[nextpos_idx];
	game_state->turn_status = START;
}

void CheckTurn(GameState* game_state) {
	if (DEBUG_MODE) return;
	if (game_state->turn_status == DONE) return;

	if (game_state->turn_status == START) {
		// temp: "finish" the game if there are no more tiles to draw
		// TODO: remove
		if (game_state->next_draw_ptr >= game_state->dead_wall_start_ptr) {
			game_state->turn_status = DONE;
			return;
		}

		DrawTile(game_state->tileset, &game_state->next_draw_ptr, game_state->current_player_turn);
		OrderHand(game_state->current_player_turn->hand, TILES_IN_HAND - 1);
		game_state->turn_status = WAITING_FOR_INPUT;
	}

	// AI brain: it will discard a random tile on its turn
	if (game_state->current_player_turn != &game_state->players[0]) {
		DiscardTile(game_state->current_player_turn, GetRandomValue(0, TILES_IN_HAND - 1), false);
		OrderHand(game_state->current_player_turn->hand, TILES_IN_HAND);
		game_state->turn_status = DONE;
		AdvanceTurn(game_state);
	} else {

		// check for player clicks
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Player* player = game_state->current_player_turn;
			Vector2 mouse = GetMousePosition();

			for (int tile = 0; tile < TILES_IN_HAND; tile++) {
				if (player->hand[tile] == -1) continue;

				if (CheckCollisionPointRec(mouse, GetRectForTileInHand(tile))) {
					DiscardTile(player, tile, false);
					OrderHand(player->hand, TILES_IN_HAND);
					game_state->turn_status = DONE;
					AdvanceTurn(game_state);
					break;
				}
			}
		}
	}
}


void AdvanceRound(GameState* game_state) {

}
