#pragma once

class Scene;
struct Entity;

namespace EntitySystem {

	glm::vec2 CalculateEntityTransformWithInterpolation(Entity entity, Timestep ts);

	void EntityRenderSystem(Timestep ts, Scene& scene);
	void CollisionRenderSystem(Timestep ts, Scene& scene);

}