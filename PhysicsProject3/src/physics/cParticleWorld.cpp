#include <physics/cParticleWorld.h>
#include <algorithm>
//Needed?
//#include <iostream>
namespace nPhysics
{
	cParticleWorld::cParticleWorld(size_t maxContacts, size_t iterations) : mResolver(iterations)
	{
		mCalculateIterations = (iterations == 0);
		mForceRegistry = new cParticleForceRegistry();
		mContacts = new cParticleContact[maxContacts];
		mMaxContacts = maxContacts;
	}
	cParticleWorld::~cParticleWorld()
	{
		delete mForceRegistry;
		delete[] mContacts;
	}
	//TODO: Add stuff related to forceRegistry?
	bool cParticleWorld::AddParticle(cParticle* particle)
	{
		//Checks if there's a particle to be added
		if (!particle)
		{
			return false; // nothing was added
		}
		//Checks if we already have the particle.
		//    If no: Add it, return true to indicate that we added it.
		//    If yes: Return false, to indicate that nothing was added.
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			// Not there! add it.
			mParticles.push_back(particle);
			return true;
		}
		return false; // nothing was added.
	}

	bool cParticleWorld::RemoveParticle(cParticle* particle)
	{
		//Checks if there's a particle to be removed
		if (!particle)
		{
			return false; // nothing was removed
		}

		//Deregister the particle from all force generators associated with it
		mForceRegistry->Deregister(particle);

		//Check if we have the particle.
		//    If no: Return false to indicate that nothing was removed
		//    If yes: Removed it. Return true to indicate that it was removed
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			return false; // nothing was removed.
		}
		mParticles.erase(itParticle);
		return true; // found it. removed it.
	}

	size_t cParticleWorld::GenerateContacts()
	{
		size_t limit = mMaxContacts;
		cParticleContact* nextContact = mContacts;

		for (iParticleContactGenerator* g : mContactGenerators)
		{
			size_t used = g->AddContact(nextContact, limit, mParticles);
			limit -= used;
			nextContact += used;

			//If we run out of contacts to fill this means we are missing collisions
			if (limit <= 0)
			{
				break;
			}
		}
		return mMaxContacts - limit;
	}

	void cParticleWorld::TimeStep(float deltaTime)
	{
		// 1) Update Force Generators
		mForceRegistry->UpdateForces(deltaTime);

		// 2) Integrate the particles
		IntegrateParticles(deltaTime);
		// 3) Generate contacts
		size_t numContactsGenerated = GenerateContacts();

		// 4) Resolve contacts
		if (numContactsGenerated)
		{
			if (mCalculateIterations)
			{
				mResolver.SetIterations(numContactsGenerated * 2);
			}
			mResolver.ResolveContacts(mContacts, numContactsGenerated, deltaTime);
		}
	}

	void cParticleWorld::IntegrateParticles(float deltaTime)
	{
		// loop over our particles and tell them to integrate
		std::vector<cParticle*>::iterator itParticle = mParticles.begin();
		while (itParticle != mParticles.end())
		{
			(*itParticle)->Integrate(deltaTime);
			itParticle++;
		}
	}

	cParticleForceRegistry* cParticleWorld::GetForceRegistry() const
	{
		return mForceRegistry;
	}

	size_t cParticleWorld::NumParticles()
	{
		return mParticles.size();
	}
	
	bool cParticleWorld::AddContactContactGenerator(iParticleContactGenerator* generator)
	{
		//Check if there's a generator to add
		if (!generator)
		{
			return false;
		}
		//Check if the generator is already in the list
		std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(mContactGenerators.begin(), mContactGenerators.end(), generator);
		if (itGenerator == mContactGenerators.end())
		{
			//not there. Add it
			mContactGenerators.push_back(generator);
			return true;
		}
		//nothing was added
		return false;
	}
	
	bool cParticleWorld::RemoveContactContactGenerator(iParticleContactGenerator* generator)
	{	
		//Check if there's a generator to be removed
		if (!generator)
		{
			return false;
		}
		//Check if the generator is in the list
		std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(mContactGenerators.begin(), mContactGenerators.end(), generator);
		if (itGenerator == mContactGenerators.end())
		{
			//nothing was removed
			return false; 
		}
		//Generator was found and removed
		mContactGenerators.erase(itGenerator);
		return true;
	}
}