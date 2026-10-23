#include "raylib.h"

#include "../include/tilecatalog.h"
#include "../include/graphutils.h"
#include "../include/globals.h"

Texture2D GR_TileTextures[];

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

Rectangle GetRectForTileInHand(int tileidx) {
	int extraMargin = tileidx == TILES_IN_HAND - 1 ? 15 : 0;
	int margin = HAND_LEFT_MARGIN + tileidx * (TILE_WIDTH + TILE_SEPARATION) + extraMargin;
	Rectangle tileBounds = {margin, SCREEN_HEIGHT - TILE_HEIGHT - 15, TILE_WIDTH, TILE_HEIGHT};

	// Keep the raised part inside the hover area so the tile does not jump back down.
	Rectangle hoverBounds = tileBounds;
	hoverBounds.y -= 15;
	hoverBounds.height += 15;

	if (CheckCollisionPointRec(GetMousePosition(), hoverBounds)) {
		tileBounds.y -= 15;
	}

	return tileBounds;
}

void RenderHand(Texture2D tilesTxtArr[], int* handTiles) {
	for (int i = 0; i < TILES_IN_HAND; i++) {
		if(handTiles[i] == -1) continue;

		Rectangle tileBounds = GetRectForTileInHand(i);
		DrawTexture(tilesTxtArr[handTiles[i]], tileBounds.x, tileBounds.y, WHITE);
	}
}

void RenderRiver() {

}

