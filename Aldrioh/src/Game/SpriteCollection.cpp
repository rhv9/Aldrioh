#include "pch.h"
#include "Graphics/SubTexture.h"

static std::shared_ptr<Texture> spritesheet;
static std::shared_ptr<Texture> squareBox;

static SubTexture* spriteMap;
static constexpr uint32_t size = 256;
static int spriteCounter = 0;

#define SPRITES_DECLARATION
#include "SpriteCollection.h"

#define SPRITES_CPP
#include "SpriteCollection.h"

std::vector<spriteid_t> CreateAnimSprites(const std::shared_ptr<Texture>& spriteSheet, const glm::vec2 pos, int animationLength)
{
	std::vector<int> sprites;
	sprites.reserve(animationLength);

	for (int i = 0; i < animationLength; ++i)
	{
		spriteid_t spriteId = spriteCounter++;
		spriteMap[spriteId] = {spriteSheet, pos + glm::vec2{i, 0.0f}, Sprites::TileSize};
		sprites.push_back(spriteId);
	}

	return sprites;
}

void Sprites::Init()
{
	
	spriteMap = new SubTexture[size];
	spritesheet = std::make_shared<Texture>("assets/textures/shooter_spritesheet.png");

	RunMonsterCodeThatInitialisesSpritesFromHeaderFileUsingVeryReadableMacros();


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

}

SubTexture* Sprites::get(spriteid_t index)
{
	return &spriteMap[index];
}

