#pragma once
class Scene;

namespace EntitySystem {


	void UIManagerUpdateSystem(Timestep ts, Scene& scene);
	void UIManagerRenderSystem(Timestep ts, Scene& scene);

}