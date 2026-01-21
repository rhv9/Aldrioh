#pragma once

#include "Graphics/SubTexture.h"

#ifdef SPRITES_CPP
	#define SPRITES_EXTERN
#else
	#define SPRITES_EXTERN extern
#endif

namespace Sprites {

	void Init();

	constexpr glm::vec2 TileSize { 16.0f, 16.0f };

	// Subtextures
	SPRITES_EXTERN int null;
	SPRITES_EXTERN int shadow;
	SPRITES_EXTERN int sand_1;
	SPRITES_EXTERN int sand_rock;
	SPRITES_EXTERN int sand_cactus;
	SPRITES_EXTERN int bullet_fire, bullet_red, bullet_white;

	SPRITES_EXTERN int player_head;
	SPRITES_EXTERN int slime;
	SPRITES_EXTERN int target;
	SPRITES_EXTERN int spawner;

	SPRITES_EXTERN int player_ship;

	SPRITES_EXTERN int asteroid_small;
	SPRITES_EXTERN int drone_mini;


	SPRITES_EXTERN int redBox, greenBox, square;
	
	SPRITES_EXTERN std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	SPRITES_EXTERN std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;

	// Textures
	SPRITES_EXTERN Texture* squareTileTexture;

	SubTexture* get(int index);

}