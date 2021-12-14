#pragma once
#include <extern_includes.h>

namespace nGraphics
{
	class cVertex
	{
	public:
		cVertex();

		glm::vec3 Position;
		// separated for convenience
		float PositionW;
		glm::vec3 Normal;
		// separated for convenience
		float NormalW;
		glm::vec4 TexCoords;
	};
}