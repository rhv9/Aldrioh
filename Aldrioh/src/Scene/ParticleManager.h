#pragma once


struct ParticleTemplate
{
	glm::vec2 startPos;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 beginColour, endColour;
	float beginSize, endSize;
	float life = 1.0f;
	float rotation = 0.0f;
};

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 beginColour, endColour;
	float beginSize, endSize;
	float life = 1.0f;
	float rotation = 0.0f;

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

private:
	static constexpr uint32_t MAX_PARTICLES = 1000;
	std::array<Particle, MAX_PARTICLES> particlePool;
	int poolIndex = 0;

};