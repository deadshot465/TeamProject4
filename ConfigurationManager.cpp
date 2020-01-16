#include "ConfigurationManager.h"
#include <cassert>

int ConfigurationManager::GetWidth() noexcept
{
	assert(mWidth > 0);
	return mWidth;
}

int ConfigurationManager::GetHeight() noexcept
{
	assert(mHeight > 0);
	return mHeight;
}

void ConfigurationManager::SetViewport(int width, int height)
{
	mWidth = width;
	mHeight = height;
}
