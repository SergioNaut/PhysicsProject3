#pragma once
#include <extern_includes.h>
#include <graphics/load/sMeshLoadingInfo.h>
#include <graphics/cBuffer.h>

namespace nGraphics
{
	//Is a Singleton
	class cMeshManager
	{
	public:
		static cMeshManager* GetInstance();
		~cMeshManager();

		// Loads an individual mesh (No submeshes are used)
		bool Load(const sMeshLoadingInfo& loadingInfo);
		// load everything
		bool Load(std::vector<sMeshLoadingInfo>& loadingInfos);
		// unload everything
		void Unload();
		// gets a loaded mesh
		// if it hasn't been loaded, 0 will be returned
		cMesh* GetMeshByName(const std::string& name);
		bool Bind(cMesh* mesh);
		bool Disable();
		bool Render(cMesh* mesh);
	private:
		cMeshManager();
		cMeshManager(const cMeshManager& tm) = delete;
		cMeshManager& operator=(const cMeshManager& tm) = delete;

		static cMeshManager* mInstance;
		cBuffer* mBuffer;
		std::vector<cMesh*> mMeshes;
	};
}