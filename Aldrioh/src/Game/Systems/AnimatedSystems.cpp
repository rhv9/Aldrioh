#include "pch.h"
#include "AnimatedSystems.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>

void EntitySystem::AnimatedMovementSystem(Timestep ts, Scene& scene)
{
	// AnimatedMovementComponent
	{
		auto view = scene.getRegistry().view<AnimatedMovementComponent, MoveComponent, VisualComponent>();

		for (entt::entity e : view)
		{
			auto [amc, mc, vc] = view.get(e);

			if (mc.dir == MoveDir::NONE)
			{
				amc.reset();
				vc.spriteId = amc.getCurrentSprite();
			}
			else
			{
				if (amc.currentDir != mc.dir)
				{
					amc.currentDir = mc.dir;
					amc.reset();
				}
				amc.update(ts);
				vc.spriteId = amc.getCurrentSprite();

			}
		}
	}

}
