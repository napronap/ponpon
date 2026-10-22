#include <inttypes.h>
#include <stdlib.h>
#include "raylib.h"

#include "../include/tileutils.h"
#include "../include/globals.h"

void GenerateTileSet(int* tileset) {
	for (int pos = 0; pos < TOTAL_TILES_NUM; pos++) {
		
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
	for (int pos = TOTAL_TILES_NUM - 1; pos > 0; pos--) {
		int swap = GetRandomValue(0, pos);
		int copy = tileset[pos];

		tileset[pos] = tileset[swap];
		tileset[swap] = copy;
	}
}

void DealTiles(int* tileset, Player* players) {
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
