#include <extern_includes.h>
#include <graphics/load/LoadCubeMapFromFiles.h>
#include <graphics/load/cImageFile.h>

//Will leave the cube map functions in case I get to use them 
//I will try
namespace nGraphics
{
	GLuint LoadCubeMapFromFiles(
		const char* posX_fileName, const char* negX_fileName,
		const char* posY_fileName, const char* negY_fileName,
		const char* posZ_fileName, const char* negZ_fileName,
		bool isSeamless)
	{
		// Generate a texture ID and bind to it
		GLuint tempTextureID;
		glGenTextures(1, &tempTextureID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tempTextureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

		cImageFile* image = new cImageFile();
		// This only works if the images have the same size
		if (image->LoadFromFile(posX_fileName))
		{
			glTexStorage2D(GL_TEXTURE_CUBE_MAP,
				10, // Mipmap levels
				GL_RGBA8,	// Internal format
				image->GetImageWidth(),	// width
				image->GetImageHeight()); // height (pixels)
		}
		else
		{
			delete image;
			return 0;
		}
		
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			0,   // Level
			0, 0, // Offset
			image->GetImageWidth(),
			image->GetImageHeight(),
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image->GetTextureData());

		if (image->LoadFromFile(negX_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
				0, 0, 0,
				image->GetImageWidth(),
				image->GetImageHeight(),
				GL_RGBA,
				GL_UNSIGNED_BYTE, 
				image->GetTextureData());
		}
		else
		{
			delete image;
			return 0;
		}

		if (image->LoadFromFile(posY_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				0, 0, 0,
				image->GetImageWidth(),
				image->GetImageHeight(),
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
		}
		else
		{
			delete image;
			return 0;
		}

		if (image->LoadFromFile(negY_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				0, 0, 0,
				image->GetImageWidth(),
				image->GetImageHeight(),
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
		}
		else
		{
			delete image;
			return 0;
		}

		if (image->LoadFromFile(posZ_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				0, 0, 0,
				image->GetImageWidth(),
				image->GetImageHeight(),
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
		}
		else
		{
			delete image;
			return 0;
		}

		if (image->LoadFromFile(negZ_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				0, 0, 0,
				image->GetImageWidth(),
				image->GetImageHeight(),
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
		}
		else
		{
			delete image;
			return 0;
		}

		delete image;
		// Finally, return the texture ID
		return tempTextureID;
	}
}