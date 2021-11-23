#pragma once

#include "cParticle.h"
#include "iParticleForceGenerator.h"

#include <map>
#include <vector>

namespace nPhysics
{
	class cParticleForceRegistry
	{
		//Typedefs for using maps
		typedef std::multimap<cParticle*, iParticleForceGenerator*> registryType;
		typedef std::multimap<cParticle*, iParticleForceGenerator*>::iterator registryIterator;
		typedef std::pair<cParticle*, iParticleForceGenerator*> registryPair;
		typedef std::pair<registryIterator, registryIterator> registryRange;

		registryType mRegistry;
	public:
		cParticleForceRegistry();
		~cParticleForceRegistry();

		void Register(cParticle* particle, iParticleForceGenerator* generator);
		void Deregister(cParticle* particle);
		void Deregister(cParticle* particle, iParticleForceGenerator* generator);
		void Deregister(iParticleForceGenerator* generator);
		void UpdateForces(float deltaTime);

		cParticleForceRegistry(cParticleForceRegistry& other) = delete;
		cParticleForceRegistry& operator =(cParticleForceRegistry& other) = delete;


	};
}