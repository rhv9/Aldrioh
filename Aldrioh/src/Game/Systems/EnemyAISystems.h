#pragma once

class Scene;

namespace EntitySystem {

	void DumbAISystem(Timestep ts, Scene& scene);
	void FollowPlayerAISystem(Timestep ts, Scene& scene);
}