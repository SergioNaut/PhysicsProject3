#include <physics/cParticleContact.h>

namespace nPhysics
{
	void cParticleContact::ResolvePosition(float deltaTime)
	{
		//If there`s no penetration, finish this method
		if (mPenetration <= 0)
		{
			return;
		}

		//Getting the total inverse mass, since the movement of each object is based on their inverse mass
		float totalInverseMass = mParticle0->GetInverseMass();
		if (mParticle1)
		{
			totalInverseMass += mParticle1->GetInverseMass();
		}
		//If all particles have infinite mass, we do nothing
		if (totalInverseMass <= 0)
		{
			glm::set(mMovement0, 0.0f, 0.0f, 0.0f);
			glm::set(mMovement1, 0.0f, 0.0f, 0.0f);
			return;
		}

		//Find the amount of mPenetration resolution per unit of Inverse Mass
		glm::vec3 movePerInverseMass = mContactNormal * (mPenetration / totalInverseMass);

		//Calculate the movement amount 
		mMovement0 = movePerInverseMass * mParticle0->GetInverseMass();
		if (mParticle1)
		{
			mMovement1 = movePerInverseMass * -mParticle1->GetInverseMass();
		}
		else
		{
			glm::set(mMovement1, 0.0f, 0.0f, 0.0f);
		}

		//Apply the penetration resolution
		mParticle0->SetPosition(mParticle0->GetPosition() + mMovement0);
		if (mParticle1)
		{
			mParticle1->SetPosition(mParticle1->GetPosition() + mMovement1);
		}
	}

	float cParticleContact::CalculateSeparatingVelocity() const
	{
		glm::vec3 relativeVelocity = mParticle0->GetVelocity();
		if (mParticle1)
		{
			relativeVelocity -= mParticle1->GetVelocity();
		}
		return glm::dot(relativeVelocity, mContactNormal);
	}

	void cParticleContact::ResolveVelocity(float deltaTime)
	{
		//Find the velocity in the direction of the contact
		float separatingVelocity = CalculateSeparatingVelocity();

		if (separatingVelocity > 0.0f)
		{
			//The contact is either separatinh or stationary, so no impulse is required
			return;
		}

		//Calculate the new separating velocity
		float newSeparatingVelocity = -separatingVelocity * mRestitution;

		//Check the velocity build-up due to acceleration only
		glm::vec3 accelerationCausedVelocity = mParticle0->GetAcceleration();
		if (mParticle1)
		{
				accelerationCausedVelocity -= mParticle1->GetAcceleration();
		}
		float accelerationCausedSeparationVelocity = glm::dot(accelerationCausedVelocity, mContactNormal) * deltaTime;
		
		//If we have a closing velocity due to acceleration build-up, remove it from the new separating velocity
		if (accelerationCausedSeparationVelocity < 0)
		{
			newSeparatingVelocity += mRestitution * accelerationCausedSeparationVelocity;

			//Make sure we haven't removed more than there was to remove
			if (newSeparatingVelocity < 0.0f)
			{
				newSeparatingVelocity = 0.0f;
			}
		}
		float deltaVelocity = newSeparatingVelocity - separatingVelocity;

		//We apply the change in velocity to each object in proportion to their inverse mass
		//higher mass = less change in velocity
		float totalInverseMass = mParticle0->GetInverseMass();
		if (mParticle1)
		{
			totalInverseMass += mParticle1->GetInverseMass();
		}
		//If all particles have infinite mass, then impulses have no effect
		if (totalInverseMass <= 0.0f)
		{
			return;
		}
		//Calculate impulses to apply
		float impulse = deltaVelocity / totalInverseMass;

		//Find the amount of impulse per unit of inverse mass
		glm::vec3 impulsePerInverseMass = mContactNormal * impulse;

		//Apply impulses
		//They are applied in the direction of the contact, and they are proportional to the inverse mass
		mParticle0->SetVelocity(mParticle0->GetVelocity() + impulsePerInverseMass * (mParticle0->GetInverseMass()));

		if (mParticle1)
		{
			//Particle1 goes in the opposite direction
			mParticle1->SetVelocity(mParticle1->GetVelocity() + (impulsePerInverseMass * -mParticle1->GetInverseMass()));
		}
	}

	void cParticleContact::Resolve(float deltaTime)
	{
		ResolveVelocity(deltaTime);
		ResolvePosition(deltaTime);
	}
}