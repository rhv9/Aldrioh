#pragma once

#include "Graphics/SubTexture.h"

namespace Sprites {

	void init();

	constexpr glm::vec2 TileSize { 16.0f, 16.0f };

	// Subtextures
	extern int null;
	extern int sand_1;
	extern int sand_rock;
	extern int sand_cactus;
	extern int fire;

	extern int player_head;
	extern int slime;
	extern int target;
	
	extern std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	extern std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;

	// Textures
	extern Texture* squareTileTexture;

	SubTexture* get(int index);

}