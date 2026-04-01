#include "pch.h"
#include "AnimatedSystems.h"
#include <Systems/HeadersUpdateSystems.h>

void EntitySystem::AnimatedMovementSystem(Timestep ts, Scene& scene)
{
	LOG_CORE_INFO("Deprecated for now");
	// AnimatedMovementComponent
	{
		//auto view = scene.getRegistry().view<AnimatedMovementComponent, MoveControllerComponent, VisualComponent>();

		//for (entt::entity e : view)
		//{
		//	auto [amc, mcc, vc] = view.get(e);
		//
		//	if (mc.dir == MoveDir::NONE)
		//	{
		//		amc.reset();
		//		vc.spriteId = amc.getCurrentSprite();
		//	}
		//	else
		//	{
		//		if (amc.currentDir != mc.dir)
		//		{
		//			amc.currentDir = mc.dir;
		//			amc.reset();
		//		}
		//		amc.update(ts);
		//		vc.spriteId = amc.getCurrentSprite();
		//
		//	}
		//}
	}

}
