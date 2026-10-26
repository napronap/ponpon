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
        player->in_riichi = false;
        player->meld_count = 0;

        for (int meld = 0; meld < MAX_MELDS; meld++) {
            player->meld_from[meld] = -1;
            for (int tile = 0; tile < MELD_MAX_SIZE; tile++) {
                player->melds[meld][tile] = -1;
            }
        }

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

	DealTiles(game_state->tileset, game_state->players, game_state->current_dealer, &game_state->dead_wall_start_ptr, &game_state->next_draw_ptr, game_state->active_dora, &game_state->next_rinshanpai_ptr);
	game_state->kan_count = 0;
	game_state->meld_option_count = 0;
	game_state->meld_discard_tile = -1;

	game_state->round_wind = TON;
	game_state->current_player_turn = game_state->current_dealer;
	game_state->turn_status = WAITING_FOR_INPUT;
}

void AdvanceTurn(GameState* game_state) {
	ptrdiff_t nextpos_idx = (game_state->current_player_turn - game_state->players + 1) % PLAYERS;
	game_state->current_player_turn = &game_state->players[nextpos_idx];
	game_state->turn_status = START;
}

static bool PrepareTurn(GameState* game_state) {
	if (DEBUG_MODE) return false;
	if (game_state->turn_status == DONE) return false;
	if (game_state->turn_status == WAITING_FOR_MELD_DECISION) return false;

	if (game_state->turn_status == START) {
		// temp: "finish" the game if there are no more tiles to draw
		// TODO: remove
		if (game_state->next_draw_ptr >= game_state->dead_wall_start_ptr) {
			game_state->turn_status = DONE;
			return false;
		}

		DrawTile(game_state->tileset, &game_state->next_draw_ptr, game_state->current_player_turn);
		OrderHand(game_state->current_player_turn->hand, TILES_IN_HAND - 1);
		game_state->turn_status = WAITING_FOR_INPUT;
	}
	return true;
}

static void CheckAITurn(GameState* game_state) {
	if (!PrepareTurn(game_state)) return;

	// AI brain: it will discard a random tile on its turn
	int tile_index = GetRandomValue(0, TILES_IN_HAND - 1);
	int discarded_tile = game_state->current_player_turn->hand[tile_index];
	DiscardTile(game_state->current_player_turn, tile_index, false);
	OrderHand(game_state->current_player_turn->hand, TILES_IN_HAND);
	if (CanCallAnyMeld(game_state, discarded_tile)) {
		game_state->turn_status = WAITING_FOR_MELD_DECISION;
		return;
	}
	game_state->turn_status = DONE;
	AdvanceTurn(game_state);
}

static void CheckPlayerTurn(GameState* game_state) {
	if (!PrepareTurn(game_state)) return;

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

void CheckTurn(GameState* game_state) {
	if (DEBUG_MODE) return;
	if (game_state->turn_status == WAITING_FOR_MELD_DECISION) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mouse = GetMousePosition();
			for (int option = 0; option <= game_state->meld_option_count; option++) {
				if (!CheckCollisionPointRec(mouse, GetRectForMeldOption(option))) continue;
				if (option == game_state->meld_option_count) PassCall(game_state);
				else CallMeld(game_state, option);
				break;
			}
		}
		return;
	}
	if (game_state->current_player_turn == &game_state->players[0]) {
		CheckPlayerTurn(game_state);
	} else {
		CheckAITurn(game_state);
	}
}


void AdvanceRound(GameState* game_state) {

}
