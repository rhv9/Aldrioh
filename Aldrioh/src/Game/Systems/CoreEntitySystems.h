#pragma once

class Scene;

namespace EntitySystem {
	void CoreEntitySystems(Timestep ts, Scene& scene);
	void RotationSystem(Timestep ts, Scene& scene);
	void HealthSystem(Timestep ts, Scene& scene);

	void DeleteEnemyOutsideScreenSystem(Timestep ts, Scene& scene);
}