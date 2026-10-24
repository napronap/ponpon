#pragma once

#include <stdbool.h>

#include "game.h"

extern bool DEBUG_MODE;

void DrawDebug(void);

void ActivateDebug(GameState* game_state);
