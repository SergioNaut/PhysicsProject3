#include <physics/cParticleForceRegistry.h>

namespace nPhysics
{
	cParticleForceRegistry::cParticleForceRegistry(){}
	cParticleForceRegistry::~cParticleForceRegistry(){}

	void cParticleForceRegistry::Register(cParticle* particle, iParticleForceGenerator* generator)
	{
		mRegistry.insert(std::make_pair(particle, generator));
	}

#pragma region Deregister
	void cParticleForceRegistry::Deregister(cParticle* particle)
	{
		mRegistry.erase(particle);
	}

	void cParticleForceRegistry::Deregister(cParticle* particle, iParticleForceGenerator* generator)
	{
		for (registryIterator it = mRegistry.begin(); it != mRegistry.end();)
		{
			registryIterator eraseIterator = it++;
			if (eraseIterator->second == generator)
			{
				mRegistry.erase(eraseIterator);
			}
		}
	}

	void cParticleForceRegistry::Deregister(iParticleForceGenerator* generator)
	{
		for (registryIterator it = mRegistry.begin(); it != mRegistry.end();)
		{
			registryIterator eraseIterator = it++;
			if (eraseIterator->second == generator)
			{
				mRegistry.erase(eraseIterator);
			}
		}
	}
#pragma endregion
	void cParticleForceRegistry::UpdateForces(float deltaTime)
	{
		for (registryIterator it = mRegistry.begin(); it != mRegistry.end(); it++)
		{
			it->second->Update(it->first, deltaTime);
		}
	}
}