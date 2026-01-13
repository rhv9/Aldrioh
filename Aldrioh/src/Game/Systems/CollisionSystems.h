#pragma once
class Scene;

namespace EntitySystem {
	void CollisionSystem(Timestep ts, Scene& scene);
	void ResetAndAddCollisionWorld(Timestep ts, Scene& scene);
}