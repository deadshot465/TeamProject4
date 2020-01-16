#include "GameScene.h"
#include "ConfigurationManager.h"

GameScene::GameScene() : Scene()
{
}

GameScene::GameScene(std::string_view name)
	: Scene(name)
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	for (auto& entity : mEntities)
		entity.Initialize();

	mInitialized = true;
}

void GameScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);
}

void GameScene::Render()
{
	Scene::Render();
}
