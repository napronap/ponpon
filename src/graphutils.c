#include "raylib.h"
#include <stddef.h>

#include "../include/tilecatalog.h"
#include "../include/graphutils.h"
#include "../include/globals.h"
#include "../include/tileutils.h"

Texture2D GR_TileTextures[];

Texture2D GR_OtherTextures[];

Texture2D GenerateTileTexture(Image tileBg, char* tileName) {
	float tileContentW = TILE_WIDTH * .9f;
	float tileContentH = TILE_HEIGHT * .9f;
	
	Image bgCopy = ImageCopy(tileBg);

	Image tileContent = LoadImage(tileName);

	ImageResize(&bgCopy, TILE_WIDTH, TILE_HEIGHT);
	ImageResize(&tileContent, tileContentW, tileContentH);

	ImageDrawImagePro(
		&bgCopy, 
		tileContent,
		(Rectangle){0,0,tileContentW, tileContentH},
		(Rectangle){(TILE_WIDTH - tileContentW) / 2,(TILE_HEIGHT - tileContentH) / 2, tileContentW, tileContentH},
		(Vector2){0,0},
		0.f,
		WHITE
	);

	Texture2D tileTexture = LoadTextureFromImage(bgCopy);

	UnloadImage(bgCopy);
	UnloadImage(tileContent);

	return tileTexture;
};

void LoadAllTileTextures() {
	Image tileBg = LoadImage(TilesCatalog[TOTAL_TILE_TEXTURES - 1]);

	for (int i = 0; i < TOTAL_TILE_TEXTURES; i++) {
		if (i == TOTAL_TILE_TEXTURES -1 ) continue;
		GR_TileTextures[i] = GenerateTileTexture(tileBg, TilesCatalog[i]);
	}

	UnloadImage(tileBg);
}

void LoadOtherTextures() {
	Image board = LoadImage(ASSETS_DIR"board.png");

	// board is square
	ImageResize(&board, SCREEN_HEIGHT, SCREEN_HEIGHT);
	Texture2D boardTxt = LoadTextureFromImage(board);

	GR_OtherTextures[0] = boardTxt;

	UnloadImage(board);
}

Rectangle GetRectForTileInHand(int tileidx) {
	int extraMargin = tileidx == TILES_IN_HAND - 1 ? 15 : 0;
	int margin = HAND_LEFT_MARGIN + tileidx * (TILE_WIDTH + TILE_SEPARATION) + extraMargin;
	Rectangle tileBounds = {margin, SCREEN_HEIGHT - TILE_HEIGHT - 15, TILE_WIDTH, TILE_HEIGHT};

	// keep the raised part inside the hover area so the tile does not jump back down
	Rectangle hoverBounds = tileBounds;
	hoverBounds.y -= 15;
	hoverBounds.height += 15;

	if (CheckCollisionPointRec(GetMousePosition(), hoverBounds)) {
		tileBounds.y -= 15;
	}

	return tileBounds;
}

void RenderHand(int* handTiles) {
	for (int i = 0; i < TILES_IN_HAND; i++) {
		if(handTiles[i] == -1) continue;

		Rectangle tileBounds = GetRectForTileInHand(i);
		DrawTexture(GR_TileTextures[handTiles[i]], tileBounds.x, tileBounds.y, WHITE);
	}
}

void RenderBoard() {
	DrawTexturePro(GR_OtherTextures[0], (Rectangle){0,0,SCREEN_HEIGHT,SCREEN_HEIGHT}, (Rectangle){350,0,SCREEN_HEIGHT,SCREEN_HEIGHT},(Vector2){0,0},0,WHITE);
}

void RenderRivers(const Player players[]) {
	Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
	float rowWidth = RIVER_TILES_PER_ROW * RIVER_TILE_WIDTH
		+ (RIVER_TILES_PER_ROW - 1) * RIVER_TILE_GAP;

	// seats go bottom, right, top, left (always start from 0 since it's the user player)
	// cols follow each player's viewpoint
	const Vector2 columnDirections[PLAYERS] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
	const Vector2 rowDirections[PLAYERS] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
	const float rotations[PLAYERS] = {0, -90, 180, 90};

	for (int seat = 0; seat < PLAYERS; seat++) {
		const Player* player = &players[seat];
		ptrdiff_t discardCount = player->next_kawa_ptr - player->kawa;
		int row = 0;
		float columnOffset = 0;

		for (int i = 0; i < discardCount; i++) {
			// after the first rows, the last row extends instead of wrapping again
			if (i > 0 && i % RIVER_TILES_PER_ROW == 0 && row < RIVER_ROWS - 1) {
				row++;
				columnOffset = 0;
			}

			Kawa discard = player->kawa[i];
			float footprint = discard.is_riichi ? RIVER_TILE_HEIGHT : RIVER_TILE_WIDTH;

			// horizontal displacement
			float alongRow = -rowWidth / 2.0f + columnOffset + footprint / 2.0f;
			// vertical (from the center)
			float outward = RIVER_CENTER_DISTANCE + row * (RIVER_TILE_HEIGHT + RIVER_TILE_GAP) + RIVER_TILE_HEIGHT / 2.0f;

			Vector2 position = {
				center.x + columnDirections[seat].x * alongRow + rowDirections[seat].x * outward,
				center.y + columnDirections[seat].y * alongRow + rowDirections[seat].y * outward
			};

			Texture2D texture = GR_TileTextures[discard.id];
			Rectangle source = {0, 0, texture.width, texture.height};
			Rectangle destination = {position.x, position.y, RIVER_TILE_WIDTH, RIVER_TILE_HEIGHT};
			Vector2 origin = {RIVER_TILE_WIDTH / 2.0f, RIVER_TILE_HEIGHT / 2.0f};
			float rotation = rotations[seat] + (discard.is_riichi ? 90 : 0);

			DrawTexturePro(texture, source, destination, origin, rotation, WHITE);

			columnOffset += footprint + RIVER_TILE_GAP;
		}
	}
}

Rectangle GetRectForMeldOption(int option_index) {
	return (Rectangle){15, 160 + option_index * 62, 315, 54};
}

void RenderMeldOptions(const GameState* game_state) {
	if (game_state->turn_status != WAITING_FOR_MELD_DECISION) return;

	DrawText("Elegir llamada", 20, 115, 22, BLACK);
	int discard = game_state->meld_discard_tile;
	if (discard >= 0 && discard < INDIVIDUAL_TILES) {
		Texture2D texture = GR_TileTextures[discard];
		DrawTexturePro(texture, (Rectangle){0, 0, texture.width, texture.height},
			(Rectangle){275, 96, 34, 51}, (Vector2){0, 0}, 0, WHITE);
	}

	const char* names[] = {"CHII", "PON", "KAN", "ANKAN"};
	for (int i = 0; i <= game_state->meld_option_count; i++) {
		Rectangle bounds = GetRectForMeldOption(i);
		bool hover = CheckCollisionPointRec(GetMousePosition(), bounds);
		DrawRectangleRounded(bounds, 0.15f, 4, hover ? SKYBLUE : RAYWHITE);
		DrawRectangleLinesEx(bounds, 2, hover ? DARKBLUE : DARKGRAY);
		if (i == game_state->meld_option_count) {
			DrawText("PASS", (int)bounds.x + 14, (int)bounds.y + 17, 20, BLACK);
			continue;
		}

		const MeldOption* option = &game_state->meld_options[i];
		DrawText(names[option->type], (int)bounds.x + 14, (int)bounds.y + 17, 20, BLACK);
		int size = option->type == KAN ? 4 : 3;
		for (int tile = 0; tile < size; tile++) {
			Texture2D texture = GR_TileTextures[option->tiles[tile]];
			DrawTexturePro(texture, (Rectangle){0, 0, texture.width, texture.height},
				(Rectangle){bounds.x + 110 + tile * 33, bounds.y + 5, 29, 44},
				(Vector2){0, 0}, 0, WHITE);
		}
	}
}

void RenderMelds(Player* player) {
	if (player->meld_count == 0) return;
	DrawText("Tus melds", SCREEN_WIDTH - 300, 375, 20, BLACK);

	for (int meld = 0; meld < player->meld_count; meld++) {
		const int* tiles = player->melds[meld];
		int size = tiles[3] == -1 ? 3 : 4;
		int first = tiles[0];
		int second = tiles[1];
		ReplaceRed5(&first, 1);
		ReplaceRed5(&second, 1);
		const char* name = size == 4 ? "KAN" : (first == second ? "PON" : "CHII");
		int top = 400 + meld * 82;
		DrawText(name, SCREEN_WIDTH - 300, top, 14, DARKGRAY);

		int called_position = player->meld_from[meld] == 3 ? 0
			: (player->meld_from[meld] == 2 ? 1 : size - 1);
		float x = SCREEN_WIDTH - 300;
		int own_tile = 1;
		for (int column = 0; column < size; column++) {
			bool called = column == called_position;
			int tile = called ? tiles[0] : tiles[own_tile++];
			Texture2D texture = GR_TileTextures[tile];
			float footprint = called ? 66 : 44;
			float center_y = top + 20 + 66 - (called ? 44 : 66) / 2.0f;
			DrawTexturePro(texture, (Rectangle){0, 0, texture.width, texture.height},
				(Rectangle){x + footprint / 2, center_y, 44, 66},
				(Vector2){22, 33}, called ? 90 : 0, WHITE);
			x += footprint + 3;
		}
	}
}

