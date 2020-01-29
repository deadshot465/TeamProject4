#include "Window.h"
#include <memory>
#include <cstdlib>
#include "ConfigurationManager.h"
#include "MapChipManager.h"
#include "Primitives.h"
#include "SFX.h"

Window::Window(std::string_view title, int width, int height)
	: mWidth(width), mHeight(height)
{
	SetConfigFlags(ConfigFlag::FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.data());

	SetTargetFPS(60);
	ConfigurationManager::SetViewport(width, height);

	mScenes.emplace_back(std::make_unique<TitleScene>("title-scene"));
	mScenes.emplace_back(std::make_unique<GameScene>("game-scene"));
	
	for (auto& scene : mScenes)
	{
		scene->SceneChangeHandler = HandleScene;
		scene->WindowHandle = this;
	}

	// Initialize map chip manager.
	MapChipManager::Initialize();

	// Initialize sfx manager.
	SFX::Initialize();
	SFX::LoadSfx("./sfx/dash_1.wav", "Dash");
	SFX::LoadSfx("./sfx/explosion_1.wav", "Explosion");
	SFX::LoadMusicFile("./sfx/bensound-scifi.ogg", "DemoMusic");

	mCurrentScene = mScenes[size_t(Scenes::TitleScene)].get();
	mCurrentScene->Initialize();
}

Window::~Window()
{
	Terminate();
}

void Window::Run(Color clearColor)
{
	using namespace std::chrono;

	SFX::PlayMusic("DemoMusic");

	static auto start_time = high_resolution_clock::now();

	while (!WindowShouldClose()) {

		SFX::UpdateMusic("DemoMusic");

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
	if (!mCurrentScene->IsInitialized()) return;

	mCurrentScene->Update(deltaTime);
}

void Window::Render()
{
	if (!mCurrentScene->IsInitialized()) return;

	mCurrentScene->Render();
}

void Window::Terminate()
{
	SFX::Release();
	MapChipManager::Release();

	for (auto& scene : mScenes)
		scene.reset();

	CloseWindow();
}

void HandleScene(int sceneNo, Window* handle)
{
	auto scene_type = Scenes(sceneNo);

	switch (scene_type)
	{
	case Scenes::TitleScene:
	{
		auto& scene = handle->mScenes[sceneNo];
		handle->mCurrentScene = scene.get();
		if (scene->IsInitialized())
			scene.reset(new TitleScene("title-scene"));
		scene->Initialize();
		break;
	}
	case Scenes::GameScene:
	{
		auto& scene = handle->mScenes[sceneNo];
		handle->mCurrentScene = scene.get();
		if (scene->IsInitialized())
			scene.reset(new GameScene("game-scene"));
		scene->Initialize();
		break;
	}
	case Scenes::End:
		handle->Terminate();
		std::exit(0);
		break;
	default:
		break;
	}
}
