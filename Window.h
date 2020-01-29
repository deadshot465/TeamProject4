#pragma once
#include <chrono>
#include <memory>
#include <raylib.h>
#include <string_view>
#include <vector>
#include "ECS.h"
#include "GameScene.h"
#include "Helper.h"
#include "TitleScene.h"

void HandleScene(int sceneNo, Window* handle);

class Window
{
public:
	Window(std::string_view title, int width, int height);
	~Window();

	void Run(Color clearColor);

private:
	int mWidth = 0;
	int mHeight = 0;
	std::chrono::time_point<std::chrono::steady_clock> mEnemyTimer
		= std::chrono::high_resolution_clock::now();
	float mSecondsBeforeNextEnemy = GetRandomNumber(1.0f, 3.0f);
	std::vector<std::unique_ptr<Scene>> mScenes;

	void Update(float deltaTime);
	void Render();
	void Terminate();

	Scene* mCurrentScene = nullptr;
	
	friend void HandleScene(int sceneNo, Window* handle);
};

