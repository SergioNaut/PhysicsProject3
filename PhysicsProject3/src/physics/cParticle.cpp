#include <physics/cParticle.h>

float getRandom(float low, float high)
{
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

namespace nPhysics
{
	cParticle::cParticle(float mass, const glm::vec3& position)
		: mPosition(position)
		, mVelocity(0.f)
		, mAcceleration(0.f)
		, mDamping(0.995f)
		, mAppliedForce(0.f)
		, mIsAlive(true)
	{
		//Randomizes the sphereRadius a little bit
		sphereRadius = getRandom(0.1f, 1.0f);
		SetMass(mass + sphereRadius);
	}
	cParticle::~cParticle(){}

	float cParticle::GetRadius()
	{
		return sphereRadius;
	}

	//Added to setup mass after particle has been created
	void cParticle::SetMass(float newMass)
	{
		if (newMass <= 0.f)
		{
			mInverseMass = 0.f;
		}
		else
		{
			mInverseMass = 1.f / newMass;
		}
	}


	void cParticle::SetDamping(float damping)
	{
		mDamping = glm::clamp(damping, 0.f, 1.f);
	}
	float cParticle::GetDamping() const
	{
		return mDamping;
	}

	float cParticle::GetMass() const
	{
		if (mInverseMass == 0.f)
		{
			return 0;
		}
		return mInverseMass;
	}
	float cParticle::GetInverseMass() const
	{
		return mInverseMass;
	}

	bool cParticle::IsStaticObject() const
	{
		return mInverseMass == 0.f;
	}

	glm::vec3 cParticle::GetPosition() const
	{
		return mPosition;
	}
	void cParticle::GetPosition(glm::vec3& position)
	{
		position = mPosition;
	}
	void cParticle::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	glm::vec3 cParticle::GetVelocity() const
	{
		return mVelocity;
	}
	void cParticle::GetVelocity(glm::vec3& velocity)
	{
		velocity = mVelocity;
	}
	void cParticle::SetVelocity(const glm::vec3& velocity)
	{
		mVelocity = velocity;
	}

	glm::vec3 cParticle::GetAcceleration() const
	{
		return mAcceleration;
	}
	void cParticle::GetAcceleration(glm::vec3& acceleration)
	{
		acceleration = mAcceleration;
	}
	void cParticle::SetAcceleration(const glm::vec3& acceleration)
	{
		mAcceleration = acceleration;
	}

	void cParticle::ApplyForce(const glm::vec3& force)
	{
		mAppliedForce += force;
	}

	void cParticle::ApplyImpulse(const glm::vec3& impulse)
	{
		mVelocity += impulse * mInverseMass;
	}

	void cParticle::ClearAppliedForces()
	{
		glm::set(mAppliedForce, 0.f, 0.f, 0.f);
	}

	void cParticle::Integrate(float deltaTime)
	{
		if (mInverseMass == 0.f)
		{
			return;
		}

		mPosition += mVelocity * deltaTime;
		mVelocity += (mAcceleration + mAppliedForce * mInverseMass) * deltaTime;

		// apply damping
		mVelocity *= glm::pow(mDamping, deltaTime);

		// clear applied forces
		ClearAppliedForces();
	}

#pragma region IsAliveHelpers

	void cParticle::SetIsAlive(bool isAlive)
	{
		mIsAlive = isAlive;
	}

	bool cParticle::GetIsAlive() const
	{
		return mIsAlive;
	}
#pragma endregion

}