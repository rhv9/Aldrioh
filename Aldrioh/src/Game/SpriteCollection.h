#pragma once

#include "Graphics/SubTexture.h"

namespace SpriteCollection {

	void init();

	constexpr glm::vec2 Tile_size { 16.0f, 16.0f };
	extern std::shared_ptr<Texture> yesSheet;

	// Subtextures

	constexpr int null = 0;
	constexpr int sand_1 = 1;
	constexpr int sand_rock = 2;
	constexpr int sand_cactus = 3;
	constexpr int fire = 4;

	constexpr int player_head = 5;
	constexpr int slime = 6;
	constexpr int target = 7;

	// Textures
	extern Texture* squareTileTexture;

	SubTexture* get(int index);

}