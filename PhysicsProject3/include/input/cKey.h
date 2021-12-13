#pragma once
#include <extern_includes.h>
#include <string>

namespace nInput
{
	class cInputManager;
	class cKey
	{
	public:

		inline int GetId() const { return mKeyId; }
		// Is down
		inline bool IsDown() const { return mDown; }
		// Is Up
		inline bool IsUp() const { return !mDown; }
		//Checks if the button has just been pressed
		inline bool IsJustPressed() const { return mDown && mJust; }
		//Checks if the button has just been released
		inline bool IsJustReleased() const { return !mDown && mJust; }

	protected:
		cKey(int id);
		virtual ~cKey();

		int mKeyId;
		bool mDown;
		bool mJust;

	private:
		friend class cInputManager;
		void Press();
		void Release();

		
		cKey(cKey& other) {}
		cKey& operator=(cKey& other) { return *this; }
	};
}