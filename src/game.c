#include "raylib.h"

#include "../include/globals.h"
#include "../include/tileutils.h"
#include "../include/game.h"


void InitPlayers(Player players[], size_t player_size, Player** current_dealer) {
	int firstWind = GetRandomValue(TON, PEI);

    for (size_t i = 0; i < player_size; i++) {
        Player* player = &players[i];

        player->current_wind = TON + (firstWind - TON + i) % PLAYERS;
        player->next_kawa_ptr = player->kawa;

        for (int tile = 0; tile < TILES_IN_HAND; tile++) {
            player->hand[tile] = -1;
        }

        if (player->current_wind == TON) {
            *current_dealer = player;
        }
    }
}

void InitGame(GameState* game_state) {
	game_state->round_wind = TON;

	InitPlayers(game_state->players, sizeof(game_state->players) / sizeof(game_state->players[0]), &game_state->current_dealer);

	GenerateTileSet(game_state->tileset, sizeof(game_state->tileset) / sizeof(game_state->tileset[0]));

	DealTiles(game_state->tileset, game_state->players, game_state->current_dealer, &game_state->dead_wall_start_ptr, &game_state->next_draw_ptr);

}
