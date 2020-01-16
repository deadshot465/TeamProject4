#pragma once

class Window;

class ConfigurationManager
{
public:
	static int GetWidth() noexcept;
	static int GetHeight() noexcept;

private:
	inline static int mWidth = 0;
	inline static int mHeight = 0;

	static void SetViewport(int width, int height);

	friend class Window;
};

