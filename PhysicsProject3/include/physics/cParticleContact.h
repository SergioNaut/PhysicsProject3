#pragma once
#include <physics/cParticle.h>

namespace nPhysics
{
	class cParticleContact
	{
	public:
		//The first particle involved in the contact. Could be the only one
		cParticle* mParticle0;
		//The second particle involved in the contact. May be null
		cParticle* mParticle1;
		//The restitution coefficient at the contact
		float mRestitution;
		//The direction on the contact in world coordinates
		glm::vec3 mContactNormal;
		//The depth of penetration at the contact
		float mPenetration;

		//Interpenetration resolution
		//Amount that mParticle0 is moved by during interpenetration resolution
		//Do not set this when creating the contact
		glm::vec3 mMovement0;

		//Amount that mParticle0 is moved by during interpenetration resolution
		//May be null?
		//Do not set this when creating the contact
		glm::vec3 mMovement1;

		//Resolves contact, both for velocity and interpenetration
		void Resolve(float deltaTime);
		//Calculates the separating velocity at this contact
		float CalculateSeparatingVelocity() const;
		//Handles impulse calculation for this collision
		void ResolveVelocity(float deltaTime);
		//Handles interpenetration resolution for this collision
		void ResolvePosition(float deltaTime);
	};
}