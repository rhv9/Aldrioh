#pragma once

class Scene;

namespace EntitySystem {

	void ResetMovementSystem(Timestep ts, Scene& scene);
	void MovementSystem(Timestep ts, Scene& scene);
	
	void JumpSystem(Timestep ts, Scene& scene);

	void LifeSystem(Timestep ts, Scene& scene);
}