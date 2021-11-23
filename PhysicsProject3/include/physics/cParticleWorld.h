#pragma once
#include <physics/cParticle.h>
#include <physics/iParticleContactGenerator.h>
#include <physics/cParticleContactResolver.h>
#include <physics/cParticleContact.h>
#include <physics/cParticleForceRegistry.h>

#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
		
		//List of contacts
		cParticleContact* mContacts;

		//Maximum number of contacts allowed
		size_t mMaxContacts;

		//Vector with all contact generators
		std::vector<iParticleContactGenerator*> mContactGenerators;

		//Particle contact resolver
		cParticleContactResolver mResolver;

		//Boolean telling The World to calculate the number of interations to tell the contact resolver each frame
		bool mCalculateIterations;

		//ForceRegistry
		cParticleForceRegistry* mForceRegistry;

	public:
		//Original Constructor
		//cParticleWorld();
		cParticleWorld(size_t maxContacts, size_t iterations = 0);
		virtual ~cParticleWorld();
		cParticleWorld() = delete;
		cParticleWorld(cParticleWorld& other) = delete;
		cParticleWorld& operator=(cParticleWorld& other) = delete;

		//Particles
		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);
		void TimeStep(float deltaTime);
		void IntegrateParticles(float deltaTime);
		size_t NumParticles();
		
		//Contacts
		size_t GenerateContacts();
		//Rename AddContactToContactGenerator?
		bool AddContactContactGenerator(iParticleContactGenerator* generator);
		bool RemoveContactContactGenerator(iParticleContactGenerator* generator);

		//Force Registry
		cParticleForceRegistry* GetForceRegistry() const;
	};
}

