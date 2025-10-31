#include <pch.h>
#include "HeadersUpdateSystems.h"
#include "SoundSystems.h"
#include <Audio/SoundManager.h>


void EntitySystem::SoundSystem(Timestep ts, Scene& scene)
{
	auto view = scene.getRegistry().view<SoundComponent>();

	for (entt::entity e : view)
	{
		SoundComponent& sc = view.get<SoundComponent>(e);
		SoundManager::Play(sc.soundName);
		scene.WrapEntityHandle(e).QueueDestroy();
	}

}
