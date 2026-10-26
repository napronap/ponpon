#pragma once

#include "globals.h"
#include "meld.h"
#include <stdbool.h>

typedef struct KAWA {
	int 	id;
	bool 	is_riichi;
} Kawa;

typedef struct PLAYER {
	int 	current_wind;
	int 	score;
	int 	hand[TILES_IN_HAND];
	Kawa 	kawa[MAX_DISCARDS];
	Kawa*	next_kawa_ptr;
	int		melds[MAX_MELDS][MELD_MAX_SIZE];
	int		meld_count;
	int		meld_from[MAX_MELDS];
	bool 	in_riichi;
} Player;

// turn phases
enum {
	START,
	WAITING_FOR_INPUT,
	WAITING_FOR_MELD_DECISION,
	DONE
};

typedef struct GAME_STATE {
	int 	tileset[TOTAL_TILES_NUM];
	int 	round_wind;
	int 	round_n;
	int 	honba;
	Player 	players[PLAYERS];

	int 	turn_status;

	// ptr in players
	Player* current_player_turn;

	// ptr in players for the current dealer
	Player* current_dealer;

	// ptr in tileset for the next draw
	int* 	next_draw_ptr;

	// ptr in the tileset for where the dead wall starts
	int* 	dead_wall_start_ptr;
	int		kan_count;

	// ptr in tileset for the next tile to be drawn after a kan (see below)
	int*	next_rinshanpai_ptr;

	// Active dora indicator IDs; the first (1 + kan_count) entries are active.
	// Indicators stay at fixed indices 130, 128, 126, 124, 122.
	// The live/dead boundary moves back once per kan; indicator positions do not.
	// while the tiles are represented in a linear list,
	// the idea is that the order is preserved as what it would look like in a real table
	//
	// x: drawable tile (kan), d: dora, u: uradora, the number represents the idx in the tileset array
	//   x 	   x   |   d     d     d     d     d
	// (134) (132) | (130) (128) (126) (124) (122) <--- initial dead wall boundary
	//   x	   x   |   u     u     u     u     u
	// (135) (133) | (131) (129) (127) (125) (123)
	int 	active_dora[MAX_ACTIVE_DORA];

	MeldOption meld_options[MAX_MELD_OPTIONS];
	int		meld_option_count;
	int		meld_discard_tile;
} GameState;

void InitGame();

void InitMatch(GameState* game_state);

void CheckTurn(GameState* game_state);

void AdvanceTurn(GameState* game_state);
