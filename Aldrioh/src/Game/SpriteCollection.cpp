#include "pch.h"
#include "SpriteCollection.h"

#include "Graphics/SubTexture.h"


static std::shared_ptr<Texture> spritesheet;
static std::shared_ptr<Texture> squareBox;

static SubTexture* spriteMap;
static constexpr uint32_t size = 256;

// direct use textures, why am I mixing so many different ways to access texture
Texture* SpriteCollection::squareTileTexture;

void SpriteCollection::init()
{
	spriteMap = new SubTexture[size];
	spritesheet = std::make_shared<Texture>("assets/textures/spritesheet.png");

	spriteMap[null] = { spritesheet, glm::vec2{ 1, 1 }, SpriteCollection::Tile_size };
	spriteMap[sand_1] = { spritesheet, glm::vec2{ 0, 0 }, SpriteCollection::Tile_size };
	spriteMap[sand_rock] = { spritesheet, glm::vec2{ 1, 0 }, SpriteCollection::Tile_size };
	spriteMap[sand_cactus] = { spritesheet, glm::vec2{ 2, 0 }, SpriteCollection::Tile_size };
	spriteMap[fire] = { spritesheet, glm::vec2{ 0, 1 }, SpriteCollection::Tile_size };

	spriteMap[player_head] = { spritesheet, glm::vec2{ 0, 2 }, SpriteCollection::Tile_size };
	spriteMap[slime] = { spritesheet, glm::vec2{ 1, 2 }, SpriteCollection::Tile_size };

	// Textures
	{
		constexpr int width = 16, height = 16, channels = 4;
		uint32_t pixelData[width * height];

		for (int i = 0; i < width * height; i++)
			pixelData[i] = 0;

		// I am sorry for you future rhv9... I know you will hate reading this code here.
		for (int i = 0; i < 2; i++)
		{
			for (int x = 0; x < width; x++)
			{
				pixelData[x + width * ((~i+1) & (height - 1))] = 0xFF0000FF;
			}
		}


		for (int y = 0; y < height; y++)
		{
			pixelData[0 + width * y] = 0xFF0000FF;
		}

		for (int y = 0; y < height; y++)
		{
			pixelData[(width - 1) + width * y] = 0xFF0000FF;
		}
		squareTileTexture = new Texture(pixelData, width, height, channels);
	}
}

SubTexture* SpriteCollection::get(int index)
{
	return &spriteMap[index];
}

