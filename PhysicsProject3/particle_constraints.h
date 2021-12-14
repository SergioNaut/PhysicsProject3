#pragma once
#include <physics/iParticleContactGenerator.h>
//TODO: Add Rope constraint
namespace nPhysics
{
	//Base Class for any constraint between 2 particles
	class cParticleConstraint : public iParticleContactGenerator
	{
	protected:
		cParticle* mParticleA;
		cParticle* mParticleB;
	public:
		cParticleConstraint(cParticle* particleA, cParticle* particleB);

		cParticleConstraint() = delete;
		cParticleConstraint(const cParticleConstraint& other) = delete;
		cParticleConstraint& operator=(const cParticleConstraint& other) = delete;

		virtual ~cParticleConstraint();

		float CurrentSeparation() const;

		cParticle* GetParticleA();
		cParticle* GetParticleB();

		// AddContact will be overridden by the inheriting classes
		//virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const = 0;
	};


	class cParticleRodConstraint : public cParticleConstraint
	{
	protected:
		float mLength;
	public:
		cParticleRodConstraint(cParticle* particleA, cParticle* particleB);

		cParticleRodConstraint() = delete;
		cParticleRodConstraint(const cParticleRodConstraint& other) = delete;
		cParticleRodConstraint& operator=(const cParticleRodConstraint& other) = delete;

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}