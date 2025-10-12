#pragma once

class Scene;

namespace EntitySystem {

	void LevelRenderSystem(Timestep ts, Scene& scene);
	void LevelUpdateSystem(Timestep ts, Scene& scene);

	void ScoreSystems(Timestep ts, Scene& scene);

}