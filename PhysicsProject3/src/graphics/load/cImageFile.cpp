#pragma once
#include <extern_includes.h>
#include <graphics/load/cImageFile.h>
#include <iostream>

//Not gonna try messing with the image reading bits since I'm super not doing fine with Graphics class

namespace nGraphics
{
	cImageFile::cImageFile()
		: mBitmap(0)
		, mBitmap32(0)
		, mImageWidth(0)
		, mImageHeight(0)
		, mBitsPerPixel(0)
	{}
	cImageFile::~cImageFile()
	{
		Unload();
	}
	bool cImageFile::IsLoaded()
	{
		return mBitmap != 0;
	}
	void cImageFile::Unload()
	{
		if (!IsLoaded())
		{
			return;
		}
		// Unload the 32-bit colour bitmap
		FreeImage_Unload(mBitmap32);

		if (mBitsPerPixel != 32)
		{
			FreeImage_Unload(mBitmap);
		}
		mBitmap = 0;
		mBitmap32 = 0;
		mImageHeight = 0;
		mImageWidth = 0;
		mBitsPerPixel = 0;
	}
	bool cImageFile::LoadFromFile(const std::string& fileName)
	{
		//For Safety
		Unload();
		// Determine the format of the image.
		//The size parameter is not being used so it's left as 0
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);
		
		//Aborts operation if image wasn't found
		if (format == -1)
		{
			std::cout << "Could not find image: " << fileName << " - Aborting." << std::endl;;
			return false;
		}

		// If image was found but the format couldn't be determined the system tries again
		if (format == FIF_UNKNOWN)
		{
			std::cout << "Couldn't determine file format - attempting to get from file extension..." << std::endl;

			format = FreeImage_GetFIFFromFilename(fileName.c_str());

			if (!FreeImage_FIFSupportsReading(format))
			{
				std::cout << "Detected image format cannot be read!" << std::endl;
				return false;
			}
		}

		// If we're here we have a known image format, so load the image into a bitap
		mBitmap = FreeImage_Load(format, fileName.c_str());

		// How many bits-per-pixel is the source image?
		mBitsPerPixel = FreeImage_GetBPP(mBitmap);
		
		if (mBitsPerPixel == 32)
		{
			std::cout << "Source image has " << std::to_string(mBitsPerPixel) << " bits per pixel. Skipping conversion." << std::endl;;
			mBitmap32 = mBitmap;
		}
		else
		{
			std::cout << "Source image has " << std::to_string(mBitsPerPixel) << " bits per pixel. Converting to 32-bit colour." << std::endl;
			mBitmap32 = FreeImage_ConvertTo32Bits(mBitmap);
		}

		// Some basic image info - strip it out if you don't care
		mImageWidth = FreeImage_GetWidth(mBitmap32);
		mImageHeight = FreeImage_GetHeight(mBitmap32);
		std::cout << "Image: " << fileName << " is size: " << std::to_string(mImageWidth) << "x" << std::to_string(mImageHeight) << "." << std::endl;;

		return true;
	}
	GLubyte* cImageFile::GetTextureData()
	{
		if (mBitmap32)
		{
			return FreeImage_GetBits(mBitmap32);;
		}
		else
		{
			return 0;
		}
	}
	int cImageFile::GetImageHeight()
	{
		return mImageHeight;
	}
	int cImageFile::GetImageWidth()
	{
		return mImageWidth;
	}
	int cImageFile::GetBitsPerPixel()
	{
		return mBitsPerPixel;
	}
}