#pragma once
class Scene;

namespace EntitySystem {
	void CollisionSystem(Timestep ts, Scene& scene);
	void ResetAndAddCollisionWorld(Timestep ts, Scene& scene);
	void ResetAndAddCollisionZone(Timestep ts, Scene& scene);


	void DebugRenderCollisionWorldVisualisation(Timestep ts, Scene& scene);
}