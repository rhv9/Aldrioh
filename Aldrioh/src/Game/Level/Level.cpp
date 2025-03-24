#include "pch.h"
#include "Level.h"

#include "Game/SpriteCollection.h"
#include "Graphics/Renderer.h"

#include "Math/Math.h"

Level::Level()
{
	width = 20;
	height = 20;
	world = new int[width * height];

	for (int i = 0; i < width * height; i++)
	{
		world[i] = Math::Random::linearInt(1, 3);
	}
}

Level::~Level()
{
	delete world;
}

void Level::OnUpdate(Timestep delta)
{
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tile = world[y * width + x];
			glm::vec3 renderPos = { x * 16, y * 16, 0.0f };
			//LOG_CORE_INFO("x:{}y:{}", renderPos.x, renderPos.y);
			//renderPos = renderPos - moveVec;
			Renderer::DrawQuad(renderPos, SpriteCollection::get(tile), SpriteCollection::Tile_size);
		}

	}

}