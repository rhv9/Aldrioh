#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "PathSystems.h"
#include <Components/Path/PathComponent.h>
#include <Math/Math.h>

void EntitySystem::PathsSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<TransformComponent, PathComponent>();

	for (entt::entity e : view)
	{
		auto [tc, pc] = view.get<TransformComponent, PathComponent>(e);
		tc.position.x = pc.currentPosition.x;
		tc.position.y = pc.currentPosition.y;
	}

	for (entt::entity e : view)
	{
		auto [tc, pc] = view.get<TransformComponent, PathComponent>(e);
		Path& path = pc.path;
		
		const glm::vec2& p1 = path.points[0];
		const glm::vec2& p2 = path.points[1];
		float speed = path.pathConfigs[0].speed;
		float h = speed * ts;

		// calculate gradient
		float m; 
		float dy = p2.y - p1.y;
		float dx = p2.x - p1.x;
		if (dx == 0) m = 0;
		else m = dy / dx;

		const glm::vec2& cp = pc.currentPosition;
		float yn = 0, xn = 0;

		if (dy == 0)
		{
			yn = cp.y;
			xn = cp.x + Math::sign(p2.x - p1.x) * h;
		}
		if (dx == 0)
		{
			xn = cp.x;
			yn = cp.y + Math::sign(p2.y - p1.y) * h;
		}
		else
		{
			float angle = Math::arcTan(m);
			if (p2.x < p1.x)
				angle -= Math::PI;

			yn = Math::sinRad(angle) * h + cp.y;
			xn = Math::cosRad(angle) * h + cp.x;
		}

		//LOG_CORE_INFO("cp {}, new {}", glm::to_string(cp), glm::to_string(glm::vec2{ xn, yn }));
		pc.currentPosition = { xn, yn };
	}
}
