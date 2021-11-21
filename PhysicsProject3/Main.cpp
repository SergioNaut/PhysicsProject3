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

#pragma region BitsfromProject1

bool particleIsAboveGround(glm::vec3& position);

void mainLoop();
#pragma endregion

#pragma region CameraSetup
nGraphics::sPerFrameVars PerFrameVars;
nGraphics::c3rdPersonCamera* camera = 0;
nGraphics::cGraphicsComponent* skyboxGraphics;
nGraphics::cGraphicsComponent* planeGraphics;
nGraphics::cGraphicsComponent* rockGraphics;

nGraphics::cGraphicsComponent* cannonGraphics;
#pragma endregion

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

	{
		// Loading textures to use with our meshes
		//nGraphics::gTextureManager->Load("../Assets/skybox/tidepool", "skybox", true);
		nGraphics::gTextureManager->Load("../Assets/WhiteSquare.bmp", "whiteSquare", false);
		nGraphics::gTextureManager->Load("../Assets/stone_wall.bmp", "stone", false);
		nGraphics::gTextureManager->Load("../Assets/grafitti.bmp", "grafitti", false);
		nGraphics::gTextureManager->Load("../Assets/white.bmp", "white", false);
	}

	{
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
		loadingInfo.Extents = glm::vec3(1.f, 1.f, 1.f); // diameter 2 spheref
		loadingInfo.SubMeshes[0].Name = "rock";
		infos.push_back(loadingInfo);
		// Box mesh for planes and boxes
		loadingInfo.File = "../Assets/box.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // cube 2x2x2
		loadingInfo.SubMeshes[0].Name = "box";
		infos.push_back(loadingInfo);

		//Cannon
		loadingInfo.File = "../Assets/Cannon.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(0.1f, 0.1f, 0.1f);
		loadingInfo.SubMeshes[0].Name = "cannon";
		infos.push_back(loadingInfo);

		if (!nGraphics::gMeshManager->Load(infos))
		{
			std::cout << "Error loading meshes" << std::endl;
			return -1;
		}
	}

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
		graphicsDef.TexDiffuse = "grafitti";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -0.2f, 0.0f);
		glm::set(graphicsDef.Scale, 10.0f, 0.2f, 10.0f);
		planeGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	//Cannon
	nGraphics::sGraphicsComponentDef graphicsDef;
	graphicsDef.Mesh = "cannon";
	graphicsDef.TexDiffuse = "white";
	glm::set(graphicsDef.ModelColor, 0.0f, 0.0f, 0.0f, 1.0f);
	glm::set(graphicsDef.Position, 0.5f, 0.0f, 0.5f);
	glm::set(graphicsDef.Rotation, -1.57f, 0.f, 0.f);
	glm::set(graphicsDef.Scale, 20.f, 20.1f, 20.1f);
	cannonGraphics = new nGraphics::cGraphicsComponent(graphicsDef);

	// Enter the main loop
	mainLoop();

	// Clean up
	delete planeGraphics;
	delete rockGraphics;
	delete skyboxGraphics;
	delete cannonGraphics;
	delete camera;

	nGraphics::Shutdown();

	// all done!
	return 0;
#pragma endregion
}

//TODO: MAKE CANNON ROTATION WORK
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


void InitProjectileVariables(float yaw, float pitch, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration,
	float size, float muzzle, float gravity)
{
	glm::mat3 rotMat = rotationMatrix(yaw, pitch);
	//Added size so projectile won't get stuck on the ground
	position = glm::vec3(0.0, size, 0.0);
	//velocity = glm::vec3(7.0f, 10.f, 0.f);
	velocity = glm::vec3((rotMat[0] * 7.0f) + (rotMat[1] * 10.f) + (rotMat[2] * 0.f));
	velocity = glm::normalize(velocity);

	//Added Muzzle to velocity
	velocity *= muzzle;

	// Added custom gravity
	acceleration = glm::vec3(0.f, -9.8f * gravity, 0.f);
}


void mainLoop()
{
	nInput::gInputManager->ClearState();

	nGraphics::Focus();

	//Setup projectile
	sConfig projectileConfig;
	////Setup cannon

	float cannonYaw = 0.3f;
	float cannonPitch = 10.0f;
	float* pCannonYaw = &cannonYaw;
	float* pCannonPitch = &cannonPitch;
	float minYaw = -1.0f;
	float maxYaw = 1.0f;
	float minPitch = 0.5f;
	float maxPitch = 26.0f;
	bool continueMainLoop = true;

	float previousTime = static_cast<float>(glfwGetTime());

	//World Setup
	nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld();
	nPhysics::cParticle* particle = new nPhysics::cParticle(1.0f, glm::vec3(0.f));
	if (world->AddParticle(particle))
	{
		std::cout << "Particle Added!" << std::endl;
	}
	else
	{
		std::cout << "Error adding particle." << std::endl;
	}
	glm::mat3 axes;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float timeElapsed = 0;

	particle->SetPosition(position);
	particle->SetVelocity(velocity);
	particle->SetAcceleration(acceleration);
	bool inFlight = false;
#pragma region Keys
	nInput::cKey* Key1 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_1);
	nInput::cKey* Key2 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_2);
	nInput::cKey* Key3 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_3);
	nInput::cKey* Key4 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_4);
	nInput::cKey* wKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_W);
	nInput::cKey* aKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_A);
	nInput::cKey* sKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_S);
	nInput::cKey* dKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_D);
#pragma endregion

	while (continueMainLoop)
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;


		if (!inFlight)
		{
			if (Key1->IsJustPressed())
			{
				//Basic Config for the projectile
				//TODO: Take out if not needed 
				projectileConfig.ProjectileDef.size = 2.0f;
				projectileConfig.ProjectileDef.mass = 1.0f;
				projectileConfig.ProjectileDef.muzzle = 10.0f;
				projectileConfig.ProjectileDef.gravity = 1.0f;
				projectileConfig.ProjectileDef.lifetime = 10000.0f;

				InitProjectileVariables(cannonYaw, cannonPitch, position, velocity, acceleration,
					projectileConfig.ProjectileDef.size, projectileConfig.ProjectileDef.muzzle, projectileConfig.ProjectileDef.gravity);
				particle->SetMass(projectileConfig.ProjectileDef.mass);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
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
		}

		particle->GetPosition(position);
		particle->GetVelocity(velocity);
		particle->GetAcceleration(acceleration);

		if (inFlight)
		{
			if (particleIsAboveGround(position))
			{
				world->TimeStep(deltaTime);
				projectileConfig.ProjectileDef.lifetime--;
				if (projectileConfig.ProjectileDef.lifetime <= 0)
				{
					inFlight = false;
				}
			}
			else
			{
				inFlight = false;
				std::cout << "It`s done" << std::endl;
			}
		}

		if (deltaTime == 0.f)
		{
			deltaTime = 0.03f;
		}

		// update the camera
		camera->Update(deltaTime);

		// done with all the updates involving input, so clear it out
		nInput::gInputManager->ClearState();

		// begin setting per-frame vars
		camera->GetEyePosition(PerFrameVars.EyePosition);
		camera->GetViewMatrix(PerFrameVars.ViewMatrix);
		camera->GetProjectionMatrix(PerFrameVars.ProjectionMatrix);
		// end setting per-frame vars

		nGraphics::BeginFrame(PerFrameVars);

		planeGraphics->Render();
		rockGraphics->GetVars()->ModelMatrix = glm::translate(glm::mat4(1.0f), position);
		//cannonGraphics->GetVars()->ModelMatrix = glm::rotateY(glm::vec3(0.f, 1.0f, 0.f),1.57f);
		//Different Projectile Colors
		//Gray bullet
		glm::set(rockGraphics->GetVars()->ModelColor, 1.0f, 1.0f, 1.0f);
		
		rockGraphics->Render();
		cannonGraphics->Render();

		nGraphics::EndFrame();

		// Exit conditions: press escape or close the window by the 'x' button
		if (!(nInput::IsKeyUp::Escape() && !nGraphics::WindowShouldClose()))
		{
			continueMainLoop = false;
		}
	}

	// clean up!
	world->RemoveParticle(particle);
	delete particle;
	delete world;
}

#pragma region OriginalHelpers

bool particleIsAboveGround(glm::vec3& position)
{

	return position.y > 1.0f;
}

#pragma endregion