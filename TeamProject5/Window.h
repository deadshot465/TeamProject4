#pragma once
#include <memory>
#include <raylib.h>
#include <string_view>
#include "Player.h"

class Window
{
private:
	int mWidth = 0;
	int mHeight = 0;

	std::unique_ptr<Player> mPlayer = nullptr;

	void Update();

public:
	Window(std::string_view title, int width, int height);
	~Window();

	void Run(Color clearColor);
};

