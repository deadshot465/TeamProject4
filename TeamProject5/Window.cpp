#include "Window.h"

void Window::Update()
{
	mPlayer->Update(mWidth, mHeight);
}

Window::Window(std::string_view title, int width, int height)
	: mWidth(width), mHeight(height)
{
	InitWindow(width, height, title.data());

	SetTargetFPS(60);

	mPlayer = std::make_unique<Player>(width / 2.0f, height / 2.0f);
}

Window::~Window()
{
	CloseWindow();
}

void Window::Run(Color clearColor)
{
	while (!WindowShouldClose()) {

		Update();

		BeginDrawing();

		ClearBackground(clearColor);

		DrawText("FUCK OFF!!!", 190, 200, 20, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 380, 300, 30, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 570, 400, 40, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 760, 500, 50, LIGHTGRAY);

		mPlayer->Render();

		EndDrawing();
	}
}
