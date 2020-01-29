#pragma once
#include <list>
#include <raylib.h>
#include "Colliders.h"
#include "SceneBase.h"

class GameScene :
	public SceneBase
{
public:
	GameScene();
	explicit GameScene(std::string_view name);
	~GameScene();

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:
	void GenerateEnemies(float deltaTime);
	std::vector<CircleCollider*> GetAllEnemyColliders() const noexcept;
	std::list<Entity*> mEnemyEntities{};
};

