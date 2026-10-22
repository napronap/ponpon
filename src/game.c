#include "raylib.h"

#include "../include/globals.h"
#include "../include/tileutils.h"
#include "../include/game.h"

void SetupPlayers(Player* players) {
	int player = GetRandomValue(TON, PEI);

	for (int i = 0; i < PLAYERS; i++) {
		for (int tile = 0; tile < TILES_IN_HAND; tile++) {
			players[i].hand[tile] = -1;
		}
	}

	players[0].current_wind = player;

	for (int i = 0; i < PLAYERS - 1; i++) {
		if (players[i].current_wind + 1 > PEI) {
			players[i+1].current_wind = TON;
		} else {
			players[i+1].current_wind = players[i].current_wind + 1;
		}
	}
}
