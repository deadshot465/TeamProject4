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
	void GenerateEnemies(float deltaTime, float speedFactor, int attack, int threshold);

	std::vector<CircleCollider*> GetAllEnemyColliders() const noexcept;
	std::list<Entity*> mEnemyEntities{};
	Texture mItemTextures[2];
	Rectangle mItemRectangles[3];

	int mEnemyCounter = 0;
	bool mIsPaused = false;

	int mThreshold = 2;
	int mEnemyAttack = 1;
	float mSpeedFactor = 1.0f;
	int mTimer = 60;
	int mScore = 0;
	int mCounter = 0;
};

