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
		Path& path = pc.path;
		
		const glm::vec2& cp = pc.currentPosition;
		pc.prevPosition = cp;
		const glm::vec2& p1 = path.points[pc.currentPathIndex];
		const glm::vec2& p2 = path.points[pc.currentPathIndex + 1];
		float speed = path.pathConfigs[pc.currentPathIndex].speed;
		float h = speed * ts;

		// calculate gradient
		float m; 
		float dy = p2.y - p1.y;
		float dx = p2.x - p1.x;
		if (dx == 0) m = 0;
		else m = dy / dx;

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

	for (entt::entity e : view)
	{
		auto [tc, pc] = view.get<TransformComponent, PathComponent>(e);
		if (pc.currentPathIndex >= pc.path.maxPaths - 1)
		{
			scene.WrapEntityHandle(e).QueueDestroy();
			continue;
		}
		Path& path = pc.path;
		const glm::vec2& cp = pc.currentPosition;
		const glm::vec2& p1 = path.points[pc.currentPathIndex];
		const glm::vec2& p2 = path.points[pc.currentPathIndex + 1];
		float dy = p2.y - p1.y;
		float dx = p2.x - p1.x;
		
		if (!(cp.x >= Math::min(p1.x, p2.x) && cp.x <= Math::max(p1.x, p2.x) && cp.y >= Math::min(p1.y, p2.y) && cp.y <= Math::max(p1.y, p2.y)))
		{
			LOG_CORE_INFO("Next path!, cp: {}, p1: {}, p2: {}", glm::to_string(cp), glm::to_string(p1), glm::to_string(p2));
			pc.currentPathIndex++;
			pc.currentPosition = p2;
			continue;
		}
	}

	for (entt::entity e : view)
	{
		auto [tc, pc] = view.get<TransformComponent, PathComponent>(e);
		tc.position.x = pc.currentPosition.x;
		tc.position.y = pc.currentPosition.y;
	}
}
