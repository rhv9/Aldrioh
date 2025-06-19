#include "pch.h"
#include "EnemyAISystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

void EntitySystem::DumbAISystem(Timestep ts, Scene& scene)
{
	// DumbAIComponent
	auto view = scene.getRegistry().view<DumbAIComponent, TransformComponent, MoveComponent>();
	auto& player_mc = scene.GetPlayer()->GetComponent<TransformComponent>();
	for (entt::entity e : view)
	{
		auto [tc, mc] = view.get<TransformComponent, MoveComponent>(e);

		glm::vec2 diff{ -tc.position.x + player_mc.position.x, -tc.position.y + player_mc.position.y };
		diff = glm::normalize(diff);

		mc.updateMoveVec(diff);
	}
}
