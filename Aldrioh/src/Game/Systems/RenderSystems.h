#pragma once

class Scene;

namespace EntitySystem {

	void EntityRenderSystem(Timestep ts, Scene& scene);
	void CollisionRenderSystem(Timestep ts, Scene& scene);

}