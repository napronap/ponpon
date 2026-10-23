#pragma once

#include "globals.h"

typedef struct PLAYER {
	int 	current_wind;
	int 	score;
	int 	hand[TILES_IN_HAND];
	int 	kawa[MAX_DISCARDS];
	int 	current_kawa_size;
} Player;

typedef struct GAME_STATE {
	int 	tileset[TOTAL_TILES_NUM];
	int 	round_wind;
	int 	round_n;
	int 	honba;
	int 	current_dealer;
	int 	next_draw_index;
	Player 	players[PLAYERS];
} GameState;

extern GameState G_GameState;

void InitGame();
