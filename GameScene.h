#pragma once
#include "ECS.h"
#include <raylib.h>

class GameScene :
	public Scene
{
public:
	GameScene();
	explicit GameScene(std::string_view name);
	~GameScene();

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};

