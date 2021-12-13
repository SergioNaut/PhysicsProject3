#include <input/cInputManager.h>
#include <graphics/graphics.h> 
#include <iostream>

//Setup for inputs
namespace nInput
{
	cInputManager* cInputManager::mInstance = 0;

	cInputManager* cInputManager::GetInstance()
	{
		if (mInstance == 0)
		{
			mInstance = new cInputManager();
		}
		return mInstance;
	}

	cInputManager::cInputManager()
	{
		glfwSetKeyCallback(nGraphics::gWindow, KeyCallback);
		glfwSetMouseButtonCallback(nGraphics::gWindow, MouseKeyCallback);
		glfwSetCursorPosCallback(nGraphics::gWindow, MousePosCallback);
		glfwSetScrollCallback(nGraphics::gWindow, ScrollCallback);
	}

	cInputManager::~cInputManager()
	{
		glfwSetKeyCallback(nGraphics::gWindow, 0);
		mInstance = 0;
	}
	
	cKey* cInputManager::GetKey(int keyId)
	{
		IterKey it = mKeys.find(keyId);
		if (it == mKeys.end()) return 0;
		return it->second;
	}
	
	cKey* cInputManager::ListenToKey(int keyId)
	{
		cKey* key = GetKey(keyId);
		if (!key)
		{
			key = new cKey(keyId);
			mKeys[keyId] = key;
		}
		return key;
	}

	void cInputManager::ClearState()
	{
		IterKey it = mKeys.begin();
		while (it != mKeys.end())
		{
			it->second->mJust = false;
			++it;
		}
		mScrollX = 0.0;
		mScrollY = 0.0;
	}
	
	void cInputManager::KeyCallback(GLFWwindow* window, int keyId, int scancode, int action, int mods)
	{
		if (!mInstance)
		{
			std::cout << "Error on KeyCallback: No inputManager Instance exists." << std::endl;
			return;
		}
		cKey* key = mInstance->GetKey(keyId);
		if (!key) return;

		if (action == GLFW_PRESS)
		{
			key->Press();
		}
		else
		{
			key->Release();
		}
	}

	void cInputManager::MouseKeyCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (!mInstance)
		{
			std::cout << "Error on MouseKeyCallback: No inputManager Instance exists." << std::endl;
			return;
		}
		cKey* key = mInstance->GetKey(button);
		if (!key) return;

		if (action == GLFW_PRESS)
		{
			key->Press();
		}
		else
		{
			key->Release();
		}
	}

	void cInputManager::MousePosCallback(GLFWwindow* window, double x, double y)
	{
		if (!mInstance)
		{
			std::cout << "Error on MousePositionCallback: No inputManager Instance exists." << std::endl;
			return;
		}
		mInstance->mCursorX = x;
		mInstance->mCursorY = y;
	}

	void cInputManager::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		if (!mInstance)
		{
			std::cout << "Error on ScrollCallback: No inputManager Instance exists." << std::endl;
			return;
		}
		mInstance->mScrollX = x;
		mInstance->mScrollY = y;
	}
}