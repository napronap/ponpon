#include "../include/meld.h"
#include "../include/game.h"
#include "../include/tileutils.h"

static int BaseTile(int tile) {
	ReplaceRed5(&tile, 1);
	return tile;
}

static void AddMeldOption(GameState* game_state, MeldNames type, int first, int second, int third) {
	MeldOption option = {.type = type, .tiles = {game_state->meld_discard_tile, first, second, third}};
	int size = type == KAN ? 4 : 3;

	// Sorting the hand's IDs makes identical choices compare equal.
	for (int i = 1; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (option.tiles[j] < option.tiles[i]) {
				int swap = option.tiles[i];
				option.tiles[i] = option.tiles[j];
				option.tiles[j] = swap;
			}
		}
	}
	for (int i = 0; i < game_state->meld_option_count; i++) {
		const MeldOption* previous = &game_state->meld_options[i];
		bool same = previous->type == type;
		for (int tile = 0; tile < size; tile++) {
			if (previous->tiles[tile] != option.tiles[tile]) same = false;
		}
		if (same) return;
	}
	if (game_state->meld_option_count < MAX_MELD_OPTIONS) {
		game_state->meld_options[game_state->meld_option_count++] = option;
	}
}

bool CanCallAnyMeld(GameState* game_state, int tile) {
	game_state->meld_option_count = 0;
	game_state->meld_discard_tile = -1;

	// we only check melds for the user player for now
	Player* player = &game_state->players[0];

	if (game_state->current_player_turn == player) return false;

	if (player->in_riichi || player->meld_count >= MAX_MELDS) return false;

	if (game_state->next_draw_ptr >= game_state->dead_wall_start_ptr) return false;

	game_state->meld_discard_tile = tile;
	int base = BaseTile(tile);
	int matching[TILES_IN_HAND];
	int matching_count = 0;
	for (int i = 0; i < TILES_IN_HAND; i++) {
		if (player->hand[i] != -1 && BaseTile(player->hand[i]) == base) {
			matching[matching_count++] = player->hand[i];
		}
	}

	for (int i = 0; i < matching_count; i++) {
		for (int j = i + 1; j < matching_count; j++) {
			AddMeldOption(game_state, PON, matching[i], matching[j], -1);
		}
	}
	if (matching_count == 3 && game_state->kan_count < MAX_ACTIVE_DORA - 1
		&& game_state->next_rinshanpai_ptr != NULL) {
		AddMeldOption(game_state, KAN, matching[0], matching[1], matching[2]);
	}

	// Only the player on our left (seat 3) can supply a chii.
	if (base < TON && game_state->current_player_turn == &game_state->players[PLAYERS - 1]) {
		int rank = base % TILES_PER_SUIT;
		int suit_start = base - rank;
		for (int start = rank - 2; start <= rank; start++) {
			if (start < 0 || start > TILES_PER_SUIT - 3) continue;
			int needed[2];
			int needed_count = 0;
			for (int offset = 0; offset < 3; offset++) {
				int wanted = suit_start + start + offset;
				if (wanted != base) needed[needed_count++] = wanted;
			}
			for (int i = 0; i < TILES_IN_HAND; i++) {
				if (player->hand[i] == -1 || BaseTile(player->hand[i]) != needed[0]) continue;
				for (int j = 0; j < TILES_IN_HAND; j++) {
					if (player->hand[j] != -1 && BaseTile(player->hand[j]) == needed[1]) {
						AddMeldOption(game_state, CHII, player->hand[i], player->hand[j], -1);
					}
				}
			}
		}
	}
	if (game_state->meld_option_count == 0) game_state->meld_discard_tile = -1;
	return game_state->meld_option_count > 0;
}

bool CallMeld(GameState* game_state, int option_index) {
	if (game_state->turn_status != WAITING_FOR_MELD_DECISION) return false;
	if (option_index < 0 || option_index >= game_state->meld_option_count) return false;

	Player* player = &game_state->players[0];
	Player* discarder = game_state->current_player_turn;
	MeldOption option = game_state->meld_options[option_index];
	if (player == discarder || player->in_riichi || player->meld_count >= MAX_MELDS) return false;
	if (discarder->next_kawa_ptr == discarder->kawa) return false;
	if (discarder->next_kawa_ptr[-1].id != game_state->meld_discard_tile) return false;
	if (game_state->next_draw_ptr >= game_state->dead_wall_start_ptr) return false;
	if (option.type != CHII && option.type != PON && option.type != KAN) return false;
	if (option.type == KAN && (game_state->kan_count >= MAX_ACTIVE_DORA - 1
		|| game_state->next_rinshanpai_ptr == NULL)) return false;

	// Resolve exact IDs before changing anything, including red-five choices.
	int size = option.type == KAN ? 4 : 3;
	int hand_indices[3] = {-1, -1, -1};
	for (int tile = 1; tile < size; tile++) {
		for (int i = 0; i < TILES_IN_HAND; i++) {
			bool used = false;
			for (int previous = 0; previous < tile - 1; previous++) {
				if (hand_indices[previous] == i) used = true;
			}
			if (!used && player->hand[i] == option.tiles[tile]) {
				hand_indices[tile - 1] = i;
				break;
			}
		}
		if (hand_indices[tile - 1] == -1) return false;
	}

	for (int tile = 1; tile < size; tile++) player->hand[hand_indices[tile - 1]] = -1;
	for (int tile = 0; tile < MELD_MAX_SIZE; tile++) {
		player->melds[player->meld_count][tile] = option.tiles[tile];
	}
	player->meld_from[player->meld_count] = (int)(discarder - game_state->players);
	player->meld_count++;

	// The claimed tile moves out of the river and into the exposed group.
	discarder->next_kawa_ptr--;
	*discarder->next_kawa_ptr = (Kawa){.id = -1, .is_riichi = false};
	OrderHand(player->hand, TILES_IN_HAND);

	if (option.type == KAN) DrawRinshanTile(game_state, player);
	game_state->current_player_turn = player;
	game_state->turn_status = WAITING_FOR_INPUT;
	game_state->meld_option_count = 0;
	game_state->meld_discard_tile = -1;
	return true;
}

void PassCall(GameState* game_state) {
	if (game_state->turn_status != WAITING_FOR_MELD_DECISION) return;
	game_state->meld_option_count = 0;
	game_state->meld_discard_tile = -1;
	AdvanceTurn(game_state);
}
