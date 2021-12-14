#include <graphics/cBuffer.h>

namespace nGraphics
{
	cBuffer::cBuffer()
		: mIds(0), mNumVertices(0), mNumIndices(0)
	{
		mDesc.push_back(cVertexAttributeDesc());
		mDesc.push_back(cVertexAttributeDesc());
		mDesc.push_back(cVertexAttributeDesc());
	}
	cBuffer::~cBuffer()
	{
		if (mIds)
		{
			mIds->Destroy();
			mIds->Clear();
			delete mIds;
			mIds = 0;
		}
	}

	cBuffer* cBuffer::CreateBuffer(std::vector<unsigned int> & indices,
		std::vector<cVertex>& vertices,
		std::vector<cMesh*>& meshes)
	{
		cBuffer* buffer = new cBuffer();
		buffer->mIds = new sBufferIds();
		if (!buffer->mDesc.GenerateBuffers(*buffer->mIds, vertices, indices, GL_STATIC_DRAW))
		{
			delete buffer->mIds;
			buffer->mIds = 0;
			delete buffer;
			buffer = 0;
			return 0;
		}
		buffer->mMeshes = std::vector<cMesh*>(meshes.begin(), meshes.end());
		buffer->mNumIndices = indices.size();
		buffer->mNumVertices = vertices.size();
		return buffer;
	}
	// Binds the buffers
	bool cBuffer::Bind()
	{
		if (!IsLoaded()) return false;
		return mDesc.BindBuffers(*mIds);
	}
	bool cBuffer::Disable()
	{
		if (!IsLoaded()) return false;
		return mDesc.DisableBuffers(*mIds);
	}
	// render the whole buffer
	bool cBuffer::Render()
	{
		return Render(NumIndices(), 0, 0);
	}
	// render the mesh id
	bool cBuffer::Render(int meshIndex)
	{
		if (meshIndex < 0 || meshIndex >= (int)NumMeshes()) return false;
		return Render(mMeshes[meshIndex]);
	}
	bool cBuffer::Render(int numIndices, int baseIndex, int baseVertex)
	{
		if (!IsLoaded()) return false;
		glDrawElementsBaseVertex(GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * baseIndex),
			baseVertex);
		return true;
	}
	bool cBuffer::Render(cMesh* mesh)
	{
		return Render(mesh->GetNumIndices(), mesh->GetBaseIndex(), mesh->GetBaseVertex());
	}
	cMesh* cBuffer::GetMesh(unsigned int meshIndex)
	{
		if (meshIndex < 0 || meshIndex >= NumMeshes()) return 0;
		return mMeshes[meshIndex];
	}
	cMesh* cBuffer::GetMeshByName(const std::string& name)
	{
		for (std::vector<cMesh*>::iterator it = mMeshes.begin(); it != mMeshes.end(); it++)
		{
			if ((*it)->GetName() == name) return *it;
		}
		return 0;
	}
}