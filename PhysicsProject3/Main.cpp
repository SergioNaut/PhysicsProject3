#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <math.h>

#include <graphics/graphics.h>
#include <input/input.h>
#include <graphics/cCamera.h>
#include <graphics/cameras/c3rdPersonCamera.h>
#include <graphics/cGraphicsComponent.h>
#include <physics/cParticleWorld.h>
#include <physics/particle_contact_generators.h>

//For now, projectile stuff from the midterms
struct sProjectileDef
{
	float size;
	float mass;
	float muzzle;
	float gravity;
	float lifetime;
};

struct sConfig
{
	sProjectileDef ProjectileDef;
};

void mainLoop();

#pragma region CameraSetup
nGraphics::sPerFrameVars PerFrameVars;
nGraphics::c3rdPersonCamera* camera = 0;
#pragma endregion

#pragma region GraphicsStuff
nGraphics::cGraphicsComponent* skyboxGraphics;
nGraphics::cGraphicsComponent* planeGraphics;
nGraphics::cGraphicsComponent* rockGraphics;

nGraphics::cGraphicsComponent* wallGraphics1;
nGraphics::cGraphicsComponent* wallGraphics2;
nGraphics::cGraphicsComponent* wallGraphics3;
nGraphics::cGraphicsComponent* wallGraphics4;
#pragma endregion

//Helper function
//Was being used for setting a random velocity for the particles
//float getRandom(float low, float high);

int main()
{
#pragma region StuffFromProject1
	camera = new nGraphics::c3rdPersonCamera();
	nGraphics::SetCamera(camera);
	glm::mat4 identity(1.f);
	camera->SetTargetTransform(identity);

	if (!nGraphics::Init())
	{
		std::cout << "Error Setting up Graphics" << std::endl;
		system("pause");
		return -1;
	}
	if (!nInput::Init())
	{
		std::cout << "Error Setting up inputs" << std::endl;
		system("pause");
		return -1;
	}

#pragma region Textures
	// Loading textures to use with our meshes
		//nGraphics::gTextureManager->Load("../Assets/skybox/tidepool", "skybox", true);
	nGraphics::gTextureManager->Load("../Assets/WhiteSquare.bmp", "whiteSquare", false);
	nGraphics::gTextureManager->Load("../Assets/stone_wall.bmp", "stone", false);
	nGraphics::gTextureManager->Load("../Assets/grafitti.bmp", "grafitti", false);
	nGraphics::gTextureManager->Load("../Assets/white.bmp", "white", false);
	nGraphics::gTextureManager->Load("../Assets/brickwall.bmp", "bricks", false);
	//nGraphics::gTextureManager->Load("../Assets/manyMtGFaces.bmp", "green", false);
	nGraphics::gTextureManager->Load("../Assets/roald.bmp", "roald", false);
#pragma endregion

#pragma region LoadingMeshes

	// Loading meshes
	std::vector<nGraphics::sMeshLoadingInfo> infos;
	nGraphics::sMeshLoadingInfo loadingInfo;

	// Skybox sphere mesh
	loadingInfo.File = "../Assets/Isoshphere_xyz_InvertedNormals.ply";
	loadingInfo.DoResize = false;
	loadingInfo.AddSubMesh("skybox");
	infos.push_back(loadingInfo);
	// Lower poly Reck mesh for Projectiles
	loadingInfo.File = "../Assets/rock.obj";
	loadingInfo.DoResize = true;
	loadingInfo.Extents = glm::vec3(1.f, 1.f, 1.f);
	loadingInfo.SubMeshes[0].Name = "rock";
	infos.push_back(loadingInfo);
	// Box mesh for planes and boxes
	loadingInfo.File = "../Assets/box.obj";
	loadingInfo.DoResize = true;
	loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // cube 2x2x2
	loadingInfo.SubMeshes[0].Name = "box";
	infos.push_back(loadingInfo);

	if (!nGraphics::gMeshManager->Load(infos))
	{
		std::cout << "Error loading meshes" << std::endl;
		return -1;
	}

#pragma endregion


	// Create the graphics components
	{
		// Skybox Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		skyboxGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Rock Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "rock";
		glm::set(graphicsDef.ModelColor, 0.0f, 0.0f, 0.0f, 0.0f);
		glm::set(graphicsDef.Position, 0.0f, 1.0f, 0.0f);
		glm::set(graphicsDef.Scale, 1.0f, 1.0f, 1.0f);
		rockGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "roald";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -0.2f, 0.0f);
		glm::set(graphicsDef.Scale, 100.0f, 0.002f, 100.0f);
		planeGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		//Wall Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "bricks";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 10.0f, 1.0f, -10.0f);
		//glm::set(graphicsDef.Position, 40.0f, 1.0f, -40.0f);
		//glm::set(graphicsDef.Scale, 80.0f, 8.0f, 1.0f);
		glm::set(graphicsDef.Scale, 20.0f, 8.0f, 0.1f);
		wallGraphics1 = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		//Wall Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "bricks";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, -10.0f, 1.0f, 10.0f);
		//glm::set(graphicsDef.Position, -35.0f, 1.0f, 40.0f);
		//glm::set(graphicsDef.Scale, 80.0f, 8.0f, 1.0f);
		glm::set(graphicsDef.Scale, 20.0f, 8.0f, 0.1f);
		wallGraphics2 = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		//Wall Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "bricks";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, -10.0f, 1.0f, 10.0f);
		//glm::set(graphicsDef.Position, -40.0f, 1.0f, 40.0f);
		//glm::set(graphicsDef.Scale, 1.0f, 8.0f, 80.0f);
		glm::set(graphicsDef.Scale, 0.1f, 8.0f, 20.0f);
		wallGraphics3 = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		//Wall Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "bricks";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 10.0f, 1.0f, -10.0f);
		//glm::set(graphicsDef.Position, 40.0f, 1.0f, -40.0f);
		//glm::set(graphicsDef.Scale, 1.0f, 8.0f,80.0f);
		glm::set(graphicsDef.Scale, 0.1f, 8.0f, 20.0f);
		wallGraphics4 = new nGraphics::cGraphicsComponent(graphicsDef);
	}

	// Enter the main loop
	mainLoop();

	// Clean up
	delete planeGraphics;
	delete rockGraphics;
	delete skyboxGraphics;
	delete wallGraphics1;
	delete wallGraphics2;
	delete wallGraphics3;
	delete wallGraphics4;

	delete camera;

	nGraphics::Shutdown();

	return 0;
#pragma endregion
}

#pragma region RotationBits
glm::mat3 yawMatrix(float yaw)
{
	return glm::mat3(cos(yaw), sin(yaw), 0, -sin(yaw), cos(yaw), 0, 0, 0, 1);
}

glm::mat3 pitchMatrix(float pitch)
{
	return glm::mat3(cos(pitch), 0, -sin(pitch), 0, 1, 0, sin(pitch), 0, cos(pitch));
}

glm::mat3 rotationMatrix(float yaw, float pitch)
{
	return yawMatrix(yaw) * pitchMatrix(pitch);
}
#pragma endregion

void mainLoop()
{
	nInput::gInputManager->ClearState();

	nGraphics::Focus();

	bool continueMainLoop = true;

	float previousTime = static_cast<float>(glfwGetTime());

	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;

//Setup cannon
#pragma region CannonStuff
	float cannonYaw = 0.3f;
	float cannonPitch = 10.0f;
	float* pCannonYaw = &cannonYaw;
	float* pCannonPitch = &cannonPitch;
	float minYaw = -1.0f;
	float maxYaw = 1.0f;
	float minPitch = 0.5f;
	float maxPitch = 26.0f;
#pragma endregion

	// BEGIN PROJECT 3 SETUP
	std::vector<nPhysics::cParticle*> particles;
	nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld(100, 0);
	//Particle variables
	glm::vec4 particleColor(1.0f, 1.0f, 1.0f, 1.0f);
	float particleRadius(1.0f);
#pragma region Planes
	//Planes have some values commented because I'm using this project to test things for the finals so it has some things from the finals
	
	//Ground Plane
	nPhysics::cPlaneParticleContactGenerator particleContactGeneratorG(glm::vec3 (0.0f,/*0.2f*/ 1.0f,0.0f),1.0f,1.0f);
	world->AddContactContactGenerator(&particleContactGeneratorG);

	//Left Wall
	nPhysics::cPlaneParticleContactGenerator particleContactGeneratorLeft(glm::vec3(1.0f, 0.f, 0.0f),/*-40.0f*/-10.0f, 1.0f);
	world->AddContactContactGenerator(&particleContactGeneratorLeft);

	//Right Wall
	nPhysics::cPlaneParticleContactGenerator particleContactGeneratorRight(glm::vec3(-1.0f, 0.f, 0.0f),/*-40.0f*/-10.0f, 1.0f);
	world->AddContactContactGenerator(&particleContactGeneratorRight);

	//Back Wall
	nPhysics::cPlaneParticleContactGenerator particleContactGeneratorBack(glm::vec3(0.0f, 0.f, -2.0f), /*-40.0f*/-10.0f, 1.0f);
	world->AddContactContactGenerator(&particleContactGeneratorBack);

	//Roof - Nope
	//nPhysics::cPlaneParticleContactGenerator particleContactGeneratorRoof(glm::vec3(1.0f, 5.f, 0.0f), -5.0f, 1.0f);
	//world->AddContactContactGenerator(&particleContactGeneratorRoof);

	//Front Wall
	nPhysics::cPlaneParticleContactGenerator particleContactGeneratorFront(glm::vec3(0.0f, 0.f, 1.0f),/*-40.0f*/-10.0f, 1.0f);
	world->AddContactContactGenerator(&particleContactGeneratorFront);
#pragma endregion

#pragma region KeySetup
	nInput::cKey* key1 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_1);
	nInput::cKey* wKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_W);
	nInput::cKey* aKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_A);
	nInput::cKey* sKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_S);
	nInput::cKey* dKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_D);
	//nInput::cKey* key2 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_2);
#pragma endregion

	while (continueMainLoop)
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// FPS TITLE STUFF
		// Kept from the class code because I like knowing the framerate since my pc lags ocasionally while testing
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				nGraphics::SetWindowTitle(newTitle);
				// reset times and counts
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}

		if (key1->IsJustPressed())
		{
			glm::mat3 rotMat = rotationMatrix(cannonYaw, cannonPitch);
			nPhysics::cParticle* particleP = new nPhysics::cParticle(1.0f, glm::vec3(0.f, 1.5f, 0.f));
			particleP->SetAcceleration(glm::vec3(0.0f, -9.8f, 0.0f));
			//Change velocity with rotMat
			//glm::vec3 randomVelocity(getRandom(-3.0f, 3.0f), 10.0f, getRandom(-3.0f, 3.0f));
			glm::vec3 randomVelocity((rotMat[0] * 3.0f) + (rotMat[1] * 10.0f) + (rotMat[2] * 3.0f));
			particleP->SetVelocity(randomVelocity);
			world->AddParticle(particleP);
			particles.push_back(particleP);
		}
		else if (aKey->IsJustPressed())
		{
		if (cannonPitch < maxPitch)
		{
			cannonPitch += 1.0f;
		}
		if (cannonPitch > maxPitch)
		{
			cannonPitch = maxPitch;
		}
		std::cout << cannonPitch << std::endl;
		}
		else if (sKey->IsJustPressed())
		{
			if (cannonYaw > minYaw)
			{
				cannonYaw -= 0.3f;
			}
			if (cannonYaw < minYaw)
			{
				cannonYaw = minYaw;
			}
		std::cout << cannonYaw << std::endl;
		}
		else if (dKey->IsJustPressed())
		{
			if (cannonPitch > minPitch)
			{
				cannonPitch -= 1.0f;
			}
			if (cannonPitch < minPitch)
			{
				cannonPitch = minPitch;
			}
			std::cout << cannonPitch << std::endl;
		}
		else if (wKey->IsJustPressed())
		{
			if (cannonYaw < maxYaw)
			{
				cannonYaw += 0.3f;
			}
			if (cannonYaw > maxYaw)
			{
				cannonYaw = maxYaw;
			}
			std::cout << cannonYaw << std::endl;
		}
	

		world->TimeStep(deltaTime);

		// Safety, mostly for first frame
		if (deltaTime == 0.f)
		{
			deltaTime = 0.03f;
		}

		// update the camera
		camera->Update(deltaTime);

		// Clear inputs
		nInput::gInputManager->ClearState();

		// begin setting per-frame vars
		camera->GetEyePosition(PerFrameVars.EyePosition);
		camera->GetViewMatrix(PerFrameVars.ViewMatrix);
		camera->GetProjectionMatrix(PerFrameVars.ProjectionMatrix);
		// end setting per-frame vars

		nGraphics::BeginFrame(PerFrameVars);

		// Graphical Item rendering
		skyboxGraphics->Render();
		planeGraphics->Render();
		wallGraphics1->Render();
		wallGraphics2->Render();
		wallGraphics3->Render();
		wallGraphics4->Render();

		// render the particles
		//Using the projectileMatrix from midterms
		for (nPhysics::cParticle* p : particles)
		{
			glm::mat4 projectileMatrix(1.0f);
			projectileMatrix = glm::translate(projectileMatrix, p->GetPosition());
			//projectileMatrix = glm::scale(projectileMatrix, glm::vec3(p->GetRadius()));
			projectileMatrix = glm::scale(projectileMatrix, glm::vec3(particleRadius));
			rockGraphics->GetVars()->ModelMatrix = projectileMatrix;
			rockGraphics->GetVars()->ModelColor = particleColor;
			rockGraphics->Render();
		}
		// end Graphical Item rendering

		nGraphics::EndFrame();
		//No need to delete the particles

		// Exit conditions: press escape or close the window by the 'x' button
		if (!(nInput::IsKeyUp::Escape() && !nGraphics::WindowShouldClose()))
		{
			continueMainLoop = false;
		}
	}
	for (nPhysics::cParticle* p : particles)
	{
		world->RemoveParticle(p);
		delete p;
	}
	particles.clear();
	// clean up!
	delete world;
}

//#pragma region Helpers
//
//float getRandom(float low, float high)
//{
//	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
//}
//
//#pragma endregion