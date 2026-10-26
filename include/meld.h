#pragma once

#include "globals.h"
#include <stdbool.h>

struct GAME_STATE;

typedef enum MELD_NAMES {
	CHII,
	PON,
	KAN,
	CLOSED_KAN
} MeldNames;

typedef struct MELD_OPTION {
	MeldNames type;
	// Called tile first, then the exact IDs to remove from the hand; -1 is empty.
	int tiles[MELD_MAX_SIZE];
} MeldOption;

// Checks calls for players[0] against the current AI player's discard.
bool CanCallAnyMeld(struct GAME_STATE* game_state, int tile);

bool CallMeld(struct GAME_STATE* game_state, int option_index);

void PassCall(struct GAME_STATE* game_state);
