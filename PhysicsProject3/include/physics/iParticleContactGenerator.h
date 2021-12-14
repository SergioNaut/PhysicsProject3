#pragma once
#include "cParticleContact.h"

namespace nPhysics
{
	//Polymorphic interface for contact generators that apply to particles
	class iParticleContactGenerator
	{
	public:
		virtual ~iParticleContactGenerator(){}
		//Fills the contact structure with the generated contacts
		//The contact pointer should point to the first available contact in the contact array,
		//Where the limit is the maximum number of contacts in the array that can be written to.
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const = 0;
		//Returns number of contacts generated

	};
}