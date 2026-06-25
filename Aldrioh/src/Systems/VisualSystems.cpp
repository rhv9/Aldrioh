#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "VisualSystems.h"


void EntitySystem::AnimatedSpriteSystem(Timestep ts, Scene& scene)
{
	static float timeElapsed = 0.0f;

	timeElapsed += (float)ts;
	if (timeElapsed >= 0.25f)
	{
		timeElapsed -= 0.25f;

		auto view = scene.getRegistry().view<VisualComponent, AnimateVisualComponent>();
		for (entt::entity entityHandle : view)
		{
			auto [vc, avc] = view.get<VisualComponent, AnimateVisualComponent>(entityHandle);
			
			if (++avc.frame >= avc.animSprite.GetFrameCount())
				avc.frame = 0;

			vc.spriteId = avc.animSprite.GetFrameUnsafe(avc.frame);
		}

	}

}
