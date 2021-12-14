#pragma once
#include <extern_includes.h>
#include "iParticleForceGenerator.h"

namespace nPhysics
{
	//Class used for all different force generators
	class cParticleGravityGenerator : public iParticleForceGenerator
	{
	public:
		cParticleGravityGenerator(const glm::vec3& gravityAcceleration);
		cParticleGravityGenerator() = delete;
		cParticleGravityGenerator(cParticleGravityGenerator& other) = delete;
		cParticleGravityGenerator& operator=(cParticleGravityGenerator& other) = delete;

		virtual ~cParticleGravityGenerator();
		virtual void Update(cParticle* particle, float deltaTIme);
	private:
		glm::vec3 mGravityAcceleration;
	};
}
