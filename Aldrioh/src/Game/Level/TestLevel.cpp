#include "pch.h"
#include "TestLevel.h"
#include <Game/SpriteCollection.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <Graphics/Renderer.h>
#include <Game/RenderDepth.h>

#include <Game/Tiles/TexturedTiles.h>

TestLevel::TestLevel(Scene& scene) : scene(scene)
{
	width = 14;
	height = 200;

	tiles = new Tiles * [width * height];

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			tiles[y * width + x] = new TexturedTiles(Sprites::sand_1);
		}
	}
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

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			TileMetaData metadata{ {x, y}, this };
			tiles[y * width + x]->OnRender(ts, metadata);
		}
	}
}
