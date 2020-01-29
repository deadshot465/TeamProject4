#pragma once
#include "ECS.h"
#include <string_view>

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
	virtual void SetupPlayer();
	virtual void SetupShield();

	Entity* mPlayerEntity = nullptr;
	Entity* mShieldEntity = nullptr;
};

