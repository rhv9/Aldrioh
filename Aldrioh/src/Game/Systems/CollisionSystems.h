#pragma once
class Scene;

namespace EntitySystem {
	void CollisionSystem(Timestep ts, Scene& scene);
	void ResetAndAddCollisionZone(Timestep ts, Scene& scene);

	void DebugRenderCollisionZoneVisualisation(Timestep ts, Scene& scene);
}