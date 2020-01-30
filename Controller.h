#pragma once
#include "ECS.h"
#include "Animator.h"

class PlayerController :
	public Component
{
public:
	PlayerController(float speed);
	~PlayerController();

	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	float Speed = 0.0f;
	int Hp = 6;
	std::vector<bool> ShieldCooldowns = std::vector<bool>(2);
	bool IsInvincible = false;

	constexpr bool IsAttacking() const noexcept { return mAttackFlag; }

private:
	void HandleShield();
	void ClampToScreenSize();

	bool mAttackFlag = false;
	int mCounter = 0;
	int mCoolingdownShieldCount = 0;
	int mCooldownTimer = 0;

#define PLAYER_ANIMATOR (mEntity->GetComponent<Animator>())
#define NORMAL_RIGHT 0, "normal-right"
#define RUN_RIGHT 1, "run-right"
#define NORMAL_LEFT 2, "normal-left"
#define RUN_LEFT 3, "run-left"
};

class EnemyController
	: public Component
{
public:
	enum class EnemyState
	{
		Initializing, Moving, Attached, Finalizing
	};

	EnemyController(float speed, int attack);
	~EnemyController();

	constexpr bool ShouldDestroy() const noexcept { return mShouldDestroy; }

	float Speed = 0.0f;
	int Attack = 0;
	glm::vec2 Velocity = {};
	EnemyState CurrentState = {};
	
protected:
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

private:
	glm::vec2 mPlayerPosition = {};
	bool mSpeedUpdateStopped = false;
	bool mShouldDestroy = false;

#define ENEMY_ANIMATOR (mEntity->GetComponent<Animator>())
#define NORMAL_RIGHT 0, "normal-right"
#define RUN_RIGHT 1, "run-right"
#define NORMAL_LEFT 2, "normal-left"
#define RUN_LEFT 3, "run-left"
};

class ShieldController
	: public Component
{
public:
	ShieldController(int offset);
	~ShieldController() = default;

	int Offset = 0;

protected:
	// Component ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
