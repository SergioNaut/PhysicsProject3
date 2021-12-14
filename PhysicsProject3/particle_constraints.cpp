#include "particle_constraints.h"

//TODO: MODIFY/ADD ROPE
namespace nPhysics
{
	cParticleConstraint::cParticleConstraint(cParticle* particleA, cParticle* particleB)
		: mParticleA(particleA)
		, mParticleB(particleB)
	{}

	cParticleConstraint::~cParticleConstraint() {}

	float cParticleConstraint::CurrentSeparation() const
	{
		return glm::distance(mParticleA->GetPosition(), mParticleB->GetPosition());
	}

	cParticle* cParticleConstraint::GetParticleA()
	{
		return mParticleA;
	}

	cParticle* cParticleConstraint::GetParticleB()
	{
		return mParticleB;
	}

	cParticleRodConstraint::cParticleRodConstraint(cParticle* particleA, cParticle* particleB)
		: cParticleConstraint(particleA, particleB)
	{
		mLength = CurrentSeparation();
	}

	size_t cParticleRodConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		// Find the current length of the rod
		float currentLen = CurrentSeparation();

		// Check if we're over-extended
		if (currentLen == mLength)
		{
			return 0;
		}

		// Otherwise return the contact
		contact->mParticle0 = mParticleA;
		contact->mParticle1 = mParticleB;

		// Calculate the normal
		glm::vec3 normal = glm::normalize(mParticleB->GetPosition() - mParticleA->GetPosition());

		// The contact normal depends on if we are extending or compressing
		if (currentLen > mLength)
		{
			contact->mContactNormal = normal;
			contact->mPenetration = currentLen - mLength;
		}
		else
		{
			contact->mContactNormal = normal * -1.0f;
			contact->mPenetration = mLength - currentLen;
		}

		// zero restitution = no bouncinesss.
		contact->mRestitution = 0.0f;

		return 1;
	}
}