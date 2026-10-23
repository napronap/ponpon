#include "raylib.h"

#include "../include/globals.h"
#include "../include/tileutils.h"
#include "../include/game.h"

GameState G_GameState;

void InitPlayers(Player players[], size_t player_size, int* current_dealer) {
	int player = GetRandomValue(TON, PEI);

	// init all tiles in hand to -1
	for (int i = 0; i < player_size; i++) {
		for (int tile = 0; tile < TILES_IN_HAND; tile++) {
			players[i].hand[tile] = -1;
		}
	}

	players[0].current_wind = player;
	if (player == TON) {
		*current_dealer = 0;
	}

	// init all winds and set current_dealer in the game state
	for (int i = 0; i < player_size - 1; i++) {
		if (players[i].current_wind + 1 > PEI) {
			players[i+1].current_wind = TON;
			*current_dealer = i + 1;
		} else {
			players[i+1].current_wind = players[i].current_wind + 1;
		}
	}
}

void InitGame() {
	G_GameState.round_wind = TON;

	InitPlayers(G_GameState.players, sizeof(G_GameState.players) / sizeof(G_GameState.players[0]), &G_GameState.current_dealer);

	GenerateTileSet(G_GameState.tileset, sizeof(G_GameState.tileset) / sizeof(G_GameState.tileset[0]));

	DealTiles(G_GameState.tileset, G_GameState.players, &G_GameState.next_draw_index);

}
