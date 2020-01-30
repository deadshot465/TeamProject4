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

	constexpr int GetScore() const { return mScore; }
private:
	void GenerateEnemies(float deltaTime, float speedFactor, int attack, int threshold, float frequencyFactor = 0.9f);
	void HandleSelection();

	std::vector<CircleCollider*> GetAllEnemyColliders() const noexcept;
	std::list<Entity*> mEnemyEntities{};

	int mEnemyCounter = 0;
	bool mIsPaused = false;
	bool mIsGameClear = false;
	bool mIsGameOver = false;

	int mThreshold = 10;
	int mEnemyAttack = 1;
	float mSpeedFactor = 1.0f;
	int mTimer = 60;
	int mScore = 0;
	int mCounter = 0;
};

