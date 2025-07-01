#include "pch.h"

#define SPRITES_CPP
#include "SpriteCollection.h"

#include "Graphics/SubTexture.h"

static std::shared_ptr<Texture> spritesheet;
static std::shared_ptr<Texture> squareBox;

static SubTexture* spriteMap;
static constexpr uint32_t size = 256;
static int spriteCounter = 0;

std::vector<int> CreateAnimSprites(const std::shared_ptr<Texture>& spriteSheet, const glm::vec2 pos, int animationLength)
{
	std::vector<int> sprites;
	sprites.reserve(animationLength);

	for (int i = 0; i < animationLength; ++i)
	{
		int spriteId = spriteCounter++;
		spriteMap[spriteId] = {spriteSheet, pos + glm::vec2{i, 0.0f}, Sprites::TileSize};
		sprites.push_back(spriteId);
	}

	return sprites;
}

void Sprites::init()
{
	
	spriteMap = new SubTexture[size];
	spritesheet = std::make_shared<Texture>("assets/textures/spritesheet.png");

	spriteMap[null = spriteCounter++] = { spritesheet, glm::vec2{ 1, 1 }, Sprites::TileSize };
	spriteMap[shadow = spriteCounter++] = { spritesheet, glm::vec2{ 0, 3 }, Sprites::TileSize };
	spriteMap[sand_1 = spriteCounter++] = { spritesheet, glm::vec2{ 0, 0 }, Sprites::TileSize };
	spriteMap[sand_rock = spriteCounter++] = { spritesheet, glm::vec2{ 1, 0 }, Sprites::TileSize };
	spriteMap[sand_cactus = spriteCounter++] = { spritesheet, glm::vec2{ 2, 0 }, Sprites::TileSize };
	spriteMap[fire = spriteCounter++] = { spritesheet, glm::vec2{ 0, 1 }, Sprites::TileSize };

	spriteMap[player_head = spriteCounter++] = { spritesheet, glm::vec2{ 0, 2 }, Sprites::TileSize };
	spriteMap[slime = spriteCounter++] = { spritesheet, glm::vec2{ 1, 2 }, Sprites::TileSize };
	spriteMap[target = spriteCounter++] = { spritesheet, glm::vec2{ 2, 2 }, Sprites::TileSize };
	spriteMap[spawner = spriteCounter++] = { spritesheet, glm::vec2{ 3, 2 }, Sprites::TileSize };

	spriteMap[redBox = spriteCounter++] = { spritesheet, glm::vec2{ 2, 3 }, Sprites::TileSize };
	spriteMap[greenBox = spriteCounter++] = { spritesheet, glm::vec2{ 3, 3 }, Sprites::TileSize };


	LOG_INFO("Sprite counter before animations: {}", spriteCounter);
	// Animated Sprites
	animPlayerUp = CreateAnimSprites(spritesheet, glm::vec2{ 12, 3 }, 4);
	animPlayerDown = CreateAnimSprites(spritesheet, glm::vec2{ 12, 2 }, 4);
	animPlayerLeft = CreateAnimSprites(spritesheet, glm::vec2{ 12, 1 }, 4);
	animPlayerRight = CreateAnimSprites(spritesheet, glm::vec2{ 12, 0 }, 4);

	animBossUp = CreateAnimSprites(spritesheet, glm::vec2{ 8, 3 }, 4);
	animBossDown = CreateAnimSprites(spritesheet, glm::vec2{ 8, 2 }, 4);
	animBossLeft = CreateAnimSprites(spritesheet, glm::vec2{ 8, 1 }, 4);
	animBossRight = CreateAnimSprites(spritesheet, glm::vec2{ 8, 0 }, 4);

	for (int i = 0; i < 4; i++)
		LOG_CORE_INFO("Sprite id number {}: {}", i, Sprites::animPlayerUp[i]);


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

