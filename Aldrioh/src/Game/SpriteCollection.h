#pragma once

#include "Graphics/SubTexture.h"

#ifdef SPRITES_CPP
	#define SPRITES_EXTERN
#else
	#define SPRITES_EXTERN extern
#endif

using spriteid_t = int;

namespace Sprites {

	void Init();

	constexpr glm::vec2 TileSize { 16.0f, 16.0f };

	// Subtextures
	SPRITES_EXTERN spriteid_t null;
	SPRITES_EXTERN spriteid_t shadow;
	SPRITES_EXTERN spriteid_t sand_1;
	SPRITES_EXTERN spriteid_t sand_rock;
	SPRITES_EXTERN spriteid_t sand_cactus;
	SPRITES_EXTERN spriteid_t bullet_fire, bullet_red, bullet_white;
	SPRITES_EXTERN spriteid_t coin, jewel;


	SPRITES_EXTERN spriteid_t player_head;
	SPRITES_EXTERN spriteid_t slime;
	SPRITES_EXTERN spriteid_t target;
	SPRITES_EXTERN spriteid_t spawner;

	SPRITES_EXTERN spriteid_t player_ship;

	SPRITES_EXTERN spriteid_t asteroid_small;
	SPRITES_EXTERN spriteid_t drone_normal, drone_tank;

	SPRITES_EXTERN spriteid_t borderBox, greenBox, square;
	
	SPRITES_EXTERN std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	SPRITES_EXTERN std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;

	// Textures
	SPRITES_EXTERN Texture* squareTileTexture;

	SubTexture* get(spriteid_t index);

}