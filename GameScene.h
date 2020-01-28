#pragma once
#include <list>
#include <raylib.h>
#include "Colliders.h"
#include "ECS.h"

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

private:
	void SetupPlayer();
	void SetupShield();
	void GenerateEnemies(float deltaTime);
	std::vector<CircleCollider*> GetAllEnemyColliders() const noexcept;

	Entity* mPlayerEntity = nullptr;
	Entity* mShieldEntity = nullptr;
	std::list<Entity*> mEnemyEntities{};
};

