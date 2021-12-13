#pragma once

#include <extern_includes.h>

namespace nPhysics
{
	class cParticle
	{
	private:
		// Position of the particle in the world
		glm::vec3 mPosition;
		// Velocities of the particle in the world
		glm::vec3 mVelocity;
		// Acceleration of the particle in the world
		glm::vec3 mAcceleration;
		// Inverse mass
		float mInverseMass;
		// Externally applied forces accumulated during a time step
		glm::vec3 mAppliedForce;
		
		float mDamping;

		//Use mIsAlive?
		bool mIsAlive;

		float sphereRadius;

		//TODO: SetSphereRadius for particles
	public:

		float GetRadius();

		cParticle(float mass, const glm::vec3& position);
		virtual ~cParticle();

		cParticle() = delete;
		cParticle(cParticle& other) = delete;
		cParticle& operator=(cParticle& other) = delete;

		void SetDamping(float damping);
		float GetDamping() const;

		//Added to change mass after particle was created
		void SetMass(float newMass);
		float GetMass() const;
		float GetInverseMass() const;

		bool IsStaticObject() const;

		glm::vec3 GetPosition() const;
		void GetPosition(glm::vec3& position);
		void SetPosition(const glm::vec3& position);

		glm::vec3 GetVelocity() const;
		void GetVelocity(glm::vec3& velocity);
		void SetVelocity(const glm::vec3& velocity);

		glm::vec3 GetAcceleration() const;
		void GetAcceleration(glm::vec3& acceleration);
		void SetAcceleration(const glm::vec3& acceleration);


		void ApplyForce(const glm::vec3& force);

		//Added Impulse
		void ApplyImpulse(const glm::vec3& impulse);

		void ClearAppliedForces();

		void Integrate(float deltaTime);

		//Added isAlive Helpers
		void SetIsAlive(bool isAlive);
		bool GetIsAlive() const;

	};
}