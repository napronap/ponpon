#pragma once

#include "globals.h"

typedef struct PLAYER {
	int 	current_wind;
	int 	score;
	int 	hand[TILES_IN_HAND];
	int 	kawa[MAX_DISCARDS];
	int*	next_kawa_ptr;
} Player;

typedef struct GAME_STATE {
	int 	tileset[TOTAL_TILES_NUM];
	int 	round_wind;
	int 	round_n;
	int 	honba;
	Player 	players[PLAYERS];

	// ptr in players for the current dealer
	Player* current_dealer;

	// ptr in tileset for the next draw
	int* 	next_draw_ptr;

	// ptr in the tileset for where the dead wall starts
	int* 	dead_wall_start_ptr;
} GameState;

void InitGame(GameState* game_state);
