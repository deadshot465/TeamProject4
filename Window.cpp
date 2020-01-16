#include "Window.h"
#include <memory>
#include "ConfigurationManager.h"
#include "Primitives.h"

Window::Window(std::string_view title, int width, int height)
	: mWidth(width), mHeight(height),
	mGameScene()
{
	SetConfigFlags(ConfigFlag::FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.data());

	SetTargetFPS(60);
	ConfigurationManager::SetViewport(width, height);
	
	mGameScene.Initialize();

	auto player = mGameScene.AddEntity("player-entity");
	player->Position = glm::vec2(512, 384);
	auto circle = std::make_unique<Primitives::Circle>("player-circle", 50.0f);
	circle->FillColor = GOLD;
	player->AddComponent(circle);
}

Window::~Window()
{
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

		BeginDrawing();

		ClearBackground(clearColor);

		DrawText("FUCK OFF!!!", 190, 200, 20, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 380, 300, 30, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 570, 400, 40, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 760, 500, 50, LIGHTGRAY);

		Render();

		EndDrawing();
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