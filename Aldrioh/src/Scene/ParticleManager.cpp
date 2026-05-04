#include <pch.h>
#include "ParticleManager.h"

#include <Graphics/Renderer.h>
#include <Graphics/RenderQueue.h>

#include <Math/Math.h>
#include <Game/SpriteCollection.h>
#include <Game.h>

#include <Game/RenderDepth.h>


void ParticleManager::Emit(const ParticleTemplate& pt)
{
	for (int i = 0; i < pt.count; ++i)
	{
		Particle& p = particlePool[poolIndex];

		p.prevPosition = pt.startPos;
		p.position = pt.startPos;
		p.lifeRemaining = pt.life;
		p.beginSize = pt.beginSize;
		p.endSize = pt.endSize;
		p.beginColour = pt.beginColour;
		p.endColour = pt.endColour;
		p.rotation = Math::Random::linearFloat(pt.rotationRange.first, pt.rotationRange.second);
		p.velocity = pt.velocity + glm::vec2{ pt.velocityVariation.x * Math::Random::linearFloat(-1, 1), pt.velocityVariation.y * Math::Random::linearFloat(-1, 1) };
		p.easingFunc = pt.easingFunc;

		p.active = true;
		poolIndex = (poolIndex + 1) % MAX_PARTICLES;
		++activeCount;
	}
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

		particle.prevPosition = particle.position;
		particle.position += particle.velocity * (float)ts;
		
		particle.lifeRemaining -= ts;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			--activeCount;
		}
	}
}

glm::vec4 interpolateWithFunc(const glm::vec4& p1, const glm::vec4& p2, float (*easingFunc)(float), float x)
{
	glm::vec4 result;
	float easingPercent = easingFunc(x);
	result.x = p1.x + (p2.x - p1.x) * easingPercent;
	result.y = p1.y + (p2.y - p1.y) * easingPercent;
	result.z = p1.z + (p2.z - p1.z) * easingPercent;
	result.w = p1.w + (p2.w - p1.w) * easingPercent;

	return result;
}

void ParticleManager::OnRender(Timestep ts)
{
	for (Particle& p : particlePool)
	{
		if (!p.active)
			continue;

		float percentLife = p.lifeRemaining / p.life;
		float size = glm::mix(p.endSize, p.beginSize, percentLife);
		glm::vec4 colour = interpolateWithFunc(p.beginColour, p.endColour, p.easingFunc, percentLife);
		p.rotation += p.rotation * Game::Instance().GetDelta();
		glm::vec2 pos = glm::mix(p.prevPosition, p.position, (float)ts) - size / 2.0f;

		//LOG_CORE_INFO("Particle colour {}", glm::to_string(colour));
		RenderQueue::EnQueue(RenderLayer::ONE, glm::vec3{ pos, 0.4f }, Sprites::square, colour, glm::vec2{ size, size }, p.rotation, RenderDepth::PARTICLE);
	}
}
