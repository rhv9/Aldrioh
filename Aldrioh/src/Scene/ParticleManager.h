#pragma once
#include <Math/Math.h>
#include <Game/RenderDepth.h>
#include <Graphics/RenderLayer.h>
#include <Graphics/RenderFlags.h>

struct SubTexture;

struct ParticleTemplate
{
	glm::vec2 startPos{ 0.0 };
	glm::vec2 velocity{ 0.0f }, velocityVariation{ 0.0f };
	glm::vec2 acceleration{ 0.0f };
	glm::vec4 beginColour, endColour;
	float beginSize = 1, endSize = 1;
	float life = 1.0f;
	std::pair<float, float> rotationRange{ 0.0f, 0.0f };
	float (*colourEasingFunc)(float) = Math::EasingFunction::easeInQuint;
	SubTexture* subTexture = nullptr;
	RenderLayer renderLayer = GameRenderLayers::PARTICLES;
	renderFlag_t renderFlag = RenderFlag::OVERWRITE_COLOUR;
	std::span<SubTexture*> subTexturePool;

	int count = 1;
};

struct Particle
{
	glm::vec2 prevPosition, position;
	glm::vec2 velocity, velocityVariation;
	glm::vec2 acceleration;
	glm::vec4 beginColour, endColour;
	float beginSize, endSize;
	float life = 1.0f;
	float rotation = 0.0f;
	float (*colourEasingFunc)(float);
	SubTexture* subTexture;
	RenderLayer renderLayer;
	renderFlag_t renderFlag;

	float lifeRemaining = 1.0f;
	bool active = false;
};


class ParticleManager
{
public:
	ParticleManager();

	void OnUpdate(Timestep ts);
	void OnRender(Timestep ts);

	void Emit(const ParticleTemplate& particleTemplate);

	int GetActiveCount() { return activeCount; }

private:
	static constexpr uint32_t MAX_PARTICLES = 1000;
	std::array<Particle, MAX_PARTICLES> particlePool;
	int poolIndex = 0;
	int activeCount = 0;

};