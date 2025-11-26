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

void Sprites::Init()
{
	
	spriteMap = new SubTexture[size];
	spritesheet = std::make_shared<Texture>("assets/textures/shooter_spritesheet.png");

	spriteMap[null = spriteCounter++] = { spritesheet, glm::vec2{ 1, 1 }, Sprites::TileSize };
	spriteMap[shadow = spriteCounter++] = { spritesheet, glm::vec2{ 0, 3 }, Sprites::TileSize };
	spriteMap[sand_1 = spriteCounter++] = { spritesheet, glm::vec2{ 0, 0 }, Sprites::TileSize };
	spriteMap[sand_rock = spriteCounter++] = { spritesheet, glm::vec2{ 1, 0 }, Sprites::TileSize };
	spriteMap[sand_cactus = spriteCounter++] = { spritesheet, glm::vec2{ 2, 0 }, Sprites::TileSize };
	
	spriteMap[bullet_fire = spriteCounter++] = { spritesheet, glm::vec2{ 0, 1 }, Sprites::TileSize };
	spriteMap[bullet_red = spriteCounter++] = { spritesheet, glm::vec2{ 0, 2 }, Sprites::TileSize };
	spriteMap[bullet_white = spriteCounter++] = { spritesheet, glm::vec2{ 0, 3 }, Sprites::TileSize };

	spriteMap[player_head = spriteCounter++] = { spritesheet, glm::vec2{ 0, 2 }, Sprites::TileSize };
	spriteMap[slime = spriteCounter++] = { spritesheet, glm::vec2{ 1, 2 }, Sprites::TileSize };
	spriteMap[target = spriteCounter++] = { spritesheet, glm::vec2{ 2, 2 }, Sprites::TileSize };
	spriteMap[spawner = spriteCounter++] = { spritesheet, glm::vec2{ 3, 2 }, Sprites::TileSize };

	spriteMap[redBox = spriteCounter++] = { spritesheet, glm::vec2{ 0, 0 }, Sprites::TileSize };
	spriteMap[greenBox = spriteCounter++] = { spritesheet, glm::vec2{ 1, 0 }, Sprites::TileSize };
	spriteMap[box = spriteCounter++] = { spritesheet, glm::vec2{ 2, 0 }, Sprites::TileSize };

	spriteMap[player_ship = spriteCounter++] = { spritesheet, glm::vec2{ 0, 15 }, Sprites::TileSize };
	spriteMap[asteroid_small = spriteCounter++] = { spritesheet, glm::vec2{ 0, 14 }, Sprites::TileSize };
	spriteMap[drone_mini = spriteCounter++] = { spritesheet, glm::vec2{ 0, 13 }, Sprites::TileSize };




	LOG_INFO("Sprite loadedCounter before animations: {}", spriteCounter);
	// Animated Sprites
	animPlayerUp = CreateAnimSprites(spritesheet, glm::vec2{ 0, 15 }, 1);
	animPlayerDown = CreateAnimSprites(spritesheet, glm::vec2{ 0, 15 }, 1);
	animPlayerLeft = CreateAnimSprites(spritesheet, glm::vec2{ 0, 15 }, 1);
	animPlayerRight = CreateAnimSprites(spritesheet, glm::vec2{ 0, 15 }, 1);

	animBossUp = CreateAnimSprites(spritesheet, glm::vec2{ 0, 14 }, 1);
	animBossDown = CreateAnimSprites(spritesheet, glm::vec2{ 0, 14 }, 1);
	animBossLeft = CreateAnimSprites(spritesheet, glm::vec2{ 0, 15 }, 1);
	animBossRight = CreateAnimSprites(spritesheet, glm::vec2{ 0, 14 }, 1);



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

