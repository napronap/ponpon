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

void DealTiles(int tileset[], Player players[], int* next_draw_index) {
	int dice = GetRandomValue(1,6) + GetRandomValue(1,6);

	int dealerpos = -1;

	for (int i = 0; i < PLAYERS; i++) {
		if (players[i].current_wind == TON) {
			dealerpos = i;
			break;
		}
	}

	int startingwall = (dealerpos + dice - 1) % PLAYERS;

	int tileInWall = dice * 2;
	int wall = startingwall;

	// three rounds of four tiles, then one tile for each player
	for (int round = 0; round < 4; round++) {
		int tilesThisRound = round < 3 ? 4 : 1;

		for (int j = 0; j < PLAYERS; j++) {
			int player = (dealerpos + j) % PLAYERS;

			for (int tile = 0; tile < tilesThisRound; tile++) {
				int src = wall * TILES_PER_WALL + tileInWall;
				int dst = round * 4 + tile;

				players[player].hand[dst] = tileset[src];
				tileset[src] = -1;

				tileInWall++;

				if (tileInWall == TILES_PER_WALL) {
					tileInWall = 0;
					wall = (wall + PLAYERS - 1) % PLAYERS;
				}
			}
		}
	}

	// east starts with 14 tiles, the others have an empty final slot
	for (int i = 0; i < PLAYERS; i++) {
		players[i].hand[TILES_IN_HAND - 1] = -1;
	}
	players[dealerpos].hand[TILES_IN_HAND - 1] = tileset[wall * TILES_PER_WALL + tileInWall];
	tileset[wall * TILES_PER_WALL + tileInWall] = -1;
	*next_draw_index = wall * TILES_PER_WALL + tileInWall + 1;
}

void DrawTile(int tileset[], int* next_draw_index, Player* player) {
	player->hand[TILES_IN_HAND-1] = tileset[*next_draw_index];
	tileset[*next_draw_index] = -1;

	// TODO: actually calculate the next draw index according to wall etc
	*next_draw_index += 1;
}

void DiscardTile(Player* player, int tileidx) {
	player->kawa[player->current_kawa_size] = player->hand[tileidx];
	player->current_kawa_size += 1;
	player->hand[tileidx] = -1;
}

int compare(const void* a, const void* b) {
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

void OrderHand(Player* player) {
	// -1 to size so we only order the first 13
	// it will never happen that we need to order the drawn tile
	qsort(player->hand, (sizeof(player->hand) / sizeof(player->hand[0])) - 1, sizeof(player->hand[0]), compare);
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
