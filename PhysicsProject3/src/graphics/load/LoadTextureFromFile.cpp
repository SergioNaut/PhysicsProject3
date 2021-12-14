#include <extern_includes.h>
#include <graphics/load/LoadTextureFromFile.h>

#include <graphics/load/cImageFile.h>

namespace nGraphics
{
	GLuint LoadTextureFromFile(const char* fileName, GLenum minificationFilter, GLenum magnificationFilter)
	{
		cImageFile* image = new cImageFile();
		if (!image->LoadFromFile(fileName))
		{
			delete image;
			return 0;
		}
		int imageWidth = image->GetImageWidth();
		int imageHeight = image->GetImageHeight();
		GLubyte* textureData = image->GetTextureData();
		// Generate a texture ID and bind to it
		GLuint tempTextureID;
		glGenTextures(1, &tempTextureID);
		glBindTexture(GL_TEXTURE_2D, tempTextureID);
		// Construct the texture.
		if (1)
		{
			glTexImage2D(GL_TEXTURE_2D,    // Type of texture
				0,                // Mipmap level (0 being the top level i.e. full size)
				GL_RGBA,          // Internal format
				imageWidth,       // Width of the texture
				imageHeight,      // Height of the texture,
				0,                // Border in pixels
				GL_BGRA,          // Data format
				GL_UNSIGNED_BYTE, // Type of texture data
				textureData);     // The image data to use for this texture
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D,
				1,
				GL_RGBA8,
				imageWidth, imageHeight);
			glTexSubImage2D(GL_TEXTURE_2D,
				0,		// Level 0
				0, 0,	// Offset of 0,0
				imageWidth,
				imageHeight,
				GL_RGBA,			// Pixel data format
				GL_UNSIGNED_BYTE,	// Pixel data type  
				textureData);
		}
		// Specify our minification and magnification filters
		// Looks like a typo, I guess it's not
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
		// If we're using MipMaps, then we'll generate them here.
		// Note: The glGenerateMipmap call requires OpenGL 3.0 as a minimum.
		if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		delete image;
		// Finally, return the texture ID
		return tempTextureID;
	}
}