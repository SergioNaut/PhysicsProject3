#include <physics/particle_contact_generators.h>

namespace nPhysics
{
	//TODO: add stuff for correct particle contact generator
	cPlaneParticleContactGenerator::cPlaneParticleContactGenerator(glm::vec3 nNormal,float dDistance, float sSphereRadius)
	{
		mNormal = nNormal;
		distance = dDistance;
		sphereRadius = sSphereRadius;
	}
	cPlaneParticleContactGenerator::~cPlaneParticleContactGenerator()
	{

	}

	size_t cPlaneParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		for (std::vector<cParticle*>::iterator p = particles.begin(); p != particles.end(); p++)
		{
			//TODO: All this stuff
			//Only relevant to the cGround...generator
			//float y = (*p)->GetPosition().y;
			glm::vec3 particlePosition = (*p)->GetPosition();
			
			//distance = glm::dot(particlePosition,mNormal);
			float particleDistance = glm::dot(particlePosition, glm::normalize(mNormal))-distance;

			//I think that's correct
			//if (distance <= sphereRadius)
			if (particleDistance <= sphereRadius)
			{
				//I think that's correct
				contact->mContactNormal = mNormal;
				contact->mParticle0 = *p;
				contact->mParticle1 = nullptr;
				//I think that's correct
				//contact->mPenetration = sphereRadius - distance /*particleDistance*/;
				contact->mPenetration = sphereRadius - particleDistance;
				//Generic restitution for now
				contact->mRestitution = 0.7f;
				contact++;
				numContactsCreated++;
			}
			if (numContactsCreated >= limit)
			{
				return numContactsCreated;
			}
		}
		return numContactsCreated;
	}
}