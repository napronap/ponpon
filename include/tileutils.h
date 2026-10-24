#pragma once

#include <stdlib.h>
#include "../include/game.h"

// ---------------------------------------------------
// tile shuffling, ordering and dealing utils
// ---------------------------------------------------

enum {
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	SOU_1,
	SOU_2,
	SOU_3,
	SOU_4,
	SOU_5,
	SOU_6,
	SOU_7,
	SOU_8,
	SOU_9,
	MAN_1,
	MAN_2,
	MAN_3,
	MAN_4,
	MAN_5,
	MAN_6,
	MAN_7,
	MAN_8,
	MAN_9,
	TON,
	NAN,
	SHAA,
	PEI,
	HAKU,
	HATSU,
	CHUN,
	PIN_5_AKA,
	SOU_5_AKA,
	MAN_5_AKA
};

void GenerateTileSet(int tileset[], size_t tileset_size);

// initial deal tiles
void DealTiles(int tileset[], Player players[], Player* current_dealer, int** dead_wall_start_ptr, int** next_draw_ptr);

// draw one tile on a player's turn
void DrawTile(int tileset[], int** next_draw_ptr, Player* player);

void OrderHand(int hand[], int tiles_to_order);

void DiscardTile(Player* player, int tileidx, bool is_riichi);

void ReplaceRed5(int* tileset, size_t size);

