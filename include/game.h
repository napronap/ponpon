#pragma once

#include "globals.h"

typedef struct PLAYER {
	int current_wind;
	int score;
	int hand[TILES_IN_HAND];
	int* kawa;
} Player;

void SetupPlayers(Player* players);
