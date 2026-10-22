#include <stdlib.h>
#include "raylib.h"

#include "../include/tileutils.h"

void GenerateTileSet(int* tileset, size_t size) {

	// 4 walls of 34 tiles
	// 17 * 2
	// al jugar, se roban fichas arriba y dps abajo
	// dice roll determines wall break
	// se reparten en los jugadores de 4 en 4 hasta tener 12
	// despues se hace 1 1 1 1 para la ultima ficha
	// +1 para el oya (el primer draw desde la live wall)
	//
	// decision de que wall romper basada en dados:
	// 5, 9: East (Dealer)
	// 2, 6, 10: South
	// 3, 7, 11: West
	// 4, 8, 12: North
	//
	// las ultimas 14 fichas se usan para la dead wall:
	// x: drawable, D: dora/kandora, u: uradora, h: haitei (utlima ficha robable, se agrega al final al hacer kan)
	// x1 x3 D D1 D2 D3 D4 / h2 h4
	// x2 x4 u u1 u2 u3 u4 / h1 h3

	for (int pos = 0; pos < size; pos++) {
		
		// the actual type as per the tile enum
		int type = pos / 4;

		int tile = type;

		// every 4th 5 tile has a remainder of 3
		if ((type == PIN_5 || type == SOU_5 || type == MAN_5) && pos % 4 == 3) {
			// first red 5 id + since each suit has 9 entries it should give an int representing which one should we choose
			tile = PIN_5_AKA + type / 9;
		}

		tileset[pos] = tile;
	}

	// shuffle
	for (int pos = size - 1; pos > 0; pos--) {
		int swap = GetRandomValue(0, pos);
		int copy = tileset[pos];

		tileset[pos] = tileset[swap];
		tileset[swap] = copy;
	}
}

void ReplaceRed5(int* tileset, size_t size) {
	for (int i = 0; i < size; i++) {
		switch (tileset[i]) {
		case PIN_5_AKA:
			tileset[i] = PIN_5;
			break;
		case SOU_5_AKA:
			tileset[i] = SOU_5;
			break;
		case MAN_5_AKA:
			tileset[i] = MAN_5;
			break;
		}
	}
}