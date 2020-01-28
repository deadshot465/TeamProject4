#include "Window.h"
#include <memory>
#include "ConfigurationManager.h"
#include "MapChipManager.h"
#include "Primitives.h"

Window::Window(std::string_view title, int width, int height)
	: mWidth(width), mHeight(height),
	mGameScene{ "game-scene" }
{
	SetConfigFlags(ConfigFlag::FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.data());

	SetTargetFPS(60);
	ConfigurationManager::SetViewport(width, height);
	MapChipManager::Initialize();
	
	mGameScene.Initialize();
}

Window::~Window()
{
	MapChipManager::Release();
	CloseWindow();
}

void Window::Run(Color clearColor)
{
	using namespace std::chrono;

	static auto start_time = high_resolution_clock::now();

	while (!WindowShouldClose()) {

		auto current_time = high_resolution_clock::now();
		float delta = duration<float, seconds::period>(current_time - start_time).count();
		start_time = current_time;

		Update(delta);

		ClearBackground(clearColor);

		Render();
	}
}

void Window::Update(float deltaTime)
{
	if (!mGameScene.IsInitialized()) return;

	mGameScene.Update(deltaTime);
}

void Window::Render()
{
	if (!mGameScene.IsInitialized()) return;

	mGameScene.Render();
}

void Window::UpdateEnemies()
{
}

void Window::GenerateEnemies()
{
}