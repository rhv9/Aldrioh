#include "pch.h"
#include "TestLevel.h"
#include <Game/SpriteCollection.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <Graphics/Renderer.h>
#include <Game/RenderDepth.h>

#include <Game/Tiles/TexturedTiles.h>
#include <Game/Tiles/SpawnerTile.h>

#include <Components/Collision.h>

class Region
{

};

class StaticRegion : public Region
{

};

class ObjectiveRegion : public Region
{

};

TestLevel::TestLevel(Scene& scene) : scene(scene)
{
	width = 40;
	height = 200;

	tiles = new Tiles * [width * height];

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			tiles[y * width + x] = new TexturedTiles(Sprites::sand_1);
		}
	}

	tiles[15 * width + 7] = new SpawnerTile(Sprites::sand_1, EntityType::Enemy);
}

TestLevel::~TestLevel()
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			delete tiles[y * width + x];
		}
	}
	delete[]tiles;
}

void TestLevel::OnUpdate(Timestep ts)
{
	auto& cameraController = scene.GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

	auto& bounds = cameraController->GetBounds();
	const glm::vec2& cameraPos = cameraController->GetPosition();
	int startX = (int)std::max(-bounds.Right + cameraPos.x, 0.0f);
	int startY = (int)std::max(-bounds.Top + cameraPos.y, 0.0f);
	int endX = (int)std::min(bounds.Right + cameraPos.x + 1, (float)width);
	int endY = (int)std::min(bounds.Top + cameraPos.y + 1, (float)height);

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			TileMetaData metadata{ {x, y}, this };
			tiles[y * width + x]->OnUpdate(ts, metadata);
		}
	}
}

void TestLevel::OnRender(Timestep ts)
{
	auto& cameraController = scene.GetPrimaryCameraEntity().GetComponent<CameraComponent>().cameraController;

	auto& bounds = cameraController->GetBounds();
	const glm::vec2& cameraPos = cameraController->GetPosition();
	int startX = (int)std::max(-bounds.Right + cameraPos.x, 0.0f);
	int startY = (int)std::max(-bounds.Top + cameraPos.y, 0.0f);
	int endX = (int)std::min(bounds.Right + cameraPos.x + 1, (float)width);
	int endY = (int)std::min(bounds.Top + cameraPos.y + 1, (float)height);

	//startX++;
	//startY++;
	//endX--;
	//endY--;

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			TileMetaData metadata{ {x, y}, this };
			tiles[y * width + x]->OnRender(ts, metadata);
		}
	}
}

void TestLevel::SetTile(int x, int y, Tiles* tile)
{
	delete tiles[y * width + x];
	tiles[y * width + x] = tile;
}

void TestLevel::CreateBoss(const glm::vec2& pos)
{
	// Create boss
	Entity boss = scene.CreateEntity("Boss");
	boss.GetComponent<TransformComponent>().position = { pos.x, pos.y, 0.4f };
	boss.AddComponent<VisualComponent>(Sprites::player_head).localTransform = { -0.5f, -0.5f, 0.0f };
	boss.AddComponent<MoveComponent>(1.0f);
	boss.AddComponent<EntityTypeComponent>(EntityType::Enemy);
	boss.AddComponent<AnimatedMovementComponent>(Sprites::animBossUp, Sprites::animBossDown, Sprites::animBossLeft, Sprites::animBossRight, 0.1f);
	boss.AddComponent<CollisionBox>(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec2{ 1.0f, 1.0f });
	boss.AddComponent<DumbAIComponent>();
}

