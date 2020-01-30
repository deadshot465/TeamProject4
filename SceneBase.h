#pragma once
#include "ECS.h"
#include <string_view>
#include <vector>

class SceneBase :
	public Scene
{
public:
	SceneBase();
	explicit SceneBase(std::string_view name);
	virtual ~SceneBase();

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	virtual void SetupUIEntities();
	virtual void SetupPlayer();
	virtual void SetupShield();
	virtual void UpdateHpUI();
	virtual void UpdateShieldUI();

	Entity* mHpUIEntities[3];
	std::vector<Entity*> mShieldUIEntities = std::vector<Entity*>(2);
	Texture mShieldMask;

	Entity* mPlayerEntity = nullptr;
	Entity* mShieldEntity = nullptr;

#define PLAYER_CONTROLLER (mPlayerEntity->GetComponent<PlayerController>())
};

