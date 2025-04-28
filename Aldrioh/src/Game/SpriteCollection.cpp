#include "pch.h"
#include "SpriteCollection.h"

#include "Graphics/SubTexture.h"

namespace Sprites {

	int null;
	int sand_1;
	int sand_rock;
	int sand_cactus;
	int fire;

	int player_head;
	int slime;
	int target;

	std::vector<int> animPlayerUp;
	std::vector<int> animPlayerUp;
}

std::vector<int> Sprites::animPlayerUp;

static std::shared_ptr<Texture> spritesheet;
static std::shared_ptr<Texture> squareBox;

static SubTexture* spriteMap;
static constexpr uint32_t size = 256;
static int spriteCounter = 0;

// direct use textures, why am I mixing so many different ways to access texture
Texture* Sprites::squareTileTexture;

std::vector<int> CreateAnimSprites(const std::shared_ptr<Texture>& spriteSheet, const glm::vec2 pos, int size)
{
	std::vector<int> sprites;

	
}

void Sprites::init()
{
	
	spriteMap = new SubTexture[size];
	spritesheet = std::make_shared<Texture>("assets/textures/spritesheet.png");

	spriteMap[null = spriteCounter++] = { spritesheet, glm::vec2{ 1, 1 }, Sprites::Tile_size };
	spriteMap[sand_1 = spriteCounter++] = { spritesheet, glm::vec2{ 0, 0 }, Sprites::Tile_size };
	spriteMap[sand_rock = spriteCounter++] = { spritesheet, glm::vec2{ 1, 0 }, Sprites::Tile_size };
	spriteMap[sand_cactus = spriteCounter++] = { spritesheet, glm::vec2{ 2, 0 }, Sprites::Tile_size };
	spriteMap[fire = spriteCounter++] = { spritesheet, glm::vec2{ 0, 1 }, Sprites::Tile_size };

	spriteMap[player_head = spriteCounter++] = { spritesheet, glm::vec2{ 0, 2 }, Sprites::Tile_size };
	spriteMap[slime = spriteCounter++] = { spritesheet, glm::vec2{ 1, 2 }, Sprites::Tile_size };
	spriteMap[target = spriteCounter++] = { spritesheet, glm::vec2{ 2, 2 }, Sprites::Tile_size };


	// Animated Sprites
	animPlayerUp.reserve(4);
	
	

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

SubTexture* Sprites::get(int index)
{
	return &spriteMap[index];
}

