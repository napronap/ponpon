#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include "raylib.h"

#include "../include/tileutils.h"
#include "../include/globals.h"

void GenerateTileSet(int tileset[], size_t tileset_size) {
	for (int pos = 0; pos < tileset_size; pos++) {
		
		// the actual type as per the tile enum
		int type = pos / 4;

		int tile = type;

		// every 4th 5 tile has a remainder of 3
		if ((type == PIN_5 || type == SOU_5 || type == MAN_5) && pos % 4 == 3) {
			// first red 5 id + since each suit has 9 entries it should give an int representing which one should we choose
			tile = PIN_5_AKA + type / 9;
		}

		tileset[pos] = tile;
	}

	// shuffle
	for (int pos = tileset_size - 1; pos > 0; pos--) {
		int swap = GetRandomValue(0, pos);
		int copy = tileset[pos];

		tileset[pos] = tileset[swap];
		tileset[swap] = copy;
	}
}

void DealTiles(int tileset[], Player players[], Player* current_dealer, int** dead_wall_start_ptr, int** next_draw_ptr, int active_dora[MAX_ACTIVE_DORA], int** next_rinshanpai_ptr) {
	int dice = GetRandomValue(1,6) + GetRandomValue(1,6);

	ptrdiff_t dealerpos_idx = current_dealer - players;

	int startingwall = (dealerpos_idx + dice - 1) % PLAYERS;

	int orderedTiles[TOTAL_TILES_NUM];

    int wall = startingwall;
    int tileInWall = dice * 2;

    // reorder walls so that our tileset is easier to read
    for (int dst = 0; dst < TOTAL_TILES_NUM; dst++) {
        int src = wall * TILES_PER_WALL + tileInWall;

        orderedTiles[dst] = tileset[src];

        tileInWall++;

        if (tileInWall == TILES_PER_WALL) {
            tileInWall = 0;
            wall = (wall + PLAYERS - 1) % PLAYERS;
        }
    }

    // apply to the tileset
    for (int i = 0; i < TOTAL_TILES_NUM; i++) {
        tileset[i] = orderedTiles[i];
    }

    (*dead_wall_start_ptr) = &tileset[TOTAL_TILES_NUM - DEAD_WALL_TILES_NUM];
    (*next_draw_ptr) = tileset;

    for (int i = 0; i < MAX_ACTIVE_DORA; i++) {
        active_dora[i] = -1;
    }
    // first active dora will always be 8 steps from the start of the dead wall
    active_dora[0] = *(*dead_wall_start_ptr + 8);

    (*next_rinshanpai_ptr) = &tileset[TOTAL_TILES_NUM - 2];

	// three rounds of 4 tiles 
	// one round of 1 tile
    for (int round = 0; round < 4; round++) {
    	int tilesThisRound = round < 3 ? 4 : 1;

    	for (int thisplayer = 0; thisplayer < PLAYERS; thisplayer++) {
    		int player_to_deal = (dealerpos_idx + thisplayer) % PLAYERS;

    		for (int tile = 0; tile < tilesThisRound; tile++) {
    			players[player_to_deal].hand[round * 4 + tile] = **next_draw_ptr;
    			**next_draw_ptr = -1;
    			(*next_draw_ptr)++;
    		}
    	}
    }

	// east starts with 14 tiles, the others have an empty final slot
	for (int i = 0; i < PLAYERS; i++) {
		players[i].hand[TILES_IN_HAND - 1] = -1;
	}

	players[dealerpos_idx].hand[TILES_IN_HAND - 1] = **next_draw_ptr;
	**next_draw_ptr = -1;
	(*next_draw_ptr)++;
}

void DrawTile(int tileset[], int** next_draw_ptr, Player* player) {
	player->hand[TILES_IN_HAND-1] = **next_draw_ptr;

	**next_draw_ptr = -1;
	(*next_draw_ptr)++;
}

bool DrawRinshanTile(GameState* game_state, Player* player) {
	if (game_state->kan_count >= MAX_ACTIVE_DORA - 1) return false;
	if (game_state->next_draw_ptr >= game_state->dead_wall_start_ptr) return false;
	if (game_state->next_rinshanpai_ptr == NULL) return false;
	if (player->hand[TILES_IN_HAND - 1] != -1) return false;

	player->hand[TILES_IN_HAND - 1] = *game_state->next_rinshanpai_ptr;
	*game_state->next_rinshanpai_ptr = -1;
	game_state->kan_count++;
	game_state->dead_wall_start_ptr--;

	// Two stacks, top then bottom, starting at the end of the wall.
	const int rinshan_indices[] = {TOTAL_TILES_NUM - 2, TOTAL_TILES_NUM - 1,
		TOTAL_TILES_NUM - 4, TOTAL_TILES_NUM - 3};
	game_state->next_rinshanpai_ptr = game_state->kan_count < 4
		? &game_state->tileset[rinshan_indices[game_state->kan_count]] : NULL;
	game_state->active_dora[game_state->kan_count] =
		game_state->tileset[TOTAL_TILES_NUM - 6 - 2 * game_state->kan_count];
	return true;
}

void DiscardTile(Player* player, int tileidx, bool is_riichi) {
	if (tileidx < 0 || tileidx >= TILES_IN_HAND) return;
	if (player->hand[tileidx] == -1) return;
	if (player->next_kawa_ptr >= player->kawa + MAX_DISCARDS) return;

	player->next_kawa_ptr->id = player->hand[tileidx];
	player->next_kawa_ptr->is_riichi = is_riichi;
	player->next_kawa_ptr++;
	player->hand[tileidx] = -1;
}

static int compare(const void* a, const void* b) {
	int inta = *(int*)a;
	int intb = *(int*)b;

	// -1 represents hand empty slots (be it by calling chii/kan/pon or by the empty 14th tile)
	// so they should always go to the back

	if (inta == -1 && intb ==-1) {
		return 0;
	}

	if (inta == -1) {
		return 1;
	}

	if (intb == -1) {
		return -1;
	}

	int copy[] = {inta, intb};

	ReplaceRed5(copy, 2);

	return (copy[0] - copy[1]);
}

void OrderHand(int hand[], int tiles_to_order) {
	// when we pick up a tile, we will only order the first 13
	// order all after discarding (14) so that the discarded tile goes back to the end of the array
	qsort(hand, tiles_to_order, sizeof(hand[0]), compare);
}

void ReplaceRed5(int* tileset, size_t size) {
	for (int i = 0; i < size; i++) {
		switch (tileset[i]) {
		case PIN_5_AKA:
			tileset[i] = PIN_5;
			break;
		case SOU_5_AKA:
			tileset[i] = SOU_5;
			break;
		case MAN_5_AKA:
			tileset[i] = MAN_5;
			break;
		}
	}
}
