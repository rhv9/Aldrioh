#include "pch.h"
#include "TestLevel.h"
#include <Game/SpriteCollection.h>
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <Graphics/Renderer.h>
#include <Game/RenderDepth.h>

TestLevel::TestLevel(Scene& scene) : scene(scene)
{
	width = 14;
	height = 200;

	tiles = new int[width * height];

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			tiles[y * width + x] = Sprites::sand_1;
		}
	}
}

TestLevel::~TestLevel()
{
	delete[] tiles;
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
			int tile = tiles[y * width + x];
			glm::vec3 renderPos = { x * 1.0f, y * 1.0f, RenderDepth::TILE };
			Renderer::DrawQuad(renderPos, Sprites::get(tile), { 1, 1 });
		}
	}
}
