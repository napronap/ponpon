#include "raylib.h"

#include "../include/tilecatalog.h"
#include "../include/graphutils.h"
#include "../include/globals.h"

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

void LoadAllTiles(Texture2D tilesTxtArr[]) {
	Image tileBg = LoadImage(TILES_ASSET_DIR"Front.png");

	for (int i = 0; i < TOTAL_TILES_TEXTURES; i++) {
		tilesTxtArr[i] = GenerateTileTexture(tileBg, TilesCatalog[i]);
	}

	UnloadImage(tileBg);
}

Rectangle GetRectForTileInHand(int posInHand) {
	int margin = HAND_LEFT_MARGIN + posInHand * TILE_WIDTH + posInHand * TILE_SEPARATION;

	Rectangle tileBounds = {margin + SCREEN_WIDTH/2.0f - TILE_WIDTH / 2.0f, (SCREEN_HEIGHT - TILE_HEIGHT - 15) / 2.0f - TILE_HEIGHT/2.0f, TILE_WIDTH, TILE_HEIGHT};

	return tileBounds;
}

void DrawHand(Texture2D tilesTxtArr[], int* handTiles) {
	for (int i = 0; i < TILES_IN_HAND; i++) {
		int margin = HAND_LEFT_MARGIN + i * TILE_WIDTH + i * TILE_SEPARATION;

		int verticalOffset = 15;

		if (CheckCollisionPointRec(GetMousePosition(), GetRectForTileInHand(i))) {
			verticalOffset = 30;
		}

		DrawTexture(tilesTxtArr[handTiles[i]], margin, SCREEN_HEIGHT - TILE_HEIGHT - verticalOffset, WHITE);
	}
}

