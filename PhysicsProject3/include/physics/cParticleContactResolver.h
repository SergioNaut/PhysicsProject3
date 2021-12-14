#pragma once
#include <physics/cParticleContact.h>

namespace nPhysics
{
	//Contact resolution routine for particle contacts
	//One resolver instance is generally shared for the entire program

	class cParticleContactResolver
	{
	public:
		//Holds the number of interactions allowed
		size_t mIterations;
		//Used to keep a record of the number of iterations
		//Used for performance reasons
		size_t mIterationsUsed;

		//Creates a new contact resolver
		//Figure out good number for iterations
		cParticleContactResolver(size_t iterations);
		//Sets the number of iterations that can be used
		void SetIterations(size_t iterations);

		//Resolves a set of particle contacts for both penetration and velocity
		void ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime);
	};
}