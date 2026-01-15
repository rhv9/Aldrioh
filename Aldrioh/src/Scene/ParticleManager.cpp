#include <pch.h>
#include "ParticleManager.h"

#include <Graphics/Renderer.h>
#include <Graphics/RenderQueue.h>

#include <Game/SpriteCollection.h>

void ParticleManager::Emit(const ParticleTemplate& particleTemplate)
{
	Particle& particle = particlePool[poolIndex];

	particle.position = particleTemplate.startPos;
	particle.lifeRemaining = particleTemplate.life;
	particle.beginSize = particleTemplate.beginSize;
	particle.endSize = particleTemplate.endSize;
	particle.beginColour = particleTemplate.beginColour;
	particle.endColour = particle.endColour;
	particle.rotation = particleTemplate.rotation;
	particle.velocity = particleTemplate.velocity;
	particle.active = true;
	poolIndex = (poolIndex + 1) % MAX_PARTICLES;
}

ParticleManager::ParticleManager()
{
	for (Particle& particle : particlePool)
	{
		particle.active = false;
	}
}

void ParticleManager::OnUpdate(Timestep ts)
{
	//LOG_CORE_INFO("Pool index: {}", poolIndex);
	for (Particle& particle : particlePool)
	{
		if (!particle.active)
			continue;

		LOG_CORE_INFO("Particle postion {}", glm::to_string(particle.position));
		particle.position += particle.velocity * (float)ts;

		particle.lifeRemaining -= ts;

		if (particle.lifeRemaining <= 0.0f)
			particle.active = false;
	}
}

void ParticleManager::OnRender(Timestep ts)
{
	for (Particle& particle : particlePool)
	{
		if (!particle.active)
			continue;

		float size = particle.beginSize;
		glm::vec4 colour = particle.beginColour;
		float rotation = particle.rotation;

		//LOG_CORE_INFO("Particle postion {}", glm::to_string(particle.position));

		RenderQueue::EnQueue(RenderLayer::TWO, glm::vec3{ particle.position, 0.8f }, Sprites::drone_mini, colour, glm::vec2{size, size}, rotation, 1);
	}
}
