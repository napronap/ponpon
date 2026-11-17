#include <stdbool.h>
#include <string.h>

#include "../include/agari.h"
#include "../include/game.h"
#include "../include/globals.h"
#include "../include/tileutils.h"

bool IsAgari(Player *player) {
	return true;
}

void To34Tiles(int* handTiles, int* hand34) {
	int copy[TILES_IN_HAND];

	memcpy(copy, handTiles, sizeof(copy));

	ReplaceRed5(copy, TILES_IN_HAND);

	for (int i = 0; i < TILES_IN_HAND; ++i) {
		int tile = copy[i];

		++hand34[tile];
	}
}