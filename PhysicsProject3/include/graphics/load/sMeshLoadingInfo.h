#pragma once
#include <extern_includes.h>
#include <string>
#include <vector>

namespace nGraphics
{
	struct sSubMeshLoadingInfo
	{
		sSubMeshLoadingInfo() : MeshIndex(0) {}
		sSubMeshLoadingInfo(const std::string& name, unsigned int meshIndex = 0)
			: Name(name), MeshIndex(meshIndex) {}
		std::string Name;
		unsigned int MeshIndex;
	};
	struct sMeshLoadingInfo
	{
		sMeshLoadingInfo() : DoResize(false), Extents(1.f, 1.f, 1.f), MaintainDimensions(true) {}

		std::string File;
		std::vector<sSubMeshLoadingInfo> SubMeshes;
		void AddSubMesh(const std::string& name, unsigned int meshIndex = 0) { SubMeshes.push_back(sSubMeshLoadingInfo(name, meshIndex)); }
		void AddSubMesh(const sSubMeshLoadingInfo& subMeshInfo) { SubMeshes.push_back(subMeshInfo); }
		//Used for resizing
		glm::vec3 Extents;
		// Checks if any resizing needs to be done
		bool DoResize;
		// if you want to maintain the dimensions,
		// we'll be using the minimum of whatever you put in Extents
		bool MaintainDimensions;
	};
}
