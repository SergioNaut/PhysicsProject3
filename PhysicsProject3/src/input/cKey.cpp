#include <input/cKey.h>

namespace nInput
{
	cKey::cKey(int id)
		: mKeyId(id), mDown(false), mJust(false)
	{

	}

	cKey::~cKey() { }

	void cKey::Press()
	{
		if (!mDown)
		{
			mDown = true;
			mJust = true;

		}
		else
		{
			mJust = false;
		}
	}

	void cKey::Release()
	{
		if (mDown)
		{
			mDown = false;
			mJust = true;

		}
		else
		{
			mJust = false;
		}
	}
}