#pragma once

class Scene;
struct Entity;

namespace EntitySystem {

	void EntityRenderSystem(Timestep ts, Scene& scene);
	void CollisionRenderSystem(Timestep ts, Scene& scene);

}